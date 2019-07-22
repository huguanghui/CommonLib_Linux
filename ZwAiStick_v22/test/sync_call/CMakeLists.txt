set(TARGET_NAME test_sync)
set(TARGET_LIST main.cpp)


find_library(USBPIPE_LIBARARY usbpipe PATHS ${PROJECT_SOURCE_DIR}/lib)
message(STATUS "USBPIPE_LIBARARY= " ${USBPIPE_LIBARARY})

add_executable(${TARGET_NAME} ${TARGET_LIST})
target_link_libraries(${TARGET_NAME} zwaistck)
target_link_libraries(${TARGET_NAME} ${USBPIPE_LIBARARY})
target_link_libraries(${TARGET_NAME} libmpi.a)
target_link_libraries(${TARGET_NAME} libjpeg.a)
target_link_libraries(${TARGET_NAME} libupvqe.a)
target_link_libraries(${TARGET_NAME} libVoiceEngine.a)
target_link_libraries(${TARGET_NAME} libdnvqe.a)
target_link_libraries(${TARGET_NAME} dl)
target_link_libraries(${TARGET_NAME} m)
target_link_libraries(${TARGET_NAME} pthread)

# 指定安装路径
#install(TARGETS demo DESTINATION /work)
