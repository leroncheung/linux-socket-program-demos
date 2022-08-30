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

target_compile_features(server_lib
PRIVATE
    cxx_std_11
)

target_compile_options(server_lib
PRIVATE
    -Wall -Wextra
)