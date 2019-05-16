set(TAGET_DEMO01 gprof01)
set(TAGET_DEMO01_LIST test_gprof.c)

add_definitions(-pg)

add_executable(${TAGET_DEMO01} ${TAGET_DEMO01_LIST})
target_link_libraries(${TAGET_DEMO01} -pg)