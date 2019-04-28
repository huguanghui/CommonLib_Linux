set(TAGET_SERVER unixserver)
set(TAGET_SERVER_LIST unix_server.c)

add_executable(${TAGET_SERVER} ${TAGET_SERVER_LIST})

set(TAGET_CLIENT unixclient)
set(TAGET_CLIENT_LIST unix_client.c)

add_executable(${TAGET_CLIENT} ${TAGET_CLIENT_LIST})