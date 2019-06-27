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
	//!< 查询所有脸库信息
	AS_TASK_FACEGROUP_GET_ALL,
	//!< HTTP配置
	AS_TASK_HTTP_CONFIG,
} AS_TASK_TYPE;

//!\brief 响应数据
typedef struct
{
	AS_HANDLE taskQueue;
	pthread_t taskDoneId;
	int bTaskDoneExit;
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
	AS_TASK_REQUEST	req;
	Response_CB cb;
	TAILQ_ENTRY(taskNode) nodes;
};

TAILQ_HEAD(taskQueue, taskNode);

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

// 测试队列的封装
struct testNode
{
	int	req;   // 请求发送的序号 
	int type;  // 0-同步 1-异步
	Response_CB cb; // 异步有效
	AS_TASK_RESULT response; // 响应消息
	sem_t sem; 
	TAILQ_ENTRY(testNode) nodes;
};

TAILQ_HEAD(testQueue, testNode);

void AS_TASK_TEST_QUEUE_INSERT(struct testQueue* tq, struct testNode* node);
struct testNode* AS_TASK_TEST_QUEUE_GET(struct testQueue* tq);
struct testNode* AS_TASK_TEST_QUEUE_SEARCH_BY_SEQ(struct testQueue* tq, int seq);
void AS_SHOW_TASK_TEST_QUEUE(struct testQueue* tq);
void AS_TASK_TEST_QUEUE_RELEASE(struct testQueue* tq);

extern sem_t g_testQueueSem;

// 保存请求返回的数据的队列

struct responseNode
{
	int seq;
	int dataLen;
	void* data;
	TAILQ_ENTRY(responseNode) nodes;
};

TAILQ_HEAD(responseQueue, responseNode);

void AS_RESPONSE_QUEUE_INSERT(struct responseQueue* tq, struct responseNode* node);
struct responseNode* AS_RESPONSE_QUEUE_GET(struct responseQueue* tq);
struct responseNode* AS_RESPONSE_QUEUE_SEARCH_BY_SEQ(struct responseQueue* tq, int seq);
void AS_RESPONSE_TEST_QUEUE(struct responseQueue* tq);
void AS_RESPONSE_QUEUE_RELEASE(struct responseQueue* tq);

extern sem_t g_respQueueSem;

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
