//
// Created by Katie on 15/11/2024.
//

#include <functional>
#include <iostream>

#include "includes/api/WebSocketClient.h"
#include "includes/BrainsBehindScreenshot.h"
#include <thread>


int main() {
    // fingers crossed lol
    ScreenCapture screencapture = ScreenCapture();
    auto bmp = screencapture.CaptureScreenBitmap();



    WebSocketClient client("localhost", "8080", "/publisher/matrixMarco");
    //threading
    std::thread client_thread(std::bind(&WebSocketClient::run, &client));

    for(int i = 0; i < 100; i++) {
        std::cout << "testing" << i << std::endl;
    }
    client_thread.join();


    //client.run();
    return 0;
}
