set(TAGET_PING net_ping)
set(TAGET_PING_LIST ping_main.c ping_api.c)

add_executable(${TAGET_PING} ${TAGET_PING_LIST})
