set(ngxlib_SRCS
    ngx_hash.c
    ngx_string.c
    ngx_errno.c
)

add_library(ngx_common ${ngxlib_SRCS})
target_link_libraries(ngx_common pthread rt)

install(TARGETS ngx_common DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/ngx_common)

add_subdirectory(test)