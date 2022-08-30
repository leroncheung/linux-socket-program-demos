cmake_minimum_required(VERSION 3.10)

project(clent_project)

add_executable(client)

target_sources(client PRIVATE
    client.cpp
)