set(PROC_NAME "smemcap")

set(TARGET_LINK_OPTIONS "Threads::Threads" "-Wl,-exclude-libs=ALL,-gc-sections")

if(CMAKE_BUILD_TYPE MATCHES release)
  list(APPEND TARGET_LINK_OPTIONS "-s")
endif()

aux_source_directory(. MOD_SRC)
add_executable(${PROC_NAME} ${MOD_SRC})

target_link_libraries(${PROC_NAME} ${TARGET_LINK_OPTIONS})

install(TARGETS ${PROC_NAME} RUNTIME DESTINATION bin)
