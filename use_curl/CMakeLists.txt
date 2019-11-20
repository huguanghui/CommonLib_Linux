<<<<<<< HEAD
=======
set(TARGET test_curl)
set(SRC_LIST src/main.c)

>>>>>>> f8201bb7556cfc97fd7b08f5331f65491d582d89
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

set(TARGET_FTPGET test_ftpget)
set(FTPGET_LIST src/ftp_get.c)

add_executable(${TARGET_FTPGET} ${FTPGET_LIST})
#link_libraries(${TARGET} ${CURL_LIBARARY})
# -lssl -lcrypto -lssl -lcrypto -lz
target_link_libraries(${TARGET_FTPGET} ${CURL_LIBARARY})
<<<<<<< HEAD
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
=======
target_link_libraries(${TARGET_FTPGET} ssl)
target_link_libraries(${TARGET_FTPGET} crypto)
target_link_libraries(${TARGET_FTPGET} z)

set(TARGET_SIMPLE_GET crul_test_get)
set(SIMPLE_GET_LIST src/simple_get.c)

add_executable(${TARGET_SIMPLE_GET} ${SIMPLE_GET_LIST})
#link_libraries(${TARGET} ${CURL_LIBARARY})
# -lssl -lcrypto -lssl -lcrypto -lz
target_link_libraries(${TARGET_SIMPLE_GET} ${CURL_LIBARARY})
target_link_libraries(${TARGET_SIMPLE_GET} ssl)
target_link_libraries(${TARGET_SIMPLE_GET} crypto)
target_link_libraries(${TARGET_SIMPLE_GET} z)

set(TARGET_SIMPLE_DBG crul_test_dbg)
set(SIMPLE_DBG_LIST src/simple_dbg.c)

add_executable(${TARGET_SIMPLE_DBG} ${SIMPLE_DBG_LIST})
#link_libraries(${TARGET} ${CURL_LIBARARY})
# -lssl -lcrypto -lssl -lcrypto -lz
target_link_libraries(${TARGET_SIMPLE_DBG} ${CURL_LIBARARY})
target_link_libraries(${TARGET_SIMPLE_DBG} ssl)
target_link_libraries(${TARGET_SIMPLE_DBG} crypto)
target_link_libraries(${TARGET_SIMPLE_DBG} z)
>>>>>>> f8201bb7556cfc97fd7b08f5331f65491d582d89
