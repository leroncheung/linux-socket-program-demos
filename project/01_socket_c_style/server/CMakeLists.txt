cmake_minimum_required(VERSION 3.10)

project(server_project)

add_executable(server)

target_sources(server PRIVATE
    server.cpp
)