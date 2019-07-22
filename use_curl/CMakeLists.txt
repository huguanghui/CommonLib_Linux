# libcurl.a
set(CURL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/curl)
message(STATUS "CURL_DIR= " ${CURL_DIR})
include_directories(${CURL_DIR}/include)
find_library(CURL_LIBARARY curl PATHS ${CURL_DIR}/lib_static)
message(STATUS "CURL_LIBARARY= " ${CURL_LIBARARY})

set(TARGET test_curl)
set(SRC_LIST src/main.c)

add_executable(${TARGET} ${SRC_LIST})
#link_libraries(${TARGET} ${CURL_LIBARARY})
# -lssl -lcrypto -lssl -lcrypto -lz
target_link_libraries(${TARGET} ${CURL_LIBARARY})
# target_link_libraries(${TARGET} ssl)
# target_link_libraries(${TARGET} crypto)
# target_link_libraries(${TARGET} z)
target_link_libraries(${TARGET} pthread)

set(TARGET_FTPGET test_ftpget)
set(FTPGET_LIST src/ftp_get.c)

add_executable(${TARGET_FTPGET} ${FTPGET_LIST})
#link_libraries(${TARGET} ${CURL_LIBARARY})
# -lssl -lcrypto -lssl -lcrypto -lz
target_link_libraries(${TARGET_FTPGET} ${CURL_LIBARARY})
# target_link_libraries(${TARGET_FTPGET} ssl)
# target_link_libraries(${TARGET_FTPGET} crypto)
# target_link_libraries(${TARGET_FTPGET} z)
target_link_libraries(${TARGET_FTPGET} pthread)

set(TARGET_CURL_DGB curl_dbg)
set(CURL_DGB_LIST src/curl_dbg.c)

add_executable(${TARGET_CURL_DGB} ${CURL_DGB_LIST})
#link_libraries(${TARGET} ${CURL_LIBARARY})
# -lssl -lcrypto -lssl -lcrypto -lz
target_link_libraries(${TARGET_CURL_DGB} ${CURL_LIBARARY})
# target_link_libraries(${TARGET_FTPGET} ssl)
# target_link_libraries(${TARGET_FTPGET} crypto)
# target_link_libraries(${TARGET_FTPGET} z)
target_link_libraries(${TARGET_CURL_DGB} pthread)

set(TARGET_CURL_FORM curl_form)
set(CURL_FORM_LIST src/curl_post_form.c)

add_executable(${TARGET_CURL_FORM} ${CURL_FORM_LIST})
#link_libraries(${TARGET} ${CURL_LIBARARY})
# -lssl -lcrypto -lssl -lcrypto -lz
target_link_libraries(${TARGET_CURL_FORM} ${CURL_LIBARARY})
# target_link_libraries(${TARGET_FTPGET} ssl)
# target_link_libraries(${TARGET_FTPGET} crypto)
# target_link_libraries(${TARGET_FTPGET} z)
target_link_libraries(${TARGET_CURL_FORM} pthread)