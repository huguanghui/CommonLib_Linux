set(TARGET_LINK_OPTIONS "Threads::Threads" "dl" "rt")

if(CMAKE_BUILD_TYPE MATCHES release)
  list(APPEND TARGET_LINK_OPTIONS "-s")
endif()

set(PROC_NAME "test3")
add_executable(${PROC_NAME} test3.c)

target_link_libraries(${PROC_NAME} ${TARGET_LINK_OPTIONS})
install(TARGETS ${PROC_NAME} RUNTIME DESTINATION bin)
