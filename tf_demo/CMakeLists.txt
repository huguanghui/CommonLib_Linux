set(TF_SRC TinyFrame.c utils.c)

include_directories(.)

set(PROC_NAME "tf_t1")
add_executable(${PROC_NAME} test/t1/main.c ${TF_SRC})
target_link_libraries(${PROC_NAME} Threads::Threads dl m rt stdc++)
install(TARGETS ${PROC_NAME} RUNTIME DESTINATION bin)

set(PROC_NAME "tf_t2")
add_executable(${PROC_NAME} test/t2/main.c ${TF_SRC})
target_link_libraries(${PROC_NAME} Threads::Threads dl m rt stdc++)
install(TARGETS ${PROC_NAME} RUNTIME DESTINATION bin)
