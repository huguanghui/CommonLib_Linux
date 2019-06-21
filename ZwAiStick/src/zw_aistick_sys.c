#include "zw_usbpipe_uptp_api.h"
#include "zw_aistick_sys.h"
#include "zw_aistick_task.h"

static void CB_EventHandler(struct zw_uptp_message* msg, void* user_data)
{
    (void)user_data;

    AS_TASK_REQUEST* req = (AS_TASK_REQUEST*)msg->data;

    info("[%s %s %d]cmd[%d] dataLen[%d]\n", __FILE__, __FUNCTION__, __LINE__, req->cmd, req->paramLen);

    return;
}

static void* fxn_task_done_thread(void *arg)
{
    AS_SYS_INFO* info = (AS_SYS_INFO*)arg;
    struct taskQueue* tq = (struct taskQueue*)info->taskQueue;
    struct taskNode *node = NULL;
    int rc = 0;
    char* req = NULL;
    int reqLen = 0;
    int pos = 0;
    char* result = NULL;
    int resultLen = 0;

    while (!info->bTaskDoneExit)
    {
        info("[%s:%d]\n", __FUNCTION__, __LINE__);
        if (TAILQ_EMPTY(tq))
        {
            info("[%s:%d]\n", __FUNCTION__, __LINE__);
            sem_wait(&g_taskQueueSem);
            info("[%s:%d]\n", __FUNCTION__, __LINE__);
        }
        else
        {
            info("[%s:%d]\n", __FUNCTION__, __LINE__);
            node = AS_TASK_QUEUE_GET(tq);
            if (node)
            {
                info("[%s:%d]\n", __FUNCTION__, __LINE__);
                pos = 0;
                reqLen = sizeof(int) + node->req.paramLen;
                req = malloc(reqLen);
                memcpy(req, &node->req, sizeof(int));
                pos += sizeof(int);
                memcpy(req+pos, node->req.param, node->req.paramLen);
                rc = zw_usbpipe_uptp_call_sync(req, reqLen, (void**)&result, &resultLen);
                free(req);
                if (0 == rc)
                {
                    if (node->cb)
                    {
                        (node->cb)(result, resultLen);
                        free(result);
                    }
                }
                else
                {
                    err("zw_usbpipe_uptp_call_sync call failed - [%d]\n", rc);
                }
                
            }
            info("[%s:%d]\n", __FUNCTION__, __LINE__);
        }
    }

    return NULL;
}

AS_API AS_S32 AS_SYS_Init(AS_SYS_INFO* info)
{
    if (NULL == info)
    {
        err("param error! info[%p]\n", info);
        return -1;
    }

    info->bTaskDoneExit = 0;
    sem_init(&g_taskQueueSem, 0, 0);
    // 建立连接
    zw_usbpipe_uptp_init();
    // 时间同步
    // 启动心跳
    // 启动同步消息传输
    // 启动异步消息传输
    AS_TASK_CreateQueue(&info->taskQueue);
    //   创建任务队列的处理线程
    pthread_create(&info->taskDoneId, NULL, fxn_task_done_thread, (void *)info);
    // 启动异步消息监听
    zw_usbpipe_uptp_bind_opt(CB_EventHandler, NULL);
    // 获取能力

    return 0;
}

AS_API AS_S32 AS_SYS_UnInit()
{
    zw_usbpipe_uptp_uninit();

    return 0;
}

AS_API AS_S32 AS_SYS_GetState()
{

    return 0;
}
