set(LIBPIPE_SRCS
    zw_usbpipe_util.c
    zw_usbpipe_base.c
    zw_usbpipe_uptp.c
    zw_usbpipe_uptp_api.c
)

add_library(usbpipe ${LIBPIPE_SRCS})
# target_link_libraries(live_common pthread rt)

install(TARGETS usbpipe DESTINATION lib)

file(GLOB HEADERS "*/*.h")
install(FILES ${HEADERS} DESTINATION include/usbpipe)

# add_subdirectory(test)
