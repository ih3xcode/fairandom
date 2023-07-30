cmake_minimum_required(VERSION 3.10)

# tests names
set(TESTS
    hex.fromBytes
    hex.toBytes
    gen.preDef
    gen.random
    proof.preDef
    proof.verify
    fairandom.fullTest)

foreach(TEST ${TESTS})
    string(REPLACE "." "_" TEST_SOURCE ${TEST})
    add_executable(${TEST} ${TEST_SOURCE}.c)
    target_link_libraries(${TEST} PRIVATE ${PROJECT_NAME})
    add_test(NAME ${TEST} COMMAND ${TEST})
endforeach()
