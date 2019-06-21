#include "zw_usbpipe_uptp_api.h"
#include "zw_aistick_task.h"

struct testQueue g_msgQueue;

int g_seq = 0;

AS_API AS_S32 AS_TASK_CreateFaceTrackQueue(AS_HANDLE *phQueue)
{
    return 0;
}

AS_API AS_S32 AS_TASK_CreateQueue(AS_HANDLE *phQueue)
{
    if (NULL == *phQueue)
    {
        return -1;
    }

    struct taskQueue* tq = (struct taskQueue*)malloc(sizeof(struct taskQueue));
    
    TAILQ_INIT(tq);

    *phQueue = tq;

    return 0;
}

AS_API AS_S32 AS_TASK_DestroyQueue(AS_HANDLE hQueue)
{
    AS_TASK_QUEUE_RELEASE(hQueue);
    free(hQueue);

    return 0;
}

AS_API AS_S32 AS_TASK_Add(AS_HANDLE hQueue, struct taskNode* pstTaskAttr)
{
    struct taskNode* node = malloc(sizeof(struct taskNode));
    memcpy(node, pstTaskAttr, sizeof(struct taskNode));
    AS_TASK_QUEUE_INSERT(hQueue, node);
    sem_post(&g_taskQueueSem);

    return 0;
}

AS_API AS_S32 AS_TASK_Run(AS_TASK_REQUEST* pstTaskAttr, AS_TASK_RESULT* pstTaskResult)
{
    if (NULL == pstTaskAttr || NULL == pstTaskResult)
    {
        err("Param Error! pstTaskAttr[%p] pstTaskResult[%p]\n", pstTaskAttr, pstTaskResult);
        return -1;
    }

    int rc = 0;
    int reqLen = 0;
    int pos = 0;
    char* result = NULL;
    int resultLen = 0;
    struct zw_uptp_message msg;
    
    memset(&msg, 0, sizeof(struct zw_uptp_message));
    msg.dataLen = sizeof(int) + pstTaskAttr->paramLen;
    char *data = msg.data;
    pos = 0;
    memcpy(data, pstTaskAttr, sizeof(int));
    pos += sizeof(int);
    memcpy(data+pos, pstTaskAttr->param, pstTaskAttr->paramLen);
    msg.seq = ++g_seq;

    struct testNode* node = malloc(sizeof(struct testNode));
    memset(node, 0, sizeof(struct testNode));
    node->req = msg.seq;
    node->type = CALL_SYNC;
    sem_init(&node->sem, 0, 0);
    AS_TASK_TEST_QUEUE_INSERT(&g_msgQueue, node);

    zw_usbpipe_uptp_write(&msg);

    struct timespec ts;
    int s;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 1;

    while ((s = sem_timedwait(&node->sem, &ts)) == -1 && errno == EINTR)
        continue;       /* Restart if interrupted by handler */

   if (s == -1) 
   {
        if (errno == ETIMEDOUT)
            info("sem_timedwait() timed out\n");
        else
            info("sem_timedwait");
    } else
    {
        info("sem_timedwait() succeeded\n");
    }
    //sem_wait(&node->sem);
    
    memcpy(pstTaskResult, &node->response, sizeof(AS_TASK_RESULT));
    TAILQ_REMOVE(&g_msgQueue, node, nodes);
    sem_destroy(&node->sem);
    free(node);

    return rc;
}