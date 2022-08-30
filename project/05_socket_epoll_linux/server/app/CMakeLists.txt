# 生成可执行文件server
add_executable(server)

# 添加源文件
target_sources(server
PRIVATE
    main.cpp
)

# 链接时需要的库文件
target_link_libraries(server
PRIVATE
    libs::server_lib
)

# 指定C++11标准
target_compile_features(server
PRIVATE
    cxx_std_11
)

# 设置编译选项
target_compile_options(server
PRIVATE
    -Wall -Wextra
)