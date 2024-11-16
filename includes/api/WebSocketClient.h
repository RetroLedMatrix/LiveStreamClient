//
// Created by marco on 15/11/2024.
//

#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

#include "../../includes/api/WebSocketConnection.h"
#include <string>

constexpr int MATRIX_WIDTH = 128;
constexpr int MATRIX_HEIGHT = 64;

class WebSocketClient {
public:
    WebSocketClient(const std::string &address, const std::string &port, const std::string &path);

    ~WebSocketClient();

    void testAllPixels() const;

    void reset() const;

    static void takeScreenShot();

    void startStream();

    void run();

private:
    WebSocketConnection connection;
};

#endif // WEBSOCKET_CLIENT_H
