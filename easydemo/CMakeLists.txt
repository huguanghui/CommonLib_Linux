add_executable(demo demo.cpp)
#target_link_libraries(demo poolbase)
target_link_libraries(demo mongo)
target_link_libraries(demo log)
target_link_libraries(demo pthread)

# 指定安装路径
# install(TARGETS demo DESTINATION /work)
install(TARGETS demo DESTINATION bin)
