# 生成可执行文件client
add_executable(client)

# 添加源文件
target_sources(client
PRIVATE
    main.cpp
)

# 链接时需要的库文件
target_link_libraries(client
PRIVATE
    libs::client_lib
)

# 指定C++11标准
target_compile_features(client
PRIVATE
    cxx_std_11
)

# 设置编译选项
target_compile_options(client
PRIVATE
    -Wall -Wextra
)