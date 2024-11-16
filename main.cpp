//
// Created by Katie on 15/11/2024.
//

#include "includes/api/WebSocketClient.h"
#include "includes/BrainsBehindScreenshot.h"

int main() {
    // fingers crossed lol
    ScreenCapture screencapture = ScreenCapture();
    auto bmp = screencapture.CaptureScreenBitmap();


    WebSocketClient client("localhost", "8080", "/publisher/matrixMarco");
    client.run();
    return 0;
}
