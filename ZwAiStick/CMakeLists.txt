cmake_minimum_required(VERSION 2.6)
#add_definitions(-std=c++11)

project(zw_ai_stick C CXX)

set(zw_ai_stick_major 1)
set(zw_ai_stick_minor 0)

set(CXX_FLAGS 
-g
-Wall
-Werror
-rdynamic
)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#set(CMAKE_C_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-gcc")
#set(CMAKE_CXX_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-g++")

message(STATUS "CMAKE_CXX_FLAGS = " ${CMAKE_CXX_FLAGS})
message(STATUS "PROJECT_SOURCE_DIR = " ${PROJECT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/src/include)

include_directories(${PROJECT_SOURCE_DIR}/include/usbpipe)

add_subdirectory(src)
add_subdirectory(test)
