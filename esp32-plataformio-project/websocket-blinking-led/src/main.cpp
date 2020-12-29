/* Arduino and ESP32 libraries */
#include <Arduino.h>

/* Project libraries */
/* guide include own headers: https://community.platformio.org/t/include-header-file-from-include-folder-via-build-flags/15195 */
#include "commandParser.h"
#include "ledModule.h"
#include "webSocketServer.h"

// Configuration
// const char *ssid = "ESP32-AP";
// const char *password = "abc12345";

const char *ssid = "Tunis1";
const char *password = "tunistunis";

/* 
  MAC Address:  24:62:AB:F9:6A:BD
  IP Address:   192.168.4.1
*/

void setup()
{
  // Init TM1638 module
  ledModule::init();

  // Init Serial port
  Serial.begin(9600);

  webSocketServer::initAccessPoint(ssid, password);

  webSocketServer::initServer();
}

void loop()
{
  // Look for and handle WebSocket data
  webSocketServer::handleData();
}
