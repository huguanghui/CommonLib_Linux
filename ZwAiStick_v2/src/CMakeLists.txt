set(TARGET_NAME zwaistck)
set(TARGET_LIST
    zw_aistick.c
    zw_aistick_task.c
    zw_aistick_sys.c
    zw_aistick_util.c
    zw_aistick_api.c
)

add_library(${TARGET_NAME} ${TARGET_LIST})

# install(TARGETS ${TARGET_NAME} DESTINATION lib)

# file(GLOB HEADERS "*.h")
# install(FILES ${HEADERS} DESTINATION include/${TARGET_NAME})
