add_executable(fairandom-cli main.c verify.c generate.c cli.h)

target_link_libraries(fairandom-cli PRIVATE fairandom)
target_link_libraries(fairandom-cli PRIVATE xcmdparser)

target_compile_options(fairandom-cli PRIVATE -g)
target_link_options(fairandom-cli PRIVATE -g)
#target_compile_options(fairandom-cli PRIVATE -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all)
#target_link_options(fairandom-cli PRIVATE -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all)