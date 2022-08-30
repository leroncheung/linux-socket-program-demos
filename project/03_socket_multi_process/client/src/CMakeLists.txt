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





add_library(client_lib)

add_library(libs::client_lib ALIAS client_lib)

target_sources(client_lib
PRIVATE
    client.cpp
)

target_include_directories(client_lib
PUBLIC
    ../include/
)

target_link_libraries(client_lib
PRIVATE
    libs::base_util_lib
)

target_compile_features(client_lib
PRIVATE
    cxx_std_11
)

target_compile_options(client_lib
PRIVATE
    -Wall -Wextra
)