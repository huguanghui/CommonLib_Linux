#ifndef __MG_UTIL_H__
#define __MG_UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
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



#define MBUF_SIZE_MULTIPLIER 1.5
#define MBUF_SIZE_MAX_HEADROOM 1024

/* 内存缓存结构 */
struct mbuf {
	char *buf;   /* 内存指针 */
	size_t len;  /* 数据长度 */
	size_t size; /* 内存大小 */
};
/*************************************** mbuf ***************************************/
/*
 * mbuf 初始化.
 */
void mbuf_init(struct mbuf *, size_t initial_capacity);

/* mbuf 释放和重置 */
void mbuf_free(struct mbuf *);

/*
 * 向 mbuf 中新增数据
 *
 * 返回: 新增数据长度
 */
size_t mbuf_append(struct mbuf *, const void *data, size_t data_size);

/*
 * 向 mbuf 中新增数据, 加完后释放数据
 *
 * 返回: 新增数据长度
 */
size_t mbuf_append_and_free(struct mbuf *, void *data, size_t data_size);

/*
 * 向 mbuf 指定的偏移位置插入数据
 *
 * 返回: 插入数据长度
 */
size_t mbuf_insert(struct mbuf *, size_t, const void *, size_t);

/* 移除 mbuf 中数据开始 data_size 的内容 */
void mbuf_remove(struct mbuf *, size_t data_size);

/*
 * 重新调整 mbuf 的大小
 * note: new_size 小于 size 不会调整
 */
void mbuf_resize(struct mbuf *, size_t new_size);

/* 将 mbuf 的内容从移动到另一个变量中 */
void mbuf_move(struct mbuf *from, struct mbuf *to);

/* 清空 mbuf 的数据内容 */
void mbuf_clear(struct mbuf *);

/* 让 mbuf 的大小等于数据的长度 */
void mbuf_trim(struct mbuf *);

#ifdef __cplusplus
}
#endif


#endif
