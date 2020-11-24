#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
/* https://github.com/Links2004/arduinoWebSockets */ 
#include <WebSocketsServer.h>
#include <TM1638plus.h>
#include <string>
#include <vector>
#include <cstdint>
#include <map>

// tm1638 config
#define STROBE_TM 4    // strobe = GPIO connected to strobe line of module
#define CLOCK_TM 16    // clock = GPIO connected to clock line of module
#define DIO_TM 17      // data = GPIO connected to data line of module
bool high_freq = true; // Required for ESP32

// Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)
TM1638plus tm(STROBE_TM, CLOCK_TM, DIO_TM, high_freq);

// Constants
const char *ssid = "ESP32-AP";
const char *password = "LetMeInPlz";
const char *msg_toggle_led = "toggleLED";
const char *msg_get_led = "getLEDState";
const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;

// Globals
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(1337);
char msg_buf[10];
int led_state = 0;

// Handler
void handleCommand(std::string *type, std::vector<uint8_t> *params);
bool parseCommand(std::string *buffer, std::string *type, std::vector<uint8_t> *params);
void setLedBundleHandler(std::vector<uint8_t> *params);
void getLedBundleHandler();

std::vector<std::string> commands = {"SET_LED_BUNDLE", "GET_LED_BUNDLE"};
uint8_t ledArrayState[8] = {0};

// Callback: receiving any WebSocket message
void onWebSocketEvent(uint8_t client_num,
                      WStype_t type,
                      uint8_t *payload,
                      size_t length)
{

  // Figure out the type of WebSocket event
  switch (type)
  {

  // Client has disconnected
  case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", client_num);
    break;

  // New client has connected
  case WStype_CONNECTED:
  {
    IPAddress ip = webSocket.remoteIP(client_num);
    Serial.printf("[%u] Connection from ", client_num);
    Serial.println(ip.toString());
  }
  break;

  // Handle text messages from client
  case WStype_TEXT:
  {

    // Print out raw message
    Serial.printf("[%u] Received text: %s\n", client_num, payload);

    std::string buffer_str((char *)payload);

    std::vector<uint8_t> parameters;
    std::string command_type("");

    bool parsingResult = parseCommand(&buffer_str, &command_type, &parameters);
    if (parsingResult)
    {
      handleCommand(&command_type, &parameters);
    }
  }

  // Toggle LED
  // if (strcmp((char *)payload, "toggleLED") == 0)
  // {
  //   led_state = led_state ? 0 : 1;
  //   Serial.printf("Toggling LED to %u\n", led_state);
  //   tm.setLED(0, led_state);

  //   // Report the state of the LED
  // }
  // else if (strcmp((char *)payload, "getLEDState") == 0)
  // {
  //   sprintf(msg_buf, "%d", led_state);
  //   Serial.printf("Sending to [%u]: %s\n", client_num, msg_buf);
  //   webSocket.sendTXT(client_num, msg_buf);

  // Message not recognized
  // }
  // else
  // {
  //   Serial.println("[%u] Message not recognized");
  // }
  break;

  // For everything else: do nothing
  case WStype_BIN:
  case WStype_ERROR:
  case WStype_FRAGMENT_TEXT_START:
  case WStype_FRAGMENT_BIN_START:
  case WStype_FRAGMENT:
  case WStype_FRAGMENT_FIN:
  default:
    break;
  }
}

// Callback: send homepage
void onIndexRequest(AsyncWebServerRequest *request)
{
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                 "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/index.html", "text/html");
}

// Callback: send style sheet
void onCSSRequest(AsyncWebServerRequest *request)
{
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                 "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/style.css", "text/css");
}

// Callback: send 404 if requested file does not exist
void onPageNotFound(AsyncWebServerRequest *request)
{
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                 "] HTTP GET request of " + request->url());
  request->send(404, "text/plain", "Not found");
}

/***********************************************************
 * Main
 */

void setup()
{
  tm.displayBegin();
  // Init LED and turn off
  tm.setLED(0, LOW);

  // Start Serial port
  Serial.begin(115200);

  // Make sure we can read the file system
  if (!SPIFFS.begin())
  {
    Serial.println("Error mounting SPIFFS");
    while (1)
      ;
  }

  // Start access point
  WiFi.softAP(ssid, password);

  // Print our IP address
  Serial.println();
  Serial.println("AP running");
  Serial.print("My IP address: ");
  Serial.println(WiFi.softAPIP());

  // On HTTP request for root, provide index.html file
  server.on("/", HTTP_GET, onIndexRequest);

  // On HTTP request for style sheet, provide style.css
  server.on("/style.css", HTTP_GET, onCSSRequest);

  // Handle requests for pages that do not exist
  server.onNotFound(onPageNotFound);

  // Start web server
  server.begin();

  // Start WebSocket server and assign callback
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}

void loop()
{

  // Look for and handle WebSocket data
  webSocket.loop();
}

bool parseCommand(std::string *buffer, std::string *type, std::vector<uint8_t> *params)
{
  (*type) = "";
  for (std::string command : commands)
  {
    if ((*buffer).find(command) != -1)
    {
      (*type) = command;
    }
  }
  if ((*type) == "")
    return false;

  bool readingParam = false;
  std::string currentParam;

  for (int i = 0; i < (*buffer).length(); i++)
  {
    if ((*buffer)[i] == ']')
    {
      readingParam = false;
      (*params).push_back(atoi(currentParam.c_str()));
    }
    if (readingParam)
      currentParam.push_back((*buffer)[i]);

    if ((*buffer)[i] == '[')
    {
      readingParam = true;
      currentParam = "";
    }
  }

  return true;
}

void handleCommand(std::string *type, std::vector<uint8_t> *params)
{
  // std::cout << "Handling command " << (*type) << std::endl;
  // std::cout << "Printing params\n";
  // for (uint8_t param : *params)
  // {
  //   std::string s = std::to_string(param);
  //   std::cout << s << std::endl;
  // }

  if ((*type) == "SET_LED_BUNDLE")
  {
    setLedBundleHandler(params);
  }
  else if ((*type) == "GET_LED_BUNDLE")
  {
    getLedBundleHandler();
  }
}

void setLedBundleHandler(std::vector<uint8_t> *params)
{
  for (int i = 0; i < 8; i++)
  {
    if ((*params)[i] == 0)
    {
      ledArrayState[i] = 0;
      tm.setLED(i, 0);
    }
    else
    {
      ledArrayState[i] = 1;
      tm.setLED(i, 1);
    }
  }
}

void getLedBundleHandler()
{
  std::string buffer;
  buffer += "SET_LED_BUNDLE";
  for (int i = 0; i < 8; i++)
  {
    buffer += "[";
    if (ledArrayState[i])
    {
      buffer += "1";
    }
    else
    {
      buffer += "0";
    }
    buffer += "]";
  }

  /* send through WebSocket */
}