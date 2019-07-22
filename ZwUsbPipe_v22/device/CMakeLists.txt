set(TARGET_NAME test_device)
set(TARGET_LIST main.cpp)

# 智能棒上的交叉编译器
set(CMAKE_C_COMPILER "/opt/hisi-linux/x86-arm/arm-himix200-linux/bin/arm-himix200-linux-gcc")
set(CMAKE_CXX_COMPILER "/opt/hisi-linux/x86-arm/arm-himix200-linux/bin/arm-himix200-linux-g++")

include_directories(${PROJECT_SOURCE_DIR}/libpipe_device/include)

add_executable(${TARGET_NAME} ${TARGET_LIST})
target_link_libraries(${TARGET_NAME} usbpipe_dev)
target_link_libraries(${TARGET_NAME} pthread)

install(TARGETS test_device DESTINATION /work)
