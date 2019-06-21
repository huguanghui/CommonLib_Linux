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
            info("[%s %s %d]\n", __FILE__, __FUNCTION__, __LINE__);
            if (pInfo->cbEventHandler != NULL)
            {
                pInfo->cbEventHandler(&msg, pInfo->eventHandlerUserData);
            }
        }
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

    // 防止自己接收自己发送的消息
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

int zw_usbpipe_uptp_write(struct zw_uptp_message* msg)
{
    info("[%s %d][write-seq]-[%d]\n", __FUNCTION__, __LINE__, msg->seq);

    zw_uptp_send(g_stInfo.fd, msg);

    return 0;
}

int zw_usbpipe_uptp_read(struct zw_uptp_message* msg)
{
    zw_uptp_recive(g_stInfo.fd, &msg);
    info("[%s %d][read-seq]-[%d]\n", __FUNCTION__, __LINE__, msg->seq);

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
