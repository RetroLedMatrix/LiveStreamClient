//
// Created by marco on 15/11/2024.
//

#include "../../includes/WebSocketClient.h"
#include <iostream>

WebSocketClient::WebSocketClient(const std::string &address, const std::string &port, const std::string &path)
    : connection(address, port, path) {
}

WebSocketClient::~WebSocketClient() {
    WSACleanup();
}

void WebSocketClient::run() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return;
    }

    if (!connection.connect()) {
        std::cerr << "Failed to connect to WebSocket server" << std::endl;
        return;
    }

    std::cout << "Type 'exit' to terminate" << std::endl;
    std::string userInput;
    while (true) {
        std::cout << ">>";
        std::getline(std::cin, userInput);

        if (userInput == "exit") {
            break;
        }

        connection.sendMessage(userInput);
        std::string response = connection.receiveMessage();
        std::cout << "Received: " << response << std::endl;
    }

    connection.closeConnection();
}
