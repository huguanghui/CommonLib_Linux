set(CMAKE_C_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-gcc")
set(CMAKE_CXX_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-g++")

add_executable(check_endian main.c)
#target_link_libraries(demo poolbase)

# 指定安装路径
install(TARGETS check_endian DESTINATION /work)