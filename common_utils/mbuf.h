#ifndef COMMON_MBUF_H

/*************************************** mbuf ***************************************/

#define MBUF_SIZE_MULTIPLIER 1.5
#define MBUF_SIZE_MAX_HEADROOM 1024

/* 内存缓存结构 */
struct mbuf {
	char *buf;   /* 内存指针 */
	size_t len;  /* 数据长度 */
	size_t size; /* 内存大小 */
};

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

/* 使用 printf 结构将数据保存到mbuf中 */
int mbuf_printf(struct mbuf *m, const char *fmt, ...);

#endif  /* mbuf.h */
