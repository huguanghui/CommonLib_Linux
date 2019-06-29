set(TARGET_NAME xml2_test01)
set(TARGET_SRC main.c)

add_executable(${TARGET_NAME} ${TARGET_SRC})
target_link_libraries(${TARGET_NAME} xml2)
