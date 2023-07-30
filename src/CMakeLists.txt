# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${fairandom_SOURCE_DIR}/include/fairandom/*.h")
file(GLOB SOURCE_LIST CONFIGURE_DEPENDS "${fairandom_SOURCE_DIR}/src/*.c")

# Make an automatic library - will be static or dynamic based on user setting
add_library(fairandom SHARED fairandom.c hash.c random.c proof.c hex.c ${HEADER_LIST})

# link with openssl
find_package(OpenSSL REQUIRED)
target_link_libraries(fairandom OpenSSL::Crypto)

# link with math library
target_link_libraries(fairandom m)


# We need this directory, and users of our library will need it too
target_include_directories(fairandom PUBLIC ../include)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
