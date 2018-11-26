cmake_minimum_required(VERSION 2.8)

project(network)
    
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(keyboard_select keyboard/keyboard_select.cc)

add_executable(simple_server simple/simple_server.cc)
add_executable(simple_client simple/simple_client.cc)
