//
// Created by marco on 16/11/2024.
//

#ifndef MESSAGEFORMAT_H
#define MESSAGEFORMAT_H

#include <string>

enum Commands {
    EXIT,
    ALL,
    RESET,
    STREAM,
    HELP,
    UNKNOWN
};

inline Commands stringToCommand(const std::string &commandStr) {
    if (commandStr == "exit") {
        return EXIT;
    }
    if (commandStr == "all") {
        return ALL;
    }
    if (commandStr == "reset") {
        return RESET;
    }
    if (commandStr == "stream") {
        return STREAM;
    }
    if (commandStr == "help") {
        return HELP;
    }
    return UNKNOWN;
}

inline void printHelp() {
    std::cout << "Available commands:\n"
              << "  exit   - Terminate the program\n"
              << "  all    - Test all pixels\n"
              << "  reset  - Turn off all pixels\n"
              << "  stream - Start streaming\n"
              << "  help   - Show this help message\n";
}

#endif //MESSAGEFORMAT_H
