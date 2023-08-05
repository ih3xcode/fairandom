add_executable(fairandom-cli main.c verify.c generate.c cli.h)

target_link_libraries(fairandom-cli PRIVATE fairandom)
target_link_libraries(fairandom-cli PRIVATE xcmdparser)

target_compile_options(fairandom-cli PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-missing-field-initializers)
target_link_options(fairandom-cli PRIVATE -Wall -Wextra -Wpedantic -Werror)

install(TARGETS fairandom-cli DESTINATION bin)
