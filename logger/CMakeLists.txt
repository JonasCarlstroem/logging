cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_library(logging_base INTERFACE)
CREATE_LIB(logging_base INTERFACE)

target_include_directories(logging_base INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include/logging/logger>
)

INSTALL_LIB(logging_base True logging/logger)