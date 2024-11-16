#include "includes/WebSocketClient.h"

int main() {
    WebSocketClient client("localhost", "8080", "/publisher/matrixMarco");
    client.run();
    return 0;
}
