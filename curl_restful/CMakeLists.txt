set(TARGET01 curl_restful)
set(TARGET01_LIST restful_main.c)

# libz.a
# link_directories(${PROJECT_SOURCE_DIR}/lib/z/lib_static)
set(ZLIB_DIR ${PROJECT_SOURCE_DIR}/libs/zlib)
message(STATUS "ZLIB_DIR= " ${ZLIB_DIR})
include_directories(${ZLIB_DIR}/include)
find_library(ZLIB_LIBRARY z PATHS ${ZLIB_DIR}/lib)
message(STATUS "ZLIB_LIBRARY= " ${ZLIB_LIBRARY})

# libcrypto.so
#set(CRYPTO_DIR ${PROJECT_SOURCE_DIR}/libs/openssl)
#message(STATUS "CRYPTO_DIR= " ${CRYPTO_DIR})
#include_directories(${CRYPTO_DIR}/include)
#find_library(CRYPTO_LIBRARY crypto PATHS ${CRYPTO_DIR}/lib)
#message(STATUS "CRYPTO_LIBRARY= " ${CRYPTO_LIBRARY})

# libssl.so
#set(SSL_DIR ${PROJECT_SOURCE_DIR}/libs/openssl)
#message(STATUS "SSL_DIR= " ${SSL_DIR})
#include_directories(${SSL_DIR}/include)
#find_library(SSL_LIBRARY ssl PATHS ${SSL_DIR}/lib)
#message(STATUS "SSL_LIBRARY= " ${SSL_LIBRARY})

# libssh2.so
#set(SSH2_DIR ${PROJECT_SOURCE_DIR}/libs/ssh2)
#message(STATUS "SSH2_DIR= " ${SSH2_DIR})
#include_directories(${SSH2_DIR}/include)
#find_library(SSH2_LIBRARY ssh2 PATHS ${SSH2_DIR}/lib)
#message(STATUS "SSH2_LIBRARY= " ${SSH2_LIBRARY})

# libcurl.a
set(CURL_DIR ${PROJECT_SOURCE_DIR}/libs/curl)
message(STATUS "CURL_DIR= " ${CURL_DIR})
include_directories(${CURL_DIR}/include)
find_library(CURL_LIBRARY curl PATHS ${CURL_DIR}/lib)
message(STATUS "CURL_LIBRARY= " ${CURL_LIBRARY})

set(CMAKE_C_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-gcc")
set(CMAKE_CXX_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-g++")

add_executable(${TARGET01} ${TARGET01_LIST})

target_link_libraries(${TARGET01} ${CURL_LIBRARY})
#target_link_libraries(${TARGET01} ${SSH2_LIBRARY})
#target_link_libraries(${TARGET01} ${SSL_LIBRARY})
#target_link_libraries(${TARGET01} ${CRYPTO_LIBRARY})
target_link_libraries(${TARGET01} ${ZLIB_LIBRARY})
