add_executable(client)

target_sources(client
PRIVATE
    main.cpp
)

target_link_libraries(client
PRIVATE
    libs::client_lib
)

target_compile_features(client
PRIVATE
    cxx_std_11
)