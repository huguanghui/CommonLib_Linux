cmake_minimum_required(VERSION 2.6)
# add_definitions(-std=c++11)

project(usbpipe C CXX)

set(usbpipe 1)
set(usbpipe 0)

set(C_FLAGS
-fPIC
-Wall
# -O3
# -DNDEBUG
)

string(REPLACE ";" " " CMAKE_C_FLAGS "${C_FLAGS}")

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

message(STATUS "CMAKE_CXX_FLAGS = " ${CMAKE_CXX_FLAGS})
message(STATUS "C_FLAGS = " ${C_FLAGS})
message(STATUS "CMAKE_C_FLAGS = " ${CMAKE_C_FLAGS})
message(STATUS "PROJECT_SOURCE_DIR = " ${PROJECT_SOURCE_DIR})

# add_subdirectory(host)
add_subdirectory(device)
add_subdirectory(libpipe)
add_subdirectory(libpipe_device)
# add_subdirectory(sender)
