//
// Created by Katie on 15/11/2024.
//

#include "includes/api/WebSocketClient.h"

int main() {
    WebSocketClient client("192.168.1.102", "8080", "/publisher/matrix");
    // WebSocketClient client("localhost", "8080", "/publisher/matrix-test");
    client.run();
    return 0;
}
