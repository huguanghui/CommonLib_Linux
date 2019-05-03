set(TARGET fcgi_demo)
# libfcgi.a
# link_directories(${PROJECT_SOURCE_DIR}/lib/z/lib_static)
set(FCGI_DIR ${PROJECT_SOURCE_DIR}/libs/fcgi)
message(STATUS "FCGI_DIR= " ${FCGI_DIR})
include_directories(${FCGI_DIR}/include)
find_library(FCGI_LIBRARY fcgi PATHS ${FCGI_DIR}/lib_static)
message(STATUS "FCGI_LIBRARY= " ${FCGI_LIBRARY})

add_executable(${TARGET} test.c)
#target_link_libraries(demo pthread)
target_link_libraries(${TARGET} ${FCGI_LIBRARY})
