#include <sys/time.h> 
#include <fstream>
#include "zw_usbpipe_uptp_api.h"
#include "zw_aistick_task.h"

// 智能棒端等待响应消息的队列(同步, 异步)
struct waitQueue g_queueWaitMsg;

int g_seq = 0;

AS_API AS_S32 AS_TASK_CreateFaceTrackQueue(AS_HANDLE *phQueue)
{
	return 0;
}

AS_API AS_S32 AS_TASK_CreateQueue(AS_HANDLE* phQueue, const char *name, void *userData)
{
	if (NULL == phQueue || NULL == name)
	{
		err("param error! phQueue[%p] name[%p]\n", phQueue, name);
		return -1;
	}
	struct taskQueue *tq = (struct taskQueue*)malloc(sizeof(struct taskQueue));
	memset(tq, 0, sizeof(struct taskQueue));
	snprintf(tq->name, sizeof(tq->name), "%s", name);
	tq->user_data = userData;
	AS_TASK_QUEUE_INIT(tq);
	*phQueue = (AS_HANDLE)tq;
	//info("Task-Queue Create![%s]\n", tq->name);
	return 0;
}

AS_API AS_S32 AS_TASK_DestroyQueue(AS_HANDLE hQueue)
{
	struct taskQueue *tq = (struct taskQueue*)hQueue;
	
	if (NULL == tq)
	{
		err("param error! hQueue[%p]\n", tq);
		return -1;
	} 
	AS_TASK_QUEUE_RELEASE(tq);
	
	info("Task-Queue Destroyed![%s]\n", tq->name);
	
	if (tq->user_data != NULL)
	{
		free(tq->user_data);
		tq->user_data = NULL;
	}
	free(tq);
	return 0;
}

AS_API AS_S32 AS_TASK_Add(AS_HANDLE hQueue, struct taskNode *pstTaskAttr)
{
	struct taskQueue *queue = NULL;
	struct taskNode *node = NULL;

	queue = (struct taskQueue*)hQueue;
	node = (struct taskNode*)malloc(sizeof(struct taskNode));
	memcpy(node, pstTaskAttr, sizeof(struct taskNode));
	AS_TASK_QUEUE_INSERT(queue, node);
	sem_post(&queue->task_sem);
	return 0;
}

AS_API AS_S32 AS_TASK_Run(AS_TASK_REQUEST* pstTaskAttr, AS_TASK_RESULT* pstTaskResult)
{
	if (NULL == pstTaskAttr || NULL == pstTaskResult)
	{
		err("Param Error! pstTaskAttr[%p] pstTaskResult[%p]\n", pstTaskAttr, pstTaskResult);
		return -1;
	}

	int ret = 0;
	int pos = 0;
	struct zw_uptp_message *msg = NULL;
	
	msg = (struct zw_uptp_message *)malloc(sizeof(struct zw_uptp_message));
	memset(msg, 0, sizeof(struct zw_uptp_message));
	msg->dataLen = sizeof(int) + pstTaskAttr->paramLen;
	char *data = msg->data;
	pos = 0;
	memcpy(data, pstTaskAttr, sizeof(int));
	pos += sizeof(int);
	memcpy(data+pos, pstTaskAttr->param, pstTaskAttr->paramLen);
	msg->seq = ++g_seq;

	struct waitNode *node = (struct waitNode*)malloc(sizeof(struct waitNode));
	memset(node, 0, sizeof(struct waitNode));
	node->req = msg->seq;
	node->type = CALL_SYNC;
	sem_init(&node->sem, 0, 0);
	//info("node->req[%d]\n", node->req);
	AS_TASK_WAIT_QUEUE_INSERT(&g_queueWaitMsg, node);

	if (AS_TASK_HTTP_CONFIG == pstTaskAttr->cmd)
	{
		info("msgType[%d] Seq[%d] DataLen[%d]\n", msg->msgType, msg->seq, msg->dataLen);
		info("CMD[%d] ParamLen[%d]\n", pstTaskAttr->cmd, pstTaskAttr->paramLen);
		info("Request[%.*s]\n", pstTaskAttr->paramLen, pstTaskAttr->param);
	}
	zw_usbpipe_uptp_write(msg);
	struct timespec ts;
	int s;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += 5;

	while ((s = sem_timedwait(&node->sem, &ts)) == -1 && errno == EINTR)
		continue;       /* Restart if interrupted by handler */

	if (s == -1) 
	{
		if (errno == ETIMEDOUT)
			info("sem_timedwait() timed out\n");
		else
			info("sem_timedwait");
		ret = -1;
	} else
	{
		//info("sem_timedwait() succeeded\n");
	}
	if (0 == ret && AS_TASK_HTTP_CONFIG == pstTaskAttr->cmd)
	{
		info("Result[%.*s]\n", node->response.paramLen, node->response.param);
	}
	memcpy(pstTaskResult, &node->response, sizeof(AS_TASK_RESULT));
	TAILQ_REMOVE(&g_queueWaitMsg, node, nodes);
	sem_destroy(&node->sem);
	free(node);
	free(msg);

	return ret;
}
