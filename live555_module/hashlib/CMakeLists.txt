set(hashlib_SRCS
    hashtable.cpp
    strDup.cpp
)

add_library(live_common ${hashlib_SRCS})
target_link_libraries(live_common pthread rt)

install(TARGETS live_common DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/live_common)

add_subdirectory(test)