//
// Created by marco on 15/11/2024.
//

#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

// Function declarations
std::string generate_websocket_key();
bool websocket_handshake(SOCKET sock, const char *server_address, const char *server_port, const char *server_path);
void send_websocket_frame(SOCKET sock, const std::string &message);
std::string receive_websocket_frame(SOCKET sock);

#endif // WEBSOCKET_H
