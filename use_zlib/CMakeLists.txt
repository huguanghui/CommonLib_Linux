set(TARGET z_demo)
# libz.a
# link_directories(${PROJECT_SOURCE_DIR}/lib/z/lib_static)
set(ZLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/zlib)
message(STATUS "ZWHOST_DIR= " ${ZLIB_DIR})
include_directories(${ZLIB_DIR}/include)
find_library(ZLIB_LIBRARY z PATHS ${ZLIB_DIR}/lib_static)
message(STATUS "ZWHOSTS_LIBRARY= " ${ZLIB_LIBRARY})

add_executable(${TARGET} src/main.c)
#target_link_libraries(demo pthread)
target_link_libraries(${TARGET} ${ZLIB_LIBRARY})
