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

target_compile_features(client_lib
PRIVATE
    cxx_std_11
)

target_compile_options(client_lib
PRIVATE
    -Wall -Wextra
)