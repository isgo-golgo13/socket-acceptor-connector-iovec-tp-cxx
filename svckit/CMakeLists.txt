cmake_minimum_required(VERSION 3.16)

project(svckit)

add_library(svckit SHARED
    socket-acceptor.cpp
    socket-connector.cpp
    socket-addr.cpp
    socket.cpp
    thread.cpp
    thread-pool.cpp
)

target_include_directories(svckit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
