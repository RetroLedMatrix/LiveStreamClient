#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

// Function to base64 encode a string (used for WebSocket handshake key)
std::string base64_encode(const std::string& input) {
    static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string output;
    int val = 0, valb = -6;
    for (char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            output.push_back(table[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) output.push_back(table[((val << 8) >> (valb + 8)) & 0x3F]);
    while (output.size() % 4) output.push_back('=');
    return output;
}

// Function to generate a random WebSocket key
std::string generate_websocket_key() {
    const std::string key = "random_key_for_websocket";
    return base64_encode(key);
}

// Function to perform WebSocket handshake
bool websocket_handshake(SOCKET sock, const char* server_address, const char* server_port, const char* server_path) {
    std::string key = generate_websocket_key();
    std::ostringstream handshake;
    handshake << "GET " << server_path << " HTTP/1.1\r\n"
              << "Host: " << server_address << ":" << server_port << "\r\n"
              << "Upgrade: websocket\r\n"
              << "Connection: Upgrade\r\n"
              << "Sec-WebSocket-Key: " << key << "\r\n"
              << "Sec-WebSocket-Version: 13\r\n\r\n";

    std::string request = handshake.str();
    send(sock, request.c_str(), request.size(), 0);

    // Read the server response
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::cout << "Server response:\n" << buffer << std::endl;
        if (strstr(buffer, "101 Switching Protocols") != nullptr) {
            return true;
        }
    }
    return false;
}

// Function to send a WebSocket frame
void send_websocket_frame(SOCKET sock, const std::string& message) {
    std::vector<unsigned char> frame;
    frame.push_back(0x81); // FIN + Text frame opcode
    frame.push_back(static_cast<unsigned char>(message.size()));
    frame.insert(frame.end(), message.begin(), message.end());
    send(sock, reinterpret_cast<const char*>(frame.data()), frame.size(), 0);
}

// Function to receive a WebSocket frame
std::string receive_websocket_frame(SOCKET sock) {
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);

    if (bytes_received > 0) {
        int payload_length = buffer[1] & 0x7F;
        return std::string(buffer + 2, payload_length);
    }
    return "";
}

int main() {
    const char* server_address = "localhost";
    const char* server_port = "8080";
    const char* server_path = "/publisher/matrix";

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

    std::string message = "Hello, WebSocket!";
    send_websocket_frame(sock, message);
    std::cout << "Sent: " << message << std::endl;

    std::string response = receive_websocket_frame(sock);
    std::cout << "Received: " << response << std::endl;

    closesocket(sock);
    WSACleanup();

    return 0;
}