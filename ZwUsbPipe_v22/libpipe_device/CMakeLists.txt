set(LIBPIPE_SRCS
    zw_usbpipe_util.c
    zw_usbpipe_base.c
    zw_usbpipe_uptp.c
    zw_usbpipe_uptp_api.c
)

# 智能棒上的交叉编译器
set(CMAKE_C_COMPILER "/opt/hisi-linux/x86-arm/arm-himix200-linux/bin/arm-himix200-linux-gcc")
set(CMAKE_CXX_COMPILER "/opt/hisi-linux/x86-arm/arm-himix200-linux/bin/arm-himix200-linux-g++")

include_directories(${PROJECT_SOURCE_DIR}/libpipe_device/include)

add_library(usbpipe_dev SHARED ${LIBPIPE_SRCS})
#add_library(usbpipe_dev ${LIBPIPE_SRCS})

# install(TARGETS usbpipe DESTINATION lib)

# file(GLOB HEADERS "*/*.h")
# install(FILES ${HEADERS} DESTINATION include/usbpipe)

# add_subdirectory(test)
