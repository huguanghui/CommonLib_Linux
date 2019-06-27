set(TARGET_NAME mongo_test)
set(TARGET_SRC main.c mg_util.c)

add_executable(${TARGET_NAME} ${TARGET_SRC})
