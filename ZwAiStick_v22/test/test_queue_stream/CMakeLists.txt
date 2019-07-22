set(TARGET_NAME test_stream)
set(TARGET_LIST main.cpp)

find_library(USBPIPE_LIBARARY usbpipe PATHS ${PROJECT_SOURCE_DIR}/lib)
message(STATUS "USBPIPE_LIBARARY= " ${USBPIPE_LIBARARY})

link_directories(${PROJECT_SOURCE_DIR}/../mpp/hi3521d/mpp/lib)
link_directories(/root/SVN/04_HVR_AiStick/DVR_SDK/lib_35xxD-static)

set(CMAKE_VERBOSE_MAKEFILE OFF)

add_executable(${TARGET_NAME} ${TARGET_LIST})
target_link_libraries(${TARGET_NAME} zwaistck)
target_link_libraries(${TARGET_NAME} ${USBPIPE_LIBARARY})
target_link_libraries(${TARGET_NAME} libFIU.a)
# target_link_libraries(${TARGET_NAME} libzwdes.a)
# target_link_libraries(${TARGET_NAME} libddt.a)
# target_link_libraries(${TARGET_NAME} libhieserver.a)
target_link_libraries(${TARGET_NAME} libhyprivate.a)
# target_link_libraries(${TARGET_NAME} libvideonetclient.a)
target_link_libraries(${TARGET_NAME} libmpi.a)
target_link_libraries(${TARGET_NAME} libjpeg.a)
target_link_libraries(${TARGET_NAME} libupvqe.a)
target_link_libraries(${TARGET_NAME} libVoiceEngine.a)
target_link_libraries(${TARGET_NAME} libdnvqe.a)
target_link_libraries(${TARGET_NAME} stdc++)
target_link_libraries(${TARGET_NAME} dl)
target_link_libraries(${TARGET_NAME} m)
target_link_libraries(${TARGET_NAME} pthread)

# 指定安装路径
install(TARGETS ${TARGET_NAME} DESTINATION /work/)
