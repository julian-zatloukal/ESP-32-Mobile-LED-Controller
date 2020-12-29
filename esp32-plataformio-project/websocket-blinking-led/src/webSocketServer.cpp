#include "webSocketServer.h"
#include "commandParser.h"

#define DNS_PORT 53
#define HTTP_PORT 80
#define WS_PORT 1337

// Globals
AsyncWebServer server(HTTP_PORT);
AsyncWebSocket ws("/ws"); // access at ws://[esp ip]/ws

// WebSocketsServer webSocket = WebSocketsServer(WS_PORT);

// Callback: receiving any WebSocket message
// void onWebSocketEvent(uint8_t client_num,
//                       WStype_t type,
//                       uint8_t *payload,
//                       size_t length)
// {

//     // Figure out the type of WebSocket event
//     switch (type)
//     {

//     // Client has disconnected
//     case WStype_DISCONNECTED:
//         Serial.printf("[%u] Disconnected!\n", client_num);
//         break;

//     // New client has connected
//     case WStype_CONNECTED:
//     {
//         IPAddress ip = webSocket.remoteIP(client_num);
//         Serial.printf("[%u] Connection from ", client_num);
//         Serial.println(ip.toString());
//     }
//     break;

//     // Handle text messages from client
//     case WStype_TEXT:
//     {

//         // Print out raw message
//         Serial.printf("[%u] Received text: %s\n", client_num, payload);

//         std::string buffer_str((char *)payload);

//         std::vector<uint8_t> parameters;
//         std::string command_type("");

//         bool parsingResult = parseCommand(&buffer_str, &command_type, &parameters);
//         if (parsingResult)
//         {
//             handleCommand(&command_type, &parameters);
//         }
//     }

//     break;

//     // For everything else: do nothing
//     case WStype_BIN:
//     case WStype_ERROR:
//     case WStype_FRAGMENT_TEXT_START:
//     case WStype_FRAGMENT_BIN_START:
//     case WStype_FRAGMENT:
//     case WStype_FRAGMENT_FIN:
//     default:
//         break;
//     }
// }

void onRequest(AsyncWebServerRequest *request)
{
  //Handle Unknown Request
  request->send(404);
}

void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  //Handle body
}

void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  //Handle upload
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  //Handle WebSocket event

  if (type == WS_EVT_CONNECT)
  {
    //client connected
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    client->printf("Hello Client %u :)", client->id());
    client->ping();
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    //client disconnected
    Serial.printf("ws[%s][%u] disconnect\n", server->url(), client->id());
  }
  else if (type == WS_EVT_ERROR)
  {
    //error was received from the other end
    Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t *)arg), (char *)data);
  }
  else if (type == WS_EVT_PONG)
  {
    //pong message was received (in response to a ping request maybe)
    Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len) ? (char *)data : "");
  }
  else if (type == WS_EVT_DATA)
  {
    //data packet
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len)
    {
      //the whole message is in a single frame and we got all of it's data
      Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT) ? "text" : "binary", info->len);
      if (info->opcode == WS_TEXT)
      {
        data[len] = 0;
        Serial.printf("%s\n", (char *)data);
      }
      else
      {
        for (size_t i = 0; i < info->len; i++)
        {
          Serial.printf("%02x ", data[i]);
        }
        Serial.printf("\n");
      }
      if (info->opcode == WS_TEXT)
        client->text("I got your text message");
      else
        client->binary("I got your binary message");
    }
    else
    {
      //message is comprised of multiple frames or the frame is split into multiple packets
      if (info->index == 0)
      {
        if (info->num == 0)
          Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
        Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? "text" : "binary", info->index, info->index + len);
      if (info->message_opcode == WS_TEXT)
      {
        data[len] = 0;
        Serial.printf("%s\n", (char *)data);
      }
      else
      {
        for (size_t i = 0; i < len; i++)
        {
          Serial.printf("%02x ", data[i]);
        }
        Serial.printf("\n");
      }

      if ((info->index + len) == info->len)
      {
        Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if (info->final)
        {
          Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
          if (info->message_opcode == WS_TEXT)
            client->text("I got your text message");
          else
            client->binary("I got your binary message");
        }
      }
    }
  }
}

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

void webSocketServer::initAccessPoint(const char *ssid, const char *password)
{
  // // Init access point
  // WiFi.softAP(ssid, password);

  // // Print the ESP32 IP address
  // Serial.println();
  // Serial.println("AP running");
  // Serial.print("My IP address: ");
  // Serial.println(WiFi.softAPIP());

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());
}

void webSocketServer::initServer()
{
  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
  //   request->send(200, "text/plain", "Connected.");
  // });

  // server.onNotFound(notFound);

  // attach AsyncWebSocket
  ws.onEvent(onEvent);
  server.addHandler(&ws);

  // server.onNotFound(onRequest);
  // server.onFileUpload(onUpload);
  // server.onRequestBody(onBody);

  // Start web server
  server.begin();

  // Start WebSocket server and assign callback
  // webSocket.begin();
  // webSocket.onEvent(onWebSocketEvent);
}

void webSocketServer::handleData()
{
  // ws.cleanupClients();
}