#include <iostream>
#include "includes/websocket_client.h"

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return false;
    }

    websocket_client client("localhost", "65432", "/");

    if (!client.connect()) {
        std::cerr << "Failed to connect to WebSocket server" << std::endl;
        return -1;
    }

    std::cout << "Type 'exit' to terminate :)" << std::endl;

    std::string userInput;
    while (true) {
        std::cout << ">>";
        std::getline(std::cin, userInput);

        if (userInput.empty()) {
            continue;
        }

        if (userInput == "exit") {
            break;
        }

        client.sendMessage(userInput);
        std::string response = client.receiveMessage();
        std::cout << "Received: " << response << std::endl;
    }

    client.closeConnection();
    return 0;
}
