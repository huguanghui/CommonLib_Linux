#set(CMAKE_C_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-gcc")
#set(CMAKE_CXX_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-g++")

include_directories(${HOME_DIR}/include/mongo)

add_executable(demo demo.cpp)
#target_link_libraries(demo poolbase)
target_link_libraries(demo mongo)

# 指定安装路径
# install(TARGETS demo DESTINATION /work)
install(TARGETS demo DESTINATION bin)