#ifndef __MBUF_H__
#define __MBUF_H__

#include <stdlib.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* 内存块 */
struct mg_str {
  const char *p; /* 内存块地址 */
  size_t len;    /* 长度 */
};

/*
 * C string 转换为 mg_str
 * 
 */
struct mg_str mg_mk_str(const char *s);

/*
 * C string 转换为 mg_str, 截取指定长度
 */
struct mg_str mg_mk_str_n(const char *s, size_t len);

/* mg_str初始化宏 */
#define MG_MK_STR(str_literal) \
  { str_literal, sizeof(str_literal) - 1 }
#define MG_MK_STR_N(str_literal, len) \
  { str_literal, len }
#define MG_NULL_STR \
  { NULL, 0 }

/*
 * Cross-platform version of `strcmp()` where where first string is
 * specified by `struct mg_str`.
 */
int mg_vcmp(const struct mg_str *str2, const char *str1);

/*
 * Cross-platform version of `strncasecmp()` where first string is
 * specified by `struct mg_str`.
 */
int mg_vcasecmp(const struct mg_str *str2, const char *str1);

/* Creates a copy of s (heap-allocated). */
struct mg_str mg_strdup(const struct mg_str s);

/*
 * Creates a copy of s (heap-allocated).
 * Resulting string is NUL-terminated (but NUL is not included in len).
 */
struct mg_str mg_strdup_nul(const struct mg_str s);

/*
 * Locates character in a string.
 */
const char *mg_strchr(const struct mg_str s, int c);

/*
 * Compare two `mg_str`s; return value is the same as `strcmp`.
 */
int mg_strcmp(const struct mg_str str1, const struct mg_str str2);

/*
 * Like `mg_strcmp`, but compares at most `n` characters.
 */
int mg_strncmp(const struct mg_str str1, const struct mg_str str2, size_t n);

/*
 * Free the string (assuming it was heap allocated).
 */
void mg_strfree(struct mg_str *s);

/*
 * Finds the first occurrence of a substring `needle` in the `haystack`.
 */
const char *mg_strstr(const struct mg_str haystack, const struct mg_str needle);

/* Strip whitespace at the start and the end of s */
struct mg_str mg_strstrip(struct mg_str s);

/* Returns 1 if s starts with the given prefix. */
int mg_str_starts_with(struct mg_str s, struct mg_str prefix);


#if defined(__cplusplus)
}
#endif /* __cplusplus */


#endif