cmake_minimum_required(VERSION 3.20)
project(tp3_taller1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(tp3_taller1
        blocking_queue.h
        client_main.cpp
        server_main.cpp
        common_socket.cpp
        common_socket.h common_message.cpp common_message.h common_protocol.cpp common_protocol.h)
