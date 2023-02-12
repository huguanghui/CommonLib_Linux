set(PROC_NAME "ntp_test")
aux_source_directory(. MOD_SRC)

add_executable(${PROC_NAME} ${MOD_SRC})

# add_dependencies(${PROC_NAME} "util_fw")

target_link_libraries(
  ${PROC_NAME}
  # util_fw
  Threads::Threads dl m rt stdc++)

install(TARGETS ${PROC_NAME} RUNTIME DESTINATION bin)
