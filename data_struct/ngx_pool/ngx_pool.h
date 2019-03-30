#ifndef __NGX_POOL_H__
#define __NGX_POOL_H__

#include <linux/types.h>

#define HGH_DBG(fmt, args...) printf("\033[40;33m [POOL]-(%s %d):\t\033[0m"fmt,  __func__, __LINE__, ## args)

typedef struct ngx_pool_s            ngx_pool_t;

#define NGX_MAX_ALLOC_FROM_POOL  (4096 - 1)

#define NGX_DEFAULT_POOL_SIZE    (16 * 1024)

#define NGX_POOL_ALIGNMENT       16
#define NGX_MIN_POOL_SIZE                                                     \
    ngx_align((sizeof(ngx_pool_t) + 2 * sizeof(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


typedef void (*ngx_pool_cleanup_pt)(void *data);

typedef struct ngx_pool_cleanup_s  ngx_pool_cleanup_t;

struct ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    void                 *data;
    ngx_pool_cleanup_t   *next;
};

typedef struct ngx_pool_large_s  ngx_pool_large_t;

struct ngx_pool_large_s {
    ngx_pool_large_t     *next;
    void                 *alloc;
};

typedef struct {
    u_char               *last;
    u_char               *end;
    ngx_pool_t           *next;
    unsigned int          failed;
} ngx_pool_data_t;

struct ngx_pool_s {
    ngx_pool_data_t       d;
    size_t                max;
    ngx_pool_t           *current;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
};

// 创建内存池
ngx_pool_t *ngx_create_pool(size_t size);
// 销毁内存池
void ngx_destroy_pool(ngx_pool_t *pool);
// 重置内存池
void ngx_reset_pool(ngx_pool_t *pool);

// 使用内存池分配siz大小的内存
void *ngx_palloc(ngx_pool_t *pool, size_t size);
// 使用内存池分配siz大小的内存
void *ngx_pnalloc(ngx_pool_t *pool, size_t size);
// 使用内存池分配siz大小的内存,并执行初始化
void *ngx_pcalloc(ngx_pool_t *pool, size_t size);
// 使用内存池分配siz大小对其字节为alignment的内存
void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
// 释放指定
ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p);

#endif