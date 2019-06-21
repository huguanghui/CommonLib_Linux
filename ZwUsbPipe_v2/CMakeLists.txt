cmake_minimum_required(VERSION 2.6)
# add_definitions(-std=c++11)

project(usbpipe C CXX)

set(usbpipe 1)
set(usbpipe 0)

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

message(STATUS "CMAKE_CXX_FLAGS = " ${CMAKE_CXX_FLAGS})
message(STATUS "PROJECT_SOURCE_DIR = " ${PROJECT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/libpipe/include)

add_subdirectory(host)
add_subdirectory(device)
add_subdirectory(libpipe)
add_subdirectory(sender)
add_subdirectory(test_list)
