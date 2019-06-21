set(TARGET_NAME pipe_sender)
set(TARGET_LIST main.c)

add_executable(${TARGET_NAME} ${TARGET_LIST})
target_link_libraries(${TARGET_NAME} usbpipe)
target_link_libraries(${TARGET_NAME} pthread)
