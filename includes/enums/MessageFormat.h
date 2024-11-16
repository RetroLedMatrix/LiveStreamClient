//
// Created by marco on 16/11/2024.
//

#ifndef MESSAGEFORMAT_H
#define MESSAGEFORMAT_H
#include <string>

enum MessageFormat {
    ALL_PIXELS,
    RESET,
    UNKNOWN
};

inline MessageFormat stringToMessageFormat(const std::string &formatString) {
    if (formatString == "all") {
        return ALL_PIXELS;
    }
    if (formatString == "reset") {
        return RESET;
    }
    return UNKNOWN;
}

#endif //MESSAGEFORMAT_H
