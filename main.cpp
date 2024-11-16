//
// Created by Katie on 15/11/2024.
//

#include "includes/BrainsBehindScreenshot.h"
#include "includes/WebSocketClient.h"

int main() {
    // fingers crossed lol
    ScreenCapture screencapture = ScreenCapture();
    auto bmp = screencapture.CaptureScreenBitmap();


    WebSocketClient client("localhost", "8080", "/publisher/matrixMarco");
    client.run();
    return 0;
}
