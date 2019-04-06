set(TARGET ngxlist)
set(SRC_LIST list_main.c ngx_list.c)

add_executable(${TARGET} ${SRC_LIST})