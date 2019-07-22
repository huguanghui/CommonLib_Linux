cmake_minimum_required(VERSION 2.6)
#add_definitions(-std=c++11)

project(zw_ai_stick C CXX)

set(zw_ai_stick_major 1)
set(zw_ai_stick_minor 0)

set(CXX_FLAGS 
-Dhi3521d
-DHICHIP=0x3521d100
-DHI_RELEASE
-DHI_XXXX
-ffunction-sections
-fdata-sections
-fPIC
-D_LINUX
-DHDT_LINUX
-Wall
# -O3
# -DNDEBUG
)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# HVR的交叉编译器
set(CMAKE_C_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-gcc")
set(CMAKE_CXX_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-g++")

# 智能棒上的交叉编译器
# set(CMAKE_C_COMPILER "/opt/hisi-linux/x86-arm/arm-himix200-linux/bin/arm-himix200-linux-gcc")
# set(CMAKE_CXX_COMPILER "/opt/hisi-linux/x86-arm/arm-himix200-linux/bin/arm-himix200-linux-g++")


message(STATUS "CMAKE_CXX_FLAGS = " ${CMAKE_CXX_FLAGS})
message(STATUS "PROJECT_SOURCE_DIR = " ${PROJECT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/include/usbpipe)
include_directories(/root/SVN/04_HVR_AiStick/DVR_SDK/include)
include_directories(/root/SVN/04_HVR_AiStick/DVR_SDK/FIU)
include_directories(/root/SVN/04_HVR_AiStick/DVR_SDK/AVL)
include_directories(/root/SVN/04_HVR_AiStick/DVR_SDK/PUB)
include_directories(/root/SVN/04_HVR_AiStick/DVR_SDK/FACE_RECO_AS)
include_directories(/root/SVN/04_HVR_AiStick/DVR_SDK)

add_subdirectory(src)
add_subdirectory(test)
