cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

#add_library(logging_console INTERFACE)
CREATE_LIB(logging_console INTERFACE)
add_executable(external_console "external_console.cpp")

add_dependencies(logging_console external_console)

target_compile_features(external_console INTERFACE cxx_std_17)

target_include_directories(logging_console INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include/logging/console>
)

#set(LIB_TARGETS logging_console PARENT_SCOPE)

INSTALL_LIB(logging_console True logging/console)