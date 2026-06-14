cmake_minimum_required(VERSION 3.20)

project(cyberpawn
    VERSION 0.1.0
    DESCRIPTION "A chess engine personal project"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CYBERPAWN_BUILD_CLI "Build the cyberpawn CLI application" ON)
option(CYBERPAWN_BUILD_PROFILER "Build the cyberpawn profiler application" ON)
option(CYBERPAWN_BUILD_TESTS "Build unit tests" ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-c++11-narrowing)
endif()

add_subdirectory(cyberpawnService/cyberpawn)

if(CYBERPAWN_BUILD_CLI)
    add_subdirectory(cyberpawnCli)
endif()

if(CYBERPAWN_BUILD_PROFILER)
    add_subdirectory(cyberpawnProfiler)
endif()

if(CYBERPAWN_BUILD_TESTS)
    enable_testing()
    add_subdirectory(cyberpawnService/cyberpawnTest)
endif()
