#include "ngx_pool.h"

// static void *ngx_palloc_small(ngx_pool_t *pool, size_t size, int align);
// static void *ngx_palloc_block(ngx_pool_t *pool, size_t size);
// static void *ngx_palloc_large(ngx_pool_t *pool, size_t size);

ngx_pool_t *ngx_create_pool(size_t size)
{
    ngx_pool_t  *p;

    HGH_DBG("start create!\n");
    p = malloc(size * sizeof(ngx_pool_t));
    if (p == NULL) {
        return NULL;
    }
    HGH_DBG("Malloc[0x%p]\n", p);
    p->d.last = (u_char *) p + sizeof(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = NULL;
    p->d.failed = 0;

    size = size - sizeof(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->large = NULL;
    p->cleanup = NULL;

    HGH_DBG("end create!\n");

    return p;
}

void ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    HGH_DBG("Begin destroy!\n");
    for (c = pool->cleanup; c; c = c->next) {
        if (c->handler) {
            c->handler(c->data);
        }
    }

    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            free(l->alloc);
        }
    }

    for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        HGH_DBG("free[0x%p]\n", p);
        free(p);

        if (n == NULL) {
            break;
        }
    }

    HGH_DBG("End destroy!\n");

    return ;
}

void ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            free(l->alloc);
        }
    }

    for (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + sizeof(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->large = NULL;
}

// void *ngx_palloc(ngx_pool_t *pool, size_t size)
// {
//     return ngx_palloc_large(pool, size);
// }

// void *ngx_pnalloc(ngx_pool_t *pool, size_t size)
// {
//     return ngx_palloc_large(pool, size);
// }

// void *ngx_pcalloc(ngx_pool_t *pool, size_t size)
// {
//     void *p;

//     p = ngx_palloc(pool, size);
//     if (p) {
//         memset(p, 0, size);
//     }

//     return p;
// }

// static void *ngx_palloc_small(ngx_pool_t *pool, size_t size, int align)
// {
//     u_char      *m;
//     ngx_pool_t  *p;

//     p = pool->current;

//     do {
//         m = p->d.last;

//         if (align) {
//             m = ngx_align_ptr(m, NGX_ALIGNMENT);
//         }

//         if ((size_t) (p->d.end - m) >= size) {
//             p->d.last = m + size;

//             return m;
//         }

//         p = p->d.next;

//     } while (p);

//     return ngx_palloc_block(pool, size);
// }

// static void *ngx_palloc_block(ngx_pool_t *pool, size_t size)
// {
//     u_char      *m;
//     size_t       psize;
//     ngx_pool_t  *p, *new;

//     psize = (size_t) (pool->d.end - (u_char *) pool);

//     m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
//     if (m == NULL) {
//         return NULL;
//     }

//     new = (ngx_pool_t *) m;

//     new->d.end = m + psize;
//     new->d.next = NULL;
//     new->d.failed = 0;

//     m += sizeof(ngx_pool_data_t);
//     m = ngx_align_ptr(m, NGX_ALIGNMENT);
//     new->d.last = m + size;

//     for (p = pool->current; p->d.next; p = p->d.next) {
//         if (p->d.failed++ > 4) {
//             pool->current = p->d.next;
//         }
//     }

//     p->d.next = new;

//     return m;
// }

// static void *ngx_palloc_large(ngx_pool_t *pool, size_t size)
// {
//     void              *p;
//     int                 n;
//     ngx_pool_large_t  *large;

//     p = malloc(size);
//     if (p == NULL) {
//         return NULL;
//     }

//     n = 0;

//     for (large = pool->large; large; large = large->next) {
//         if (large->alloc == NULL) {
//             large->alloc = p;
//             return p;
//         }

//         if (n++ > 3) {
//             break;
//         }
//     }

//     large = ngx_palloc_small(pool, sizeof(ngx_pool_large_t), 1);
//     if (large == NULL) {
//         ngx_free(p);
//         return NULL;
//     }

//     large->alloc = p;
//     large->next = pool->large;
//     pool->large = large;

//     return p;
// }

// void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
// {
//     void              *p;
//     ngx_pool_large_t  *large;

//     p = ngx_memalign(alignment, size, pool->log);
//     if (p == NULL) {
//         return NULL;
//     }

//     large = ngx_palloc_small(pool, sizeof(ngx_pool_large_t), 1);
//     if (large == NULL) {
//         ngx_free(p);
//         return NULL;
//     }

//     large->alloc = p;
//     large->next = pool->large;
//     pool->large = large;

//     return p;
// }

// int ngx_pfree(ngx_pool_t *pool, void *p)
// {
//     ngx_pool_large_t  *l;

//     for (l = pool->large; l; l = l->next) {
//         if (p == l->alloc) {
//             free(l->alloc);
//             l->alloc = NULL;

//             return 0;
//         }
//     }

//     return -1;
// }
