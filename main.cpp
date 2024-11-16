#include "includes/WebSocketClient.h"

int main() {
    WebSocketClient client("localhost", "65432", "/");
    client.run();
    return 0;
}
