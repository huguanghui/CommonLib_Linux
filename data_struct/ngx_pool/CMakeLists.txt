#add_executable(ngxpool pool_main.c ngx_pool.c)
set(POOL_SRCS ngx_pool.c)
add_library(poolbase ${POOL_SRCS})