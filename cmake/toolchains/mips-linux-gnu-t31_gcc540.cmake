set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_PROCESSOR mips)

# Specify the cross compiler.
set(CMAKE_C_COMPILER /opt/mips-gcc540-glibc222-64bit-r3.3.0/bin/mips-linux-uclibc-gnu-gcc CACHE FILEPATH "C compiler")
set(CMAKE_CXX_COMPILER /opt/mips-gcc540-glibc222-64bit-r3.3.0/bin/mips-linux-uclibc-gnu-g++ CACHE FILEPATH "C++ compiler")
set(CMAKE_ASM_COMPILER /opt/mips-gcc540-glibc222-64bit-r3.3.0/bin/mips-linux-uclibc-gnu-gcc CACHE FILEPATH "ASM compiler")

# Search libraries only under *target* paths.
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
