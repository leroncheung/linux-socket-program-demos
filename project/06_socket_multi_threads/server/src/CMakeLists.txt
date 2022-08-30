# 默认生成静态库，可以改为SHARED生成动态库
add_library(base_util_lib)

# 与外部库文件命名空间引用方式保持一致，增加别名
add_library(libs::base_util_lib ALIAS base_util_lib)

# 生成库的源文件
target_sources(base_util_lib
PRIVATE
    base_util.cpp
)

# 生成库文件需要的头文件目录。注意：其他使用本库的使用者也会引用该目录，所以应用范围必须为PUBLIC
target_include_directories(base_util_lib
PUBLIC
    ../include/
)

# 指定C++11标准
target_compile_features(base_util_lib
PRIVATE
    cxx_std_11
)

# 设置编译选项
target_compile_options(base_util_lib
PRIVATE
    -Wall -Wextra
)





add_library(server_lib)

add_library(libs::server_lib ALIAS server_lib)

target_sources(server_lib
PRIVATE
    server.cpp
)

target_include_directories(server_lib
PUBLIC
    ../include/
)

target_link_libraries(server_lib
PRIVATE
    libs::base_util_lib
)

target_compile_features(server_lib
PRIVATE
    cxx_std_11
)

target_compile_options(server_lib
PRIVATE
    -Wall -Wextra
)