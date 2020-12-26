/* Arduino and ESP32 libraries */
#include <Arduino.h>
#include <WiFi.h>

/* Command parser library */
/* include own headers: https://community.platformio.org/t/include-header-file-from-include-folder-via-build-flags/15195 */
#include "commandParser.h"
#include "ledModule.h"

/* https://github.com/me-no-dev/ESPAsyncWebServer */
#include <ESPAsyncWebServer.h>
/* https://github.com/Links2004/arduinoWebSockets */
#include <WebSocketsServer.h>

// Constants
const char *ssid = "ESP32-AP";
const char *password = "abc12345";
const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;

// Globals
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(1337);
char msg_buf[10];
int led_state = 0;

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

void setup()
{
  // Init TM1638 module
  ledModule::init();
  ledModule::setLED(0, LOW);

  // Init Serial port
  Serial.begin(115200);

  // Init access point
  WiFi.softAP(ssid, password);

  // Print the ESP32 IP address
  Serial.println();
  Serial.println("AP running");
  Serial.print("My IP address: ");
  Serial.println(WiFi.softAPIP());

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
