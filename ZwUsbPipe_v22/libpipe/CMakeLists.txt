set(LIBPIPE_SRCS
    zw_usbpipe_util.c
    zw_usbpipe_base.c
    zw_usbpipe_uptp.c
    zw_usbpipe_uptp_api.c
)

# HVR的交叉编译器
set(CMAKE_C_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-gcc")
set(CMAKE_CXX_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-g++")

include_directories(${PROJECT_SOURCE_DIR}/libpipe/include)
include_directories(${PROJECT_SOURCE_DIR}/../mpp/hi3521d/mpp/include)
link_directories(${PROJECT_SOURCE_DIR}/../mpp/hi3521d/mpp/lib)

add_library(usbpipe ${LIBPIPE_SRCS})

install(TARGETS usbpipe DESTINATION lib)

file(GLOB HEADERS "*/*.h")
install(FILES ${HEADERS} DESTINATION include/usbpipe)
