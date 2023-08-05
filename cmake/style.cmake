include(cmake/CPM.cmake)
CPMAddPackage(
    NAME Format.cmake
    VERSION 1.7.3
    GITHUB_REPOSITORY TheLartians/Format.cmake
    OPTIONS
        # set to yes skip cmake formatting
        "FORMAT_SKIP_CMAKE YES"
        # path to exclude (optional, supports regular expressions)
        "CMAKE_FORMAT_EXCLUDE cmake/CPM.cmake"
)
