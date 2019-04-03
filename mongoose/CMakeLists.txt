set(TARGET_NAME "api_server")

FILE(GLOB SRC_SOURCE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
set(SRC_LIST ${SRC_SOURCE_LIST})

message(STATUS "CMAKE_CURRENT_LIST_FILE= " ${CMAKE_CURRENT_LIST_FILE})
message(STATUS "CMAKE_CURRENT_SOURCE_DIR= " ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${TARGET_NAME} ${SRC_LIST})
target_link_libraries(${TARGET_NAME} dl)
target_link_libraries(${TARGET_NAME} m)
target_link_libraries(${TARGET_NAME} pthread)