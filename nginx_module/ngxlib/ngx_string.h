#ifndef _NGX_STRING_H
#define _NGX_STRING_H

#include "ngx_config.h"
#include "ngx_core.h"

typedef struct {
    size_t      len;
    u_char     *data;
} ngx_str_t;

#define ngx_string(str)     { sizeof(str) - 1, (u_char *) str }
#define ngx_null_string     { 0, NULL }
#define ngx_str_set(str, text)                                               \
    (str)->len = sizeof(text) - 1; (str)->data = (u_char *) text
#define ngx_str_null(str)   (str)->len = 0; (str)->data = NULL

#define ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

#define ngx_strncmp(s1, s2, n)  strncmp((const char *) s1, (const char *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#define ngx_strcmp(s1, s2)  strcmp((const char *) s1, (const char *) s2)


#define ngx_strstr(s1, s2)  strstr((const char *) s1, (const char *) s2)
#define ngx_strlen(s)       strlen((const char *) s)

#define ngx_memcpy(dst, src, n)   (void) memcpy(dst, src, n)
#define ngx_cpymem(dst, src, n)   (((u_char *) memcpy(dst, src, n)) + (n))

#define ngx_strchr(s1, c)   strchr((const char *) s1, (int) c)

#define ngx_memzero(buf, n)       (void) memset(buf, 0, n)
#define ngx_memset(buf, c, n)     (void) memset(buf, c, n)

#define ngx_base64_encoded_length(len)  (((len + 2) / 3) * 4)
#define ngx_base64_decoded_length(len)  (((len + 3) / 4) * 3)

void ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
void ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src);

#endif