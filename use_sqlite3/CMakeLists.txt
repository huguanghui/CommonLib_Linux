set(TARGET01 sqltest01)
set(TARGET01_LIST sql_main.c sqlite3.c)

add_executable(${TARGET01} ${TARGET01_LIST})
target_link_libraries(${TARGET01} dl)
target_link_libraries(${TARGET01} m)
target_link_libraries(${TARGET01} pthread)