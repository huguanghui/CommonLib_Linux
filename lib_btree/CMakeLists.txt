set(TARGET_NAME "btree")

aux_source_directory(. SRC_LIST)
add_library(${TARGET_NAME} ${SRC_LIST})

install(TARGETS ${TARGET_NAME} DESTINATION lib)
file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/btree)
