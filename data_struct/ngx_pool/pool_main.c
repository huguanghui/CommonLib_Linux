#include <stdio.h>
#include <sys/types.h>

#include "ngx_pool.h"

int main(int argc, char const *argv[])
{
    ngx_pool_t *pool = NULL;
    // 创建内存池
    pool = ngx_create_pool(512);
    // 重置内存池
    //ngx_reset_pool(pool);
    char *buf = ngx_palloc(pool, 4096);
    snprintf(buf, 4096, "hello world!");
    // 销毁内存池
    ngx_destroy_pool(pool);
    pool = NULL;

    return 0;
}

