#include <stdio.h>
#include <sys/types.h>

#include "ngx_pool.h"

//#define HGH_DBG(fmt, args...) printf("\033[40;33m [POOL]-(%s %d):\t\033[0m"fmt,  __func__, __LINE__, ## args)

int main(int argc, char const *argv[])
{
    ngx_pool_t *pool = NULL;
    // 创建内存池
    pool = ngx_create_pool(512);
    // 重置内存池
    // 销毁内存池
    ngx_destroy_pool(pool);
    pool = NULL;

    return 0;
}

