# Install script for directory: /home/hgh/GIT/CommonLib_linux/nginx_module/ngxlib

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/hgh/GIT/CommonLib_linux/build/debug-install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/hgh/GIT/CommonLib_linux/build/debug/lib/libngx_common.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ngx_common" TYPE FILE FILES
    "/home/hgh/GIT/CommonLib_linux/nginx_module/ngxlib/ngx_alloc.h"
    "/home/hgh/GIT/CommonLib_linux/nginx_module/ngxlib/ngx_auto_config.h"
    "/home/hgh/GIT/CommonLib_linux/nginx_module/ngxlib/ngx_config.h"
    "/home/hgh/GIT/CommonLib_linux/nginx_module/ngxlib/ngx_core.h"
    "/home/hgh/GIT/CommonLib_linux/nginx_module/ngxlib/ngx_errno.h"
    "/home/hgh/GIT/CommonLib_linux/nginx_module/ngxlib/ngx_hash.h"
    "/home/hgh/GIT/CommonLib_linux/nginx_module/ngxlib/ngx_linux_config.h"
    "/home/hgh/GIT/CommonLib_linux/nginx_module/ngxlib/ngx_list.h"
    "/home/hgh/GIT/CommonLib_linux/nginx_module/ngxlib/ngx_log.h"
    "/home/hgh/GIT/CommonLib_linux/nginx_module/ngxlib/ngx_palloc.h"
    "/home/hgh/GIT/CommonLib_linux/nginx_module/ngxlib/ngx_rbtree.h"
    "/home/hgh/GIT/CommonLib_linux/nginx_module/ngxlib/ngx_string.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/hgh/GIT/CommonLib_linux/build/debug/nginx_module/ngxlib/test/cmake_install.cmake")

endif()

