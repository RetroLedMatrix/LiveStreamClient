//
// Created by marco on 15/11/2024.
//

#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

#include "WebSocketConnection.h"
#include <string>

class WebSocketClient {
public:
    WebSocketClient(const std::string &address, const std::string &port, const std::string &path);

    ~WebSocketClient();

    void run();

private:
    WebSocketConnection connection;
};

#endif // WEBSOCKET_CLIENT_H
