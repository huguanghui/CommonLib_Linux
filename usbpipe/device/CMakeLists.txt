set(TARGET_NAME test_device)
set(TARGET_LIST main.c)

add_executable(${TARGET_NAME} ${TARGET_LIST})
target_link_libraries(${TARGET_NAME} usbpipe)
target_link_libraries(${TARGET_NAME} pthread)
