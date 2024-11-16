//
// Created by Katie on 15/11/2024.
//

#include "brainsBehindScreenshot.h"
#include <iostream>
#include "includes/WebSocketClient.h"

int main() {
    ScreenCapture screenCapture;

    std::wstring filename = L"screenshot.bmp";

    if (screenCapture.CaptureScreen(filename)) {
        std::wcout << L"Screenshot saved to " << filename << std::endl;
    } else {
        std::wcerr << L"Failed to capture the screen." << std::endl;
    }

    WebSocketClient client("localhost", "8080", "/publisher/matrixMarco");
    client.run();

    return 0;
}

