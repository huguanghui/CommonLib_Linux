set(TARGET test_restful)
set(SRC_LIST restful_main.c)

# libcurl.a
set(CURL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/curl)
message(STATUS "CURL_DIR= " ${CURL_DIR})
include_directories(${CURL_DIR}/include)
find_library(CURL_LIBARARY curl PATHS ${CURL_DIR}/lib_static)
message(STATUS "CURL_LIBARARY= " ${CURL_LIBARARY})

add_executable(${TARGET} ${SRC_LIST})
#link_libraries(${TARGET} ${CURL_LIBARARY})
# -lssl -lcrypto -lssl -lcrypto -lz
target_link_libraries(${TARGET} ${CURL_LIBARARY})
target_link_libraries(${TARGET} ssl)
target_link_libraries(${TARGET} crypto)
target_link_libraries(${TARGET} z)
