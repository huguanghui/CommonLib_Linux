set(PROC_NAME "tt_test")

aux_source_directory(. PROC_SRC)
add_executable(${PROC_NAME} ${PROC_SRC})
target_link_libraries(
  ${PROC_NAME}
  Threads::Threads
  dl
  m
  rt)

install(TARGETS ${PROC_NAME} RUNTIME DESTINATION bin)

