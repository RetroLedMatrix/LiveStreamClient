//
// Created by marco on 15/11/2024.
//

#include <iostream>
#include "../../includes/Server.h"
#include "../../includes/libs/httplib.h"

void start_server() {
    httplib::Server svr;

    std::string server_url = "http://localhost:" + std::to_string(REST_SERVER_PORT);

    svr.Get("/", [server_url](const httplib::Request &, httplib::Response &res) {
        res.set_content("Server is running at " + server_url, "text/html");
    });

    svr.Get("/hello", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("Hello, World!", "text/plain");
    });

    svr.Post("/echo", [](const httplib::Request &req, httplib::Response &res) {
        res.set_content(req.body, "text/plain");
    });

    std::cout << "Server is running at " << server_url << std::endl;

    if (!svr.listen("127.0.0.1", REST_SERVER_PORT)) {
        std::cerr << "Error: Failed to start the server on port " << REST_SERVER_PORT << "!" << std::endl;
    }
}
