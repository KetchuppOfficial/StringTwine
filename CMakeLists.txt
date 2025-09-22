cmake_minimum_required(VERSION 3.20)

project(twine LANGUAGES CXX)

option(BUILD_TESTS "build tests")
option(BUILD_BENCHMARK "build benchmark")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} INTERFACE)
add_library(cool::twine ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE include)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_23)
target_compile_options(${PROJECT_NAME}
INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<CXX_COMPILER_ID:GNU,Clang>: -Wall -Wextra -pedantic>
)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if (BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif()
