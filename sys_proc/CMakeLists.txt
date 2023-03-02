set(PROC_NAME "sys_proc")

aux_source_directory(. MOD_SRC)
add_executable(${PROC_NAME} ${MOD_SRC})

target_link_libraries(
  ${PROC_NAME}
  Threads::Threads dl m rt stdc++)

install(TARGETS ${PROC_NAME} RUNTIME DESTINATION bin)
