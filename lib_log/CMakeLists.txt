set(TARGET_NAME "log")

FILE(GLOB SRC_SOURCE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
set(SRC_LIST ${SRC_SOURCE_LIST})

# message(STATUS "CMAKE_CURRENT_LIST_FILE= " ${CMAKE_CURRENT_LIST_FILE})
# message(STATUS "CMAKE_CURRENT_SOURCE_DIR= " ${CMAKE_CURRENT_SOURCE_DIR})

set(C_FLAGS
-Wall
#-Werror
-rdynamic
-D_GNU_SOURCE
-D__USE_XOPEN
)

string(REPLACE ";" " " CMAKE_C_FLAGS "${C_FLAGS}")
message(STATUS "CMAKE_C_FLAGS = " ${CMAKE_C_FLAGS})

add_library(${TARGET_NAME} ${SRC_LIST})

install(TARGETS ${TARGET_NAME} DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/log)