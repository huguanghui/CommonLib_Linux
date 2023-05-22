set(PROC_NAME "btree_test")

aux_source_directory(. MOD_SRC)
include_directories(${PROJECT_SOURCE_DIR}/lib_btree/)

add_executable(${PROC_NAME} ${MOD_SRC})
add_dependencies(${PROC_NAME} "btree")

target_link_libraries(
  ${PROC_NAME}
  btree
  Threads::Threads
  dl
  m
  rt
  stdc++)

install(TARGETS ${PROC_NAME} RUNTIME DESTINATION bin)
