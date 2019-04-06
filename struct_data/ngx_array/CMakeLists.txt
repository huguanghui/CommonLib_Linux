set(TARGET ngxarry)
set(SRC_LIST array_main.c ngx_array.c)

include_directories(${PROJECT_SOURCE_DIR}/data_struct/ngx_pool)
add_executable(${TARGET} ${SRC_LIST})
target_link_libraries(${TARGET} poolbase)