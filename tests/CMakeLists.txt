cmake_minimum_required(VERSION 3.0.0)

if(BUILD_TESTING)
    message(STATUS "Building Lazarus tests")
    file(GLOB TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/tests/*.cpp
    ${PROJECT_SOURCE_DIR}/tests/catch/catch.hpp)

    add_executable(lazarus_test ${TEST_SOURCES})
    target_link_libraries(lazarus_test ${LIBRARY_NAME})

    target_include_directories(lazarus_test PRIVATE ${CMAKE_SOURCE_DIR}/src)

    enable_testing()
    add_test(NAME test
            COMMAND sh -c
            "$<TARGET_FILE:lazarus_test>"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endif()