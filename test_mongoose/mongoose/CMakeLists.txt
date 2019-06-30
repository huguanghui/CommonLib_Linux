set(TARGET_NAME "api_server")

FILE(GLOB SRC_SOURCE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
set(SRC_LIST ${SRC_SOURCE_LIST})

message(STATUS "CMAKE_CURRENT_LIST_FILE= " ${CMAKE_CURRENT_LIST_FILE})
message(STATUS "CMAKE_CURRENT_SOURCE_DIR= " ${CMAKE_CURRENT_SOURCE_DIR})
# set(CMAKE_C_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-gcc")
# set(CMAKE_CXX_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-g++")


include_directories(/usr/include/libxml2)

add_executable(${TARGET_NAME} ${SRC_LIST})
target_link_libraries(${TARGET_NAME} xml2)
target_link_libraries(${TARGET_NAME} dl)
target_link_libraries(${TARGET_NAME} m)
target_link_libraries(${TARGET_NAME} pthread)
