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

static void* fxn_listen_thread(void *arg)
{
	pthread_detach(pthread_self());

	info("\n");
	struct uptp_info* pInfo = (struct uptp_info*)arg;
	struct pollfd read_poll;
	int status;
	struct zw_uptp_message *msg = NULL;
	char *msg_buf_start = (char *)malloc(USBPIPE_MAXSIZE);
	char *msg_buf_end = msg_buf_start + USBPIPE_MAXSIZE;
	int unread_msg_size = 0;
	char *read_head_ptr = msg_buf_start;
	char *read_tail_ptr = msg_buf_start;

	read_poll.fd = pInfo->fd;
	read_poll.events = POLLIN;
	read_poll.events |= POLLERR;

	while (!pInfo->bListemThreadExit)
	{
		status = poll(&read_poll, 1, -1);
		if (status == 0) {
			err("interrupt\n");
			continue;
		}
		if (status < 0) {
			err("poll return err:%d\n", status);
			continue;
		}
		if (read_poll.revents != 1)
		{
			err("read_poll.revents[%d]\n", read_poll.revents);
		}
		if (read_poll.revents & POLLIN) {
			int msg_size = 0;
			int readLen = 0;

			// 读取消息头
			readLen = as_usbpipe_recv(pInfo->fd, read_tail_ptr, msg_buf_end-read_tail_ptr);
			if (readLen > 0)
			{
				read_tail_ptr += readLen;
			}
			else
			{
				continue;
			}

			unread_msg_size = read_tail_ptr - read_head_ptr;
			// 缓冲区中消息的长度大于一个msg的长度
			// 获取缓冲区中第一个消息的长度
			if (unread_msg_size >= ZW_UPTP_MSG_HEAD_SIZE)
			{
				msg = (struct zw_uptp_message *)read_head_ptr;
				msg_size = msg->dataLen + ZW_UPTP_MSG_HEAD_SIZE;
			}
			else
			{
				// 缓冲区中消息的长度小于一个msg的长度
				// 继续读消息
				continue;
			}

			while (msg_size <= unread_msg_size)
			{
				// 回调一次消息
				if (pInfo->cbEventHandler != NULL)
				{
					pInfo->cbEventHandler(msg, pInfo->eventHandlerUserData);
				}
				read_head_ptr += msg_size;
				unread_msg_size = read_tail_ptr - read_head_ptr;
				// 没有读的消息没有达到消息头的长度
				if (unread_msg_size < ZW_UPTP_MSG_HEAD_SIZE)
				{
					break;
				}
				// 继续读下一个消息
				msg = (struct zw_uptp_message *)read_head_ptr;
				msg_size = msg->dataLen+16;
			}

			//有以下几种情况:
			//1、缓冲区的第一个消息不完整
			//   read_head_ptr == msg_buf_start && unread_msg_size != 0
			//   不需要memmove
			//2、消息完全读完 unread_msg_size = 0
			//   不需要memmove
			//3、读完了1个或几个消息，缓冲区最后剩余一个不完整的消息 
			//   read_head_ptr != msg_buf_start && unread_msg_size != 0
			//   需要memmove
			if ((unread_msg_size) && (read_head_ptr != msg_buf_start))
			{
				// 将剩余的消息移动到buf的开头
				memmove(msg_buf_start, read_head_ptr, unread_msg_size);
				// 移动读指针的位置
				read_head_ptr = msg_buf_start;
				read_tail_ptr = read_head_ptr + unread_msg_size;
			}
		}
	}

	return NULL;
}

// uptp协议初始化
int zw_usbpipe_uptp_init()
{
	g_stInfo.fd = as_usbpipe_open();
	if (-1 == g_stInfo.fd)
	{
		err("usbpipe open failed!\n");
		return -1;
	}
	g_stInfo.status = UPTP_STATUS_INITED;
	// 监听消息线程
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
	zw_uptp_send(g_stInfo.fd, msg);
	return 0;
}

// uptp协议反初始化
int zw_usbpipe_uptp_uninit()
{
	g_stInfo.bListemThreadExit = 1;

	if (g_stInfo.fd != -1)
	{
		as_usbpipe_close(g_stInfo.fd);
		g_stInfo.fd = -1;
	}
	g_stInfo.status = UPTP_STATUS_DESTROYED;
	return 0;
}
