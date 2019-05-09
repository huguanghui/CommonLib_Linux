set(TARGET01 th_test)
set(TARGET01_LIST th_main.c)

add_executable(${TARGET01} ${TARGET01_LIST})
target_link_libraries(${TARGET01} pthread)