cmake_minimum_required(VERSION 2.8)

project(network)
    
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(keyboard_select keyboard/keyboard_select.cc)

add_executable(simple_server simple/simple_server.cc)
add_executable(simple_client simple/simple_client.cc)

add_executable(epoll_server epoll/epoll_server.cc epoll/epoll_control.cc epoll/socket_common.cc)
add_executable(epoll_client epoll/epoll_client.cc epoll/epoll_control.cc epoll/socket_common.cc)