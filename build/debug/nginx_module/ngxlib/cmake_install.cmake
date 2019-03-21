# Install script for directory: /root/GIT/CommonLib_Linux/nginx_module/ngxlib

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/root/GIT/CommonLib_Linux/build/debug-install")
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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/root/GIT/CommonLib_Linux/build/debug/lib/libngx_common.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ngx_common" TYPE FILE FILES
    "/root/GIT/CommonLib_Linux/nginx_module/ngxlib/ngx_log.h"
    "/root/GIT/CommonLib_Linux/nginx_module/ngxlib/ngx_string.h"
    "/root/GIT/CommonLib_Linux/nginx_module/ngxlib/ngx_errno.h"
    "/root/GIT/CommonLib_Linux/nginx_module/ngxlib/ngx_core.h"
    "/root/GIT/CommonLib_Linux/nginx_module/ngxlib/ngx_rbtree.h"
    "/root/GIT/CommonLib_Linux/nginx_module/ngxlib/ngx_config.h"
    "/root/GIT/CommonLib_Linux/nginx_module/ngxlib/ngx_palloc.h"
    "/root/GIT/CommonLib_Linux/nginx_module/ngxlib/ngx_linux_config.h"
    "/root/GIT/CommonLib_Linux/nginx_module/ngxlib/ngx_auto_config.h"
    "/root/GIT/CommonLib_Linux/nginx_module/ngxlib/ngx_list.h"
    "/root/GIT/CommonLib_Linux/nginx_module/ngxlib/ngx_alloc.h"
    "/root/GIT/CommonLib_Linux/nginx_module/ngxlib/ngx_hash.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/root/GIT/CommonLib_Linux/build/debug/nginx_module/ngxlib/test/cmake_install.cmake")

endif()

