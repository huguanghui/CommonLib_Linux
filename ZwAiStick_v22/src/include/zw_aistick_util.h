#ifndef __ZW_AI_STICK_UTIL_H__
#define __ZW_AI_STICK_UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/queue.h>
#include <semaphore.h>
#include <time.h>
#include <stdarg.h>

#if defined(__cplusplus)
#define AS_API extern "C"
#else
#define AS_API
#endif

typedef uint32_t AS_BOOL;
typedef int8_t AS_S8;
typedef uint8_t AS_U8;
typedef int16_t AS_S16;
typedef uint16_t AS_U16;
typedef int32_t AS_S32;
typedef uint32_t AS_U32;
typedef int64_t AS_S64;
typedef uint64_t AS_U64;

typedef AS_U64 AS_FACEGRP;
typedef AS_U64 AS_FACE;
typedef AS_S64 AS_INDEX;
typedef void * AS_HANDLE;
typedef void AS_VOID;

#define AS_HANDLE_INVALID ((void *)(-1))

typedef int	(*AS_CALLBACK)(void *data, int len, void *user_data);

typedef enum
{
	//!< 抓拍任务
	AS_TASK_CAPTURE_FACE = 0,
	//!< 抽取特征
	AS_TASK_EXTRACT_FEATURE,
	//!< 抓拍并抽取特征
	AS_TASK_CAPTURE_EXTRACT_FEATURE,
	//!< 抓拍并比对
	AS_TASK_CAPTURE_SEARCH,
	//!< 按图片比对
	AS_TASK_SEARCH_BY_JPG,
	//!< 按特征比对
	AS_TASK_SEARCH_BY_FEATURE,
	//!< HTTP配置
	AS_TASK_HTTP_CONFIG,
	//!< 比对流
	AS_TASK_STREAM_FRAME,
	//!< 比对事件上报
	AS_TASK_EVENT_REPORT,
	//!< 智能帧上报
	AS_TASK_SMART_DATA,
	//!< 检测设备是否可操作
	AS_TASK_CHECK_ONLINE,
} AS_TASK_TYPE;

//!\brief 响应数据
typedef struct
{
	int status; // 0 - 未初始化 1 - 已初始化
	AS_HANDLE taskQueueAsyncMsg;
	pthread_t taskDoneId;
	int bTaskDoneExit;
	AS_CALLBACK cbEventReport;
	void *eventReportUserData;
	AS_CALLBACK cbSmartFrameReport;
	void *smartFrameReportUserData;
} AS_SYS_INFO;

//!\brief 请求数据
typedef struct
{
	int cmd;  // 操作类型
	int paramLen;
	void* param;
} AS_TASK_REQUEST;

//!\brief 响应数据
typedef struct
{
	void* param;
	int paramLen;
} AS_TASK_RESULT;

#ifdef __cplusplus
extern "C" {
#endif

// queue封装
typedef void (*Response_CB)(char *result, int len);

// 任务队列的封装
struct taskNode
{
	int type;        // 异步消息类型 - 0 - 没有返回 1 - 需要处理返回消息
	AS_TASK_REQUEST	req;
	Response_CB cb;
	TAILQ_ENTRY(taskNode) nodes;
};

struct taskQueue
{
	char name[32];
	pthread_mutex_t task_mtx;
	sem_t task_sem; 
	struct taskNode *tqh_first;
	struct taskNode **tqh_last;
	void *user_data;
};
// TAILQ_HEAD(taskQueue, taskNode);

void AS_TASK_QUEUE_INIT(struct taskQueue* tq);
void AS_TASK_QUEUE_INSERT(struct taskQueue* tq, struct taskNode* node);
struct taskNode* AS_TASK_QUEUE_GET(struct taskQueue* tq);
void AS_SHOW_TASK_QUEUE(struct taskQueue* tq);
void AS_TASK_QUEUE_RELEASE(struct taskQueue* tq);

// 人脸抓拍和人脸对比任务队列

extern sem_t g_taskQueueSem;

typedef enum
{
	CALL_SYNC = 0,
	CALL_ASYNC
}E_CALL_TYPE;

// 发送消息后等待响应的队列
struct waitNode
{
	int	req;   // 请求发送的序号 
	int type;  // 0-同步 1-异步
	Response_CB cb; // 异步有效
	AS_TASK_RESULT response; // 响应消息
	sem_t sem; 
	TAILQ_ENTRY(waitNode) nodes;
};

struct waitQueue
{
	char name[32];
	pthread_mutex_t mtx;
	struct waitNode *tqh_first;
	struct waitNode **tqh_last;
};

void AS_TASK_WAIT_QUEUE_INIT(struct waitQueue *tq);
void AS_TASK_WAIT_QUEUE_RELEASE(struct waitQueue *tq);
void AS_TASK_WAIT_QUEUE_INSERT(struct waitQueue *tq, struct waitNode *node);
struct waitNode* AS_TASK_WAIT_QUEUE_GET(struct waitQueue *tq);
struct waitNode* AS_TASK_WAIT_QUEUE_SEARCH_BY_SEQ(struct waitQueue *tq, int seq);
void AS_SHOW_TASK_WAIT_QUEUE(struct mbuf *m, struct waitQueue *tq);

// 队列 - 处理响应消息
struct responseNode
{
	int seq;
	int dataLen;
	void* data;
	TAILQ_ENTRY(responseNode) nodes;
};

struct responseQueue
{
	char name[32];
	pthread_mutex_t mtx;
	sem_t sem;
	struct responseNode *tqh_first;
	struct responseNode **tqh_last;
};

void AS_RESPONSE_QUEUE_INIT(struct responseQueue *tq);
void AS_RESPONSE_QUEUE_RELEASE(struct responseQueue *tq);
void AS_RESPONSE_QUEUE_INSERT(struct responseQueue *tq, struct responseNode *node);
struct responseNode* AS_RESPONSE_QUEUE_GET(struct responseQueue *tq);
struct responseNode* AS_RESPONSE_QUEUE_SEARCH_BY_SEQ(struct responseQueue *tq, int seq);
void AS_SHOW_RESPONSE_QUEUE(struct responseQueue *tq);

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
/*************************************** mg_str ***************************************/

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

/*************************************** base64 ***************************************/

typedef void (*cs_base64_putc_t)(char, void *);

struct cs_base64_ctx {
	/* cannot call it putc because it's a macro on some environments */
	cs_base64_putc_t b64_putc;
	unsigned char chunk[3];
	int chunk_size;
	void *user_data;
};

void cs_base64_init(struct cs_base64_ctx *ctx, cs_base64_putc_t putc,
                    void *user_data);
void cs_base64_update(struct cs_base64_ctx *ctx, const char *str, size_t len);
void cs_base64_finish(struct cs_base64_ctx *ctx);

void cs_base64_encode(const unsigned char *src, int src_len, char *dst);
void cs_fprint_base64(FILE *f, const unsigned char *src, int src_len);

/*
 * Decodes a base64 string `s` length `len` into `dst`.
 * `dst` must have enough space to hold the result.
 * `*dec_len` will contain the resulting length of the string in `dst`
 * while return value will return number of processed bytes in `src`.
 * Return value == len indicates successful processing of all the data.
 */
int cs_base64_decode(const unsigned char *s, int len, char *dst, int *dec_len);

// type - 添加方式 0 - 重新保存 1 - 追加数据
void aistick_util_save2file(const char *filename, void *s, int len, int type);

#ifdef __cplusplus
}
#endif

#endif
