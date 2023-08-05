add_executable(fairandom-cli main.c verify.c generate.c cli.h)

target_link_libraries(fairandom-cli PRIVATE fairandom)
target_link_libraries(fairandom-cli PRIVATE xcmdparser)

target_compile_options(fairandom-cli PRIVATE -Wall -Wextra -Wpedantic -Werror -Wshadow -Wno-missing-field-initializers -D_FORTIFY_SOURCE=2)

set_target_properties(fairandom-cli PROPERTIES C_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

install(TARGETS fairandom-cli DESTINATION bin)
