cmake_minimum_required(VERSION 3.29)
project(LiveStreamClient)

set(CMAKE_CXX_STANDARD 20)

add_executable(LiveStreamClient main.cpp)

target_link_libraries(LiveStreamClient PRIVATE gdiplus)
