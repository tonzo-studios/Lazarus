cmake_minimum_required(VERSION 3.0.0)

# Use C++14
set(CMAKE_CXX_STANDARD 14)

# Enable debug symbols by default
# Must be done before project() statement
# Can also set it on the command line: -DCMAKE_BUILD_TYPE=Release)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

project(lazarus)

set(LIBRARY_NAME "lazarus")

file(GLOB SOURCES src/lazarus/*)

add_library(${LIBRARY_NAME} SHARED ${SOURCES})

# TODO: Add option to link SFML statically
# Detect and link SFML 2.5
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    # Only the highest level is needed, other dependencies (windows, system, etc)
    # are automatically resolved
    target_link_libraries(${LIBRARY_NAME} sfml-graphics)
else()
    message(FATAL_ERROR "SFML was not found.")
endif()

include_directories(src)
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lazarus/src)
