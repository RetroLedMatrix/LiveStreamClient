#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "includes/websocket.h"

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return -1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return -1;
    }

    const char *server_address = "localhost";
    const char *server_port = "65432";
    const char *server_path = "/";

    struct addrinfo hints{}, *result;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(server_address, server_port, &hints, &result) != 0) {
        std::cerr << "getaddrinfo failed" << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    if (connect(sock, result->ai_addr, static_cast<int>(result->ai_addrlen)) != 0) {
        std::cerr << "Connection failed" << std::endl;
        freeaddrinfo(result);
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    freeaddrinfo(result);

    if (!websocket_handshake(sock, server_address, server_port, server_path)) {
        std::cerr << "WebSocket handshake failed" << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    std::cout << "WebSocket handshake succeeded!" << std::endl;

    std::string message = "Hello, WebSocket!";
    send_websocket_frame(sock, message);
    std::cout << "Sent: " << message << std::endl;

    std::string response = receive_websocket_frame(sock);
    std::cout << "Received: " << response << std::endl;

    closesocket(sock);
    WSACleanup();

    return 0;
}
