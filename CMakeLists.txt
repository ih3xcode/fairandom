cmake_minimum_required(VERSION 3.22)

project(
    fairandom
    VERSION 1.0.0
    DESCRIPTION "A provable random generator"
    LANGUAGES C
)

#add_compile_options(-fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -g)
#add_link_options(-fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -g)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # Testing only available if this is the main app
  # Note this needs to be done in the main CMakeLists
  # since it calls enable_testing, which must be in the
  # main CMakeLists.
  include(CTest)

  include(InstallRequiredSystemLibraries)
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
  set(CPACK_PACKAGE_VERSION_MAJOR "${fairandom_VERSION_MAJOR}")
  set(CPACK_PACKAGE_VERSION_MINOR "${fairandom_VERSION_MINOR}")
  set(CPACK_PACKAGE_VERSION_PATCH "${fairandom_VERSION_PATCH}")
  set(CPACK_PACKAGE_VENDOR "h3xcode")
  set(CPACK_SOURCE_GENERATOR "TGZ")
  include(CPack)

  # Docs only available if this is the main app
  #find_package(Doxygen)
  if(Doxygen_FOUND)
    add_subdirectory(docs)
  else()
    message(STATUS "Doxygen not found, not building docs")
  endif()
endif()

add_subdirectory(src)
add_subdirectory(xcmdparser)
add_subdirectory(cli)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
  add_subdirectory(tests)
endif()
