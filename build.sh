#!/bin/sh

SOURCE_DIR=$(pwd)

# BUILD_TYPE_ARRAY="debug release"
BUILD_TYPE=debug
INSTALL_DIR=output/
BUILD_DIR=build/

cmake -H. -B${BUILD_DIR} \
  -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
#\ -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/mips-linux-gnu-t40_glibc.cmake
# -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/arm-linux-gnu-sv823c_glibc_gcc750.cmake
# -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/mips-linux-gnu-t41_uclibc_gcc720.cmake
# -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/mips-linux-gnu-t31_uclibc_gcc472.cmake
cmake --build ${BUILD_DIR} -j8
cmake --install ${BUILD_DIR}
