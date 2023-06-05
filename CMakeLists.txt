cmake_minimum_required(VERSION 3.12)
# add_definitions(-std=c++11)

project(common_linux VERSION 0.0.1)

set(CMAKE_C_STANDARD 99)
enable_language(C CXX ASM)

# 编译调试开关
set(CMAKE_VERBOSE_MAKEFILE OFF)
# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads REQUIRED)
# include_directories(${HOME_DIR}/include)

# link_directories(${HOME_DIR}/lib)

# add_subdirectory(live555_module) add_subdirectory(nginx_module)
# add_subdirectory(data_struct) add_subdirectory(struct_data)
# add_subdirectory(easydemo) add_subdirectory(demo_err) add_subdirectory(test)
# add_subdirectory(linux_net) add_subdirectory(test_mongoose)
# add_subdirectory(use_libxml2) add_subdirectory(use_zlib)
# add_subdirectory(use_curl) add_subdirectory(use_fcgi)
# add_subdirectory(use_sqlite3) add_subdirectory(multi_pthread)
# add_subdirectory(use_gprof) add_subdirectory(curl_restful)
# add_subdirectory(test_alist) add_subdirectory(test_libxml2)
# add_subdirectory(common_utils) add_subdirectory(check_endian)
# add_subdirectory(lib_mongoose) add_subdirectory(lib_log) add_subdirectory(ntp)
add_subdirectory(sys_proc)
add_subdirectory(det_sys)
add_subdirectory(tcp_cli)
add_subdirectory(lib_btree)
add_subdirectory(btree_test)
# add_subdirectory(tinyframe)
add_subdirectory(tf_demo)
