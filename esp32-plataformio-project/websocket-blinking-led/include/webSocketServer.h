#pragma once
#ifndef WEB_SOCKET_SERVER_H_
#define WEB_SOCKET_SERVER_H_

#include <WiFi.h>

/*  
    Useful documentation:
        https://github.com/me-no-dev/ESPAsyncWebServer
        https://github.com/me-no-dev/ESPAsyncWebServer#setting-up-the-server
        https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/examples/simple_server/simple_server.ino
*/
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
/* https://github.com/Links2004/arduinoWebSockets */
#include <WebSocketsServer.h>


namespace webSocketServer {
    void initAccessPoint(const char *ssid, const char *password);
    void initServer();
    void handleData();
}

#endif