//
// Created by marco on 15/11/2024.
//

#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <winsock2.h>
#include <string>
#include <vector>

class WebSocketClient {
public:
    WebSocketClient(std::string address, std::string port, std::string path);

    ~WebSocketClient();

    bool connect();

    void closeConnection();

    void sendMessage(const std::string &message) const;

    [[nodiscard]] std::string receiveMessage() const;

private:
    SOCKET sock;
    std::string server_address;
    std::string server_port;
    std::string server_path;

    static std::string generateWebSocketKey();

    [[nodiscard]] bool performHandshake() const;

    static std::string base64Encode(const std::vector<unsigned char> &input);
};

#endif // WEBSOCKETCLIENT_H
