# CommonLib_Linux
主要为Linux的项目提供基础接口库.积累数据结构和实用接口.

[Goole的C/C++编程规范](https://google.github.io/styleguide/cppguide.html)
[中文翻译](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/#general-naming-rules)

https://www.vandyke.com/

## Demo 
```
set(TARGET01 demo_ftpget)
set(TARGET01_LIST ftpget_main.c)

add_executable(${TARGET01} ${TARGET01_LIST})
```

```
# libz.a
# link_directories(${PROJECT_SOURCE_DIR}/lib/z/lib_static)
set(ZLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/zlib)
message(STATUS "ZWHOST_DIR= " ${ZLIB_DIR})
include_directories(${ZLIB_DIR}/include)
find_library(ZLIB_LIBRARY z PATHS ${ZLIB_DIR}/lib_static)
message(STATUS "ZWHOSTS_LIBRARY= " ${ZLIB_LIBRARY})
```

```
target_link_libraries(${TARGET} ${ZLIB_LIBRARY})
```




