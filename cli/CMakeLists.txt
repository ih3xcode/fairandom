add_executable(fairandom-cli main.c)

target_link_libraries(fairandom-cli PRIVATE fairandom)

target_compile_options(fairandom-cli PRIVATE -pg)