set(TARGET_NAME test_init_uninit)
set(TARGET_LIST main.cpp)

add_executable(${TARGET_NAME} ${TARGET_LIST})

# 指定安装路径
#install(TARGETS demo DESTINATION /work)
