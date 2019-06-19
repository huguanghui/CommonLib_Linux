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
                msg.data[msg.dateLen] = '\0';
                info("Seq[%d] MsgType[%s] DataLen[%d] Data[%s]\n", msg.seq, getMsgTypeName(msg.msgType), msg.dateLen, msg.data);
                // 放入缓冲队列
                node = (struct elNode *)malloc(sizeof(struct elNode));
                memset(node, 0, sizeof(struct elNode));
                node->seq = msg.seq;
                node->data = malloc(msg.dateLen);
                memcpy(node->data, msg.data, msg.dateLen);
                node->dataLen = msg.dateLen;
                zw_uptp_list_push(&g_list, node);
            }
            else if (MSG_TYPE_REQUEST == msg.msgType)
            {
                if (pInfo->cbEventHandler != NULL)
                {
                    pInfo->cbEventHandler(msg.data, msg.dateLen, pInfo->eventHandlerUserData);
                }
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
int zw_usbpipe_uptp_call_sync(const void* req, size_t reqLen, const void** response, size_t* resLen)
{
    info("[%s %d]\n", __FUNCTION__, __LINE__);

    if (g_stInfo.status != UPTP_STATUS_INITED)
    {
        err("[%s %d]untp process status errno[%d]!\n", __FUNCTION__, __LINE__, g_stInfo.status);
        return -1;        
    }

    struct zw_uptp_message msg;
    struct elNode *node = NULL;

    msg.identify[0] = 'P';
    msg.identify[1] = 'I';
    msg.identify[2] = 'P';
    msg.identify[3] = 'E';
    
    msg.seq = ++g_seq;
    memcpy(msg.data, req, reqLen);
    msg.dateLen = reqLen;
    zw_uptp_send(g_stInfo.fd, &msg);
    while(1)
    {
        node = zw_uptp_list_pop_by_seq(&g_list, msg.seq);
        if (node)
        {
            *response = malloc(node->dataLen);
            memcpy((void*)*response, node->data, node->dataLen);
            *resLen = node->dataLen;
            free(node->data);
            free(node);

            char buf[1024];
            memcpy(buf, *response, *resLen);
            buf[*resLen] = '\0';
            info("Response[%s]\n", buf);

            break;
        }
    }
    // zw_uptp_recive(g_stInfo.fd, &msg);
    // info("Seq[%d] DataLen[%d]\n", msg.seq, msg.dateLen);
    // msg.data[msg.dateLen] = '\0';
    // info("Data[%s]\n", msg.data);
    // *response = malloc(msg.dateLen+1);
    // *resLen = msg.dateLen+1;
    // memcpy((void*)*response, msg.data, msg.dateLen);
    // ((char *)*response)[msg.dateLen] = '\0';

    return 0;
}

// uptp协议反初始化
int zw_usbpipe_uptp_uninit()
{
    info("[%s %d]\n", __FUNCTION__, __LINE__);

    if (g_stInfo.fd != -1)
    {
        as_usbpipe_close(g_stInfo.fd);
        g_stInfo.fd = -1;
    }
    g_stInfo.status = UPTP_STATUS_DESTROYED;

    zw_uptp_list_destroy(&g_list);

    return 0;
}
