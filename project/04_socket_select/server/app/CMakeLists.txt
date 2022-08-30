add_executable(server)

target_sources(server
PRIVATE
    main.cpp
)

target_link_libraries(server
PRIVATE
    libs::server_lib
)

target_compile_features(server
PRIVATE
    cxx_std_11
)

target_compile_options(server
PRIVATE
    -Wall -Wextra
)