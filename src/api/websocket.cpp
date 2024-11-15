//
// Created by marco on 15/11/2024.
//

#include "../../includes/websocket.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <cstring>

#define BUFFER_SIZE 1024

std::string base64_encode(const std::vector<unsigned char> &input) {
    static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string output;
    int val = 0, valb = -6;
    for (unsigned char c: input) {
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

std::string generate_websocket_key() {
    std::vector<unsigned char> random_bytes(16);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 255);
    for (auto &byte: random_bytes) {
        byte = dis(gen);
    }
    return base64_encode(random_bytes);
}

bool websocket_handshake(SOCKET sock, const char *server_address, const char *server_port, const char *server_path) {
    std::string key = generate_websocket_key();
    std::ostringstream handshake;
    handshake << "GET " << server_path << " HTTP/1.1\r\n"
              << "Host: " << server_address << ":" << server_port << "\r\n"
              << "Upgrade: websocket\r\n"
              << "Connection: Upgrade\r\n"
              << "Sec-WebSocket-Key: " << key << "\r\n"
              << "Sec-WebSocket-Version: 13\r\n"
              << "Origin: http://" << server_address << "\r\n\r\n";

    std::string request = handshake.str();
    send(sock, request.c_str(), request.size(), 0);

    char buffer[BUFFER_SIZE];
    int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        return strstr(buffer, "101 Switching Protocols") != nullptr;
    }
    return false;
}

void send_websocket_frame(SOCKET sock, const std::string &message) {
    std::vector<unsigned char> frame;
    frame.push_back(0x81);

    const bool mask = true;
    std::vector<unsigned char> masking_key(4);
    std::random_device rd;
    for (int i = 0; i < 4; ++i) {
        masking_key[i] = rd() % 256;
    }

    size_t payload_len = message.size();
    if (payload_len <= 125) {
        frame.push_back(static_cast<unsigned char>(payload_len) | (mask ? 0x80 : 0));
    } else if (payload_len <= 65535) {
        frame.push_back(126 | (mask ? 0x80 : 0));
        frame.push_back((payload_len >> 8) & 0xFF);
        frame.push_back(payload_len & 0xFF);
    } else {
        frame.push_back(127 | (mask ? 0x80 : 0));
        for (int i = 7; i >= 0; --i) {
            frame.push_back((payload_len >> (i * 8)) & 0xFF);
        }
    }

    if (mask) {
        frame.insert(frame.end(), masking_key.begin(), masking_key.end());
    }

    for (size_t i = 0; i < message.size(); ++i) {
        frame.push_back(message[i] ^ masking_key[i % 4]);
    }

    send(sock, reinterpret_cast<const char *>(frame.data()), frame.size(), 0);
}

std::string receive_websocket_frame(SOCKET sock) {
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
    if (bytes_received <= 0) {
        return "";
    }

    if ((buffer[0] & 0x0F) != 0x1) {
        return "";
    }

    int payload_len = buffer[1] & 0x7F;
    int offset = 2;

    if (payload_len == 126) {
        payload_len = (buffer[2] << 8) | buffer[3];
        offset += 2;
    } else if (payload_len == 127) {
        payload_len = 0;
        for (int i = 0; i < 8; ++i) {
            payload_len = (payload_len << 8) | buffer[offset++];
        }
    }

    bool mask = buffer[1] & 0x80;
    unsigned char masking_key[4];
    if (mask) {
        std::memcpy(masking_key, buffer + offset, 4);
        offset += 4;
    }

    std::string message(buffer + offset, payload_len);
    if (mask) {
        for (int i = 0; i < payload_len; ++i) {
            message[i] ^= masking_key[i % 4];
        }
    }

    return message;
}
