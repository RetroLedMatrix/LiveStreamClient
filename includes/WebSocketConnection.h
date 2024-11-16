//
// Created by marco on 16/11/2024.
//

#ifndef WEBSOCKET_CONNECTION_H
#define WEBSOCKET_CONNECTION_H

#include <winsock2.h>
#include <string>
#include <vector>

class WebSocketConnection {
public:
    WebSocketConnection(std::string address, std::string port, std::string path);

    ~WebSocketConnection();

    bool connect();

    [[nodiscard]] bool isConnected() const;

    void sendMessage(const std::string &message) const;

    [[nodiscard]] std::string receiveMessage() const;

    void closeConnection();

private:
    SOCKET sock;
    std::string server_address;
    std::string server_port;
    std::string server_path;

    static std::string generateWebSocketKey();

    [[nodiscard]] bool performHandshake() const;

    static std::string base64Encode(const std::vector<unsigned char> &input);
};

#endif // WEBSOCKET_CONNECTION_H
