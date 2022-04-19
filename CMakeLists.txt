cmake_minimum_required(VERSION 3.18)
project(rtc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

include(cmake/conan.cmake)

option(RUN_TESTS "Build the tests" ON)
if(RUN_TESTS)
    enable_testing()
    find_package(GTest)
endif()

add_subdirectory(app)