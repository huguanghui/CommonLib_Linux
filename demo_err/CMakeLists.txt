#set(CMAKE_C_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-gcc")
#set(CMAKE_CXX_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-g++")
set(TARGET_NAME err_demo)

add_executable(${TARGET_NAME} main.c)

# 指定安装路径
install(TARGETS ${TARGET_NAME} DESTINATION bin)