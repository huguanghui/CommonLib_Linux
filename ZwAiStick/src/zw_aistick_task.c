#include "zw_usbpipe_uptp_api.h"
#include "zw_aistick_task.h"

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
    char* req = NULL;
    int reqLen = 0;
    int pos = 0;
    char* result = NULL;
    int resultLen = 0;

    pos = 0;
    reqLen = sizeof(int) + pstTaskAttr->paramLen;
    req = malloc(reqLen);
    memcpy(req, pstTaskAttr, sizeof(int));
    pos += sizeof(int);
    memcpy(req+pos, pstTaskAttr->param, pstTaskAttr->paramLen);
    rc = zw_usbpipe_uptp_call_sync(req, reqLen, (void**)&result, &resultLen);
    free(req);
    if (0 == rc)
    {
        pstTaskResult->param = result;
        pstTaskResult->paramLen = resultLen;
    }
    
    return rc;
}