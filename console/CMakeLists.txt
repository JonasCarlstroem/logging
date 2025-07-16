cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_library(logging_console INTERFACE)
add_executable(external_console "external_console.cpp")

add_dependencies(logging_console external_console)

target_compile_features(external_console INTERFACE cxx_std_17)