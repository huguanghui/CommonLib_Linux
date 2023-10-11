set(PROC_NAME "smemcap")

aux_source_directory(. MOD_SRC)
add_executable(${PROC_NAME} ${MOD_SRC})

target_link_libraries(${PROC_NAME} Threads::Threads)

install(TARGETS ${PROC_NAME} RUNTIME DESTINATION bin)
