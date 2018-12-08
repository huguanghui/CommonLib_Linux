#ifndef _NGX_HASH_H
#define _NGX_HASH_H

#include "ngx_config.h"
#include "ngx_core.h"

typedef struct {
    void             *value;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;

typedef struct {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        size;
} ngx_hash_t;

#endif