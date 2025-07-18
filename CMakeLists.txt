cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(logger)
add_subdirectory(console)
add_subdirectory(event)
add_subdirectory(file)

add_library(logging INTERFACE)

target_compile_features(logging INTERFACE cxx_std_17)

target_include_directories(logging
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(logging
	INTERFACE logging_base
	INTERFACE logging_file
	INTERFACE logging_console
)

set(LIB_TARGETS logging PARENT_SCOPE)