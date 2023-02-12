set(TARGET_NAME err_demo)

add_executable(${TARGET_NAME} main.c)

# 指定安装路径
install(TARGETS ${TARGET_NAME} DESTINATION bin)
