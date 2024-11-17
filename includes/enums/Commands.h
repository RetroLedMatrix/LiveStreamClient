//
// Created by marco on 16/11/2024.
//

#ifndef MESSAGEFORMAT_H
#define MESSAGEFORMAT_H

#include <string>

enum Commands {
    PIXELS,
    ALL,
    RESET,
    TAKE,
    START,
    SAVE,
    HELP,
    EXIT,
    CLOSE,
    UNKNOWN
};

inline Commands stringToCommand(const std::string &commandStr) {
    if (commandStr == "pixels") {
        return PIXELS;
    }
    if (commandStr == "all") {
        return ALL;
    }
    if (commandStr == "reset" || commandStr == "clear") {
        return RESET;
    }
    if (commandStr == "take") {
        return TAKE;
    }
    if (commandStr == "start") {
        return START;
    }
    if (commandStr == "save") {
        return SAVE;
    }
    if (commandStr == "help") {
        return HELP;
    }
    if (commandStr == "exit") {
        return EXIT;
    }
    if (commandStr == "close") {
        return CLOSE;
    }
    return UNKNOWN;
}

inline void printHelp() {
    std::cout << "Available commands:\n"
            << "  pixels - Test some pixels\n"
            << "  all    - Test all pixels\n"
            << "  reset  - Turn off all pixels\n"
            << "  take   - Take a screenshot and send it\n"
            << "  start  - Start streaming\n"
            << "  save   - Take a screenshot and save it\n"
            << "  help   - List available commands\n"
            << "  close  - Close the websocket connection\n"
            << "  exit   - Terminate the program\n";
}

#endif //MESSAGEFORMAT_H
