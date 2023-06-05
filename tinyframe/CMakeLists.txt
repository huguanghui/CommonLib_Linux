set(PROC_NAME "tinyframe")

add_executable(${PROC_NAME} main.c TinyFrame.c)

target_link_libraries(${PROC_NAME} Threads::Threads dl m rt stdc++)

install(TARGETS ${PROC_NAME} RUNTIME DESTINATION bin)
