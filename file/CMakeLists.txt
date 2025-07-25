cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

#add_library(logging_file INTERFACE)
CREATE_LIB(logging_file INTERFACE)

target_include_directories(logging_file INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include/logging/file>
)

INSTALL_LIB(logging_file True logging/file)