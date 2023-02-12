set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_PROCESSOR mips)

# Specify the cross compiler.
set(CMAKE_C_COMPILER /opt/gcc_720/mips-gcc720-glibc229-r5.1.4/bin/mips-linux-gnu-gcc CACHE FILEPATH "C compiler")
set(CMAKE_CXX_COMPILER /opt/gcc_720/mips-gcc720-glibc229-r5.1.4/bin/mips-linux-gnu-g++ CACHE FILEPATH "C++ compiler")
set(CMAKE_ASM_COMPILER /opt/gcc_720/mips-gcc720-glibc229-r5.1.4/bin/mips-linux-gnu-gcc CACHE FILEPATH "ASM compiler")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -muclibc -march=mips32r2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -muclibc -march=mips32r2")
set(CMAKE_LD_FLAGS "${CMAKE_CXX_FLAGS} -muclibc")

# Search libraries only under *target* paths.
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
