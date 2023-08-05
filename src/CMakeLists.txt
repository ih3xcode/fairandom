file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${fairandom_SOURCE_DIR}/include/fairandom/*.h")
file(GLOB SOURCE_LIST CONFIGURE_DEPENDS "${fairandom_SOURCE_DIR}/src/*.c")

add_library(fairandom SHARED fairandom.c hash.c random.c proof.c hex.c ${HEADER_LIST})

# link with openssl
find_package(OpenSSL REQUIRED)
target_link_libraries(fairandom OpenSSL::Crypto)

# link with math library
target_link_libraries(fairandom m)


# We need this directory, and users of our library will need it too
target_include_directories(fairandom PUBLIC ../include)

target_compile_options(fairandom PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-missing-field-initializers)
target_link_options(fairandom PRIVATE -Wall -Wextra -Wpedantic -Werror)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})

# install
install(TARGETS fairandom DESTINATION lib)
install(FILES ${HEADER_LIST} DESTINATION include/fairandom)
