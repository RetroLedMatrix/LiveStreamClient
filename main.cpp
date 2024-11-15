//
// Created by Katie on 15/11/2024.
//

#include "brainsBehindScreenshot.h"
#include <iostream>

int main() {
    ScreenCapture screenCapture;

    std::wstring filename = L"screenshot.png";

    if (screenCapture.CaptureScreen(filename)) {
        std::wcout << L"Screenshot saved to " << filename << std::endl;
    } else {
        std::wcerr << L"Failed to capture the screen." << std::endl;
    }

    return 0;
}
