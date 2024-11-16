cmake_minimum_required(VERSION 3.29)
project(LiveStreamClient VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(LiveStreamClient
        main.cpp
        includes/httplib.h
        includes/server.h
        includes/WebSocketClient.h
        src/api/server.cpp
        src/api/WebSocketClient.cpp
)

# Link static libraries to the executable target
# (In this case we need this for the lib 'libstdc++-6.dll' and some more)
# ----------------------------------------------------------------
# "PRIVATE" means that the libraries are only linked
# to the "ftp_client" target and not propagated to
# any other targets that link to "ftp_client".
# This is useful for keeping dependencies encapsulated within a target.
target_link_libraries(LiveStreamClient PRIVATE
        -static
        ws2_32
        gdiplus
)