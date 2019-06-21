#include <poll.h>
#include "zw_usbpipe_base.h"
#include "zw_usbpipe_uptp.h"
#include "zw_usbpipe_uptp_api.h"

typedef enum 
{
   UPTP_STATUS_UNINIT = 0,
   UPTP_STATUS_INITED,
   UPTP_STATUS_DESTROYED
}E_UPTP_STATUS;

struct uptp_info {
    int fd;
    int status;
    pthread_t listemThreadId;
    int bListemThreadExit;
    EventHandler_Callback cbEventHandler;
    void* eventHandlerUserData;
};

static struct uptp_info g_stInfo = {
    -1,
    UPTP_STATUS_UNINIT,
    0,
    0,
    NULL,
    NULL
};

static int g_seq = 0;

struct zw_uptp_list g_list;

static void* fxn_listen_thread(void *arg)
{
    pthread_detach(pthread_self());

    info("[%s: %d]\n", __FUNCTION__, __LINE__);
    struct uptp_info* pInfo = (struct uptp_info*)arg;
    struct pollfd read_poll;
    int status;
    struct zw_uptp_message msg;
    struct elNode *node = NULL;

    read_poll.fd = pInfo->fd;
    read_poll.events = POLLIN;

    while (!pInfo->bListemThreadExit)
    {
        info("[%s: %d]\n", __FUNCTION__, __LINE__);
        status = poll(&read_poll, 1, -1);
		if (status == 0) {
			err("interrupt\n");
			continue;
		}
		if (status < 0) {
			err("poll return err:%d\n", status);
			continue;
        }

        if (read_poll.revents & POLLIN) {
            memset(&msg, 0, sizeof(struct zw_uptp_message));
            zw_uptp_recive(pInfo->fd, &msg);
            if (MSG_TYPE_RESPONSE == msg.msgType)
            {
                msg.data[msg.dataLen] = '\0';
                info("Seq[%d] MsgType[%s] DataLen[%d] Data[%p]\n", msg.seq, getMsgTypeName(msg.msgType), msg.dataLen, msg.data);
                // 放入缓冲队列
                node = (struct elNode *)malloc(sizeof(struct elNode));
                memset(node, 0, sizeof(struct elNode));
                node->seq = msg.seq;
                node->data = malloc(msg.dataLen);
                memcpy(node->data, msg.data, msg.dataLen);
                node->dataLen = msg.dataLen;
                zw_uptp_list_push(&g_list, node);
            }
            else if (MSG_TYPE_REQUEST == msg.msgType)
            {
                if (pInfo->cbEventHandler != NULL)
                {
                    pInfo->cbEventHandler(&msg, pInfo->eventHandlerUserData);
                }
            }
            else
            {
                warn("Unknow Message Type[%d]\n", msg.msgType);
            }
            
        }
        sleep(1);
    }

    return NULL;
}

// uptp协议初始化
int zw_usbpipe_uptp_init()
{
    info("[%s %d]\n", __FUNCTION__, __LINE__);
    
    g_stInfo.fd = as_usbpipe_open();
    if (-1 == g_stInfo.fd)
    {
        err("[%s %d]usbpipe open failed!\n", __FUNCTION__, __LINE__);
        return -1;
    }
    g_stInfo.status = UPTP_STATUS_INITED;

    LIST_INIT(&g_list);
    pthread_mutex_init(&g_list.mtxLock, NULL);

    pthread_create(&g_stInfo.listemThreadId, NULL, fxn_listen_thread, (void *)&g_stInfo);

    return 0;
}

int zw_usbpipe_uptp_bind_opt(EventHandler_Callback cb, void* user_data)
{
    if (g_stInfo.cbEventHandler != NULL)
    {
        warn("EventHandler_Callback had binded! Please Checkout Your Operator!\n");
        return -1;
    }
    g_stInfo.cbEventHandler = cb;
    g_stInfo.eventHandlerUserData = user_data;

    return 0;
}

// 异步调用接口
int zw_usbpipe_uptp_call_async(const void* req, size_t reqLen, Response_Callback cb)
{
    info("[%s %d]\n", __FUNCTION__, __LINE__);

    if (g_stInfo.status != UPTP_STATUS_INITED)
    {
        err("[%s %d]untp process status errno[%d]!\n", __FUNCTION__, __LINE__, g_stInfo.status);
        return -1;        
    }
    
    return 0;
}

// 同步接口
int zw_usbpipe_uptp_call_sync(void* req, size_t reqLen, void** response, size_t* resLen)
{
    info("[%s %d]\n", __FUNCTION__, __LINE__);

    if (g_stInfo.status != UPTP_STATUS_INITED)
    {
        err("[%s %d]untp process status errno[%d]!\n", __FUNCTION__, __LINE__, g_stInfo.status);
        return -1;        
    }

    int ret = -1;
    struct zw_uptp_message msg;
    struct elNode *node = NULL;
    int try_count = 20;

    msg.seq = ++g_seq;
    memcpy(msg.data, req, reqLen);
    msg.dataLen = reqLen;
    zw_uptp_send(g_stInfo.fd, &msg);
    while (try_count > 0)
    {
        node = zw_uptp_list_pop_by_seq(&g_list, msg.seq);
        if (node)
        {
            *response = malloc(node->dataLen);
            memcpy((void*)*response, node->data, node->dataLen);
            *resLen = node->dataLen;
            free(node->data);
            free(node);
            ret = 0;
            break;
        }
        else
        {
            usleep(250*1000);
            try_count--;
            warn("waiting response! [total:cur][%d:%d]\n", 20, 20 - try_count);
        }
    }

    return ret;
}

int zw_usbpipe_uptp_write(struct zw_uptp_message* msg)
{
    info("[%s %d]\n", __FUNCTION__, __LINE__);

    zw_uptp_send(g_stInfo.fd, msg);

    return 0;
}

// uptp协议反初始化
int zw_usbpipe_uptp_uninit()
{
    info("[%s %d]\n", __FUNCTION__, __LINE__);

    g_stInfo.bListemThreadExit = 1;

    if (g_stInfo.fd != -1)
    {
        as_usbpipe_close(g_stInfo.fd);
        g_stInfo.fd = -1;
    }
    g_stInfo.status = UPTP_STATUS_DESTROYED;

    zw_uptp_list_destroy(&g_list);

    return 0;
}
