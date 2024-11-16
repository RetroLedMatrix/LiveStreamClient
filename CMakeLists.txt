cmake_minimum_required(VERSION 3.29)
project(LiveStreamClient VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(LiveStreamClient main.cpp
        brainsBehindScreenshot.cpp
        brainsBehindScreenshot.h)

#add_executable(ftp_client
#        main.cpp
#        includes/FtpSocket.h
#        includes/FtpClient.h
#        includes/enums/FtpServerResponseCode.h
#        includes/enums/FtpCommand.h
#        src/FtpSocket.cpp
#        src/FtpClient.cpp
#        includes/exeptions/LoginFailureException.h
#        includes/exeptions/ParsePasvFailureException.h
#        includes/exeptions/SocketConnectionFailureException.h
#        includes/exeptions/SocketDataFailureException.h
#)

# Link static libraries to the executable target
# (In this case we need this for the lib 'libstdc++-6.dll' and some more)
# ----------------------------------------------------------------
# "PRIVATE" means that the libraries are only linked
# to the "ftp_client" target and not propagated to
# any other targets that link to "ftp_client".
# This is useful for keeping dependencies encapsulated within a target.
target_link_libraries(LiveStreamClient
        PRIVATE -static
        PRIVATE ws2_32
)

target_link_libraries(LiveStreamClient PRIVATE gdiplus)