# CMake最低版本要求：3.10
cmake_minimum_required(VERSION 3.10)

# 指定project：使用语言, 版本号, 项目描述
project(client_project
        VERSION 0.1.5
)

# 添加子目录
add_subdirectory(src)
add_subdirectory(app)