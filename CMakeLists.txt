cmake_minimum_required(VERSION 2.6)
#add_definitions(-std=c++11)

project(cplusplus_common C CXX)

set(cplusplus_common_major 1)
set(cplusplus_common_minor 0)

set(CXX_FLAGS
-g
-Wall
#-Werror
-rdynamic
)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(HOME_DIR /root)

message(STATUS "CMAKE_CXX_FLAGS = " ${CMAKE_CXX_FLAGS})
message(STATUS "CMAKE_C_FLAGS = " ${CMAKE_C_FLAGS})
message(STATUS "PROJECT_SOURCE_DIR = " ${PROJECT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${HOME_DIR}/include)

link_directories(${HOME_DIR}/lib)

# add_subdirectory(live555_module)
# add_subdirectory(nginx_module)
# add_subdirectory(data_struct)
# add_subdirectory(struct_data)
# add_subdirectory(easydemo)
add_subdirectory(demo_err)
# add_subdirectory(linux_net)
#add_subdirectory(test_mongoose)
#add_subdirectory(use_libxml2)
#add_subdirectory(use_zlib)
#add_subdirectory(use_curl)
#add_subdirectory(use_fcgi)
# add_subdirectory(use_sqlite3)
# add_subdirectory(multi_pthread)
# add_subdirectory(use_gprof)
#add_subdirectory(curl_restful)
# add_subdirectory(test_alist)
#add_subdirectory(test_libxml2)
# add_subdirectory(common_utils)
#add_subdirectory(check_endian)
# add_subdirectory(lib_mongoose)
# add_subdirectory(lib_log)