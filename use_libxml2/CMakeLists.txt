set(TARGET xml_demo)
set(SRC_LIST xml_main.c)

include_directories(/usr/local/include/libxml2)
add_executable(${TARGET} ${SRC_LIST})
target_link_libraries(${TARGET} xml2)

add_executable(xml_create gen_main.cpp)
target_link_libraries(xml_create xml2)
