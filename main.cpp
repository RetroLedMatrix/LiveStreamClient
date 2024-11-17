//
// Created by Katie on 15/11/2024.
//

#include <chrono>
#include <iostream>
#include <thread>
#include "includes/api/WebSocketClient.h"

int main() {
    std::string address;
    std::string port;
    std::string endpoint;

    int connectionStatus = 0;
    do {
        std::cout << "Enter WebSocket server address:\n";
        std::cin >> address;

        std::cout << "Enter WebSocket server port:\n";
        std::cin >> port;

        std::cout << "Enter WebSocket endpoint:\n";
        std::cin >> endpoint;

        WebSocketClient client(address, port, "/publisher/" + endpoint);
        connectionStatus = client.run();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (connectionStatus != 0);

    // WebSocketClient client("192.168.1.102", "8080", "/publisher/matrix");
    // WebSocketClient client("localhost", "8080", "/publisher/matrix-test");

    return 0;
}
