#ifndef __NGX_STRING_H__
#define __NGX_STRING_H__

#include <sys/types.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t        len;
    u_char        *data;
} ngx_str_t;

#define ngx_string(str)     { sizeof(str) - 1, (u_char *) str }
#define ngx_null_string     { 0, NULL }
#define ngx_str_set(str, text)                                               \
    (str)->len = sizeof(text) - 1; (str)->data = (u_char *) text
#define ngx_str_null(str)   (str)->len = 0; (str)->data = NULL

size_t ngx_strnlen(u_char *p, size_t n);

#endif