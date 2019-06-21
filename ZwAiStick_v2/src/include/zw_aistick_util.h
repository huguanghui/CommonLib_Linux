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

#endif