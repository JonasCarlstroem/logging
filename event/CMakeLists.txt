cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_library(logging_event INTERFACE)
CREATE_LIB(logging_event INTERFACE)

target_include_directories(logging_event INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include/logging/event>
)

INSTALL_LIB(logging_event True logging/event)