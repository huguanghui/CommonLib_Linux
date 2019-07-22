set(TARGET_NAME zwaistck)
set(TARGET_LIST
    zw_aistick_task.cpp
    zw_aistick_sys.cpp
    zw_aistick_util.cpp
    zw_aistick_api.cpp
    zw_aistick_httpclient.cpp
)

add_library(${TARGET_NAME} ${TARGET_LIST})

# install(TARGETS ${TARGET_NAME} DESTINATION lib)

# file(GLOB HEADERS "*.h")
# install(FILES ${HEADERS} DESTINATION include/${TARGET_NAME})
