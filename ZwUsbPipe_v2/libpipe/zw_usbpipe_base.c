#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

#include "zw_usbpipe_base.h"

#define USBPIPE_FILE_NAME	"/dev/usbpipe0"
#define USBPIPE_TEST_FILE_NAME "/tmp/usbpipe0"

#define IOCTL_OPEN_USBPIPE      0x1
#define IOCTL_CLOSE_USBPIPE     0x2
#define IOCTL_STREAM_ON         0x3
#define IOCTL_STREAM_OFF        0x4
#define IOCTL_QUEUE_PHY_BUFF    0x5
#define IOCTL_DEQUEUE_RESULT    0x6


struct usbpipe_ioctl_arg {
	unsigned long addr;
	unsigned int len;
};

int as_usbpipe_open()
{
	info("[%s %d]\n", __FUNCTION__, __LINE__);
	int fd;
#ifdef PIPE_TEST
    if(access(USBPIPE_TEST_FILE_NAME,F_OK)==-1)
    {
        int res = mkfifo(USBPIPE_TEST_FILE_NAME, 0777);
        if(res!=0)
        {
            err("[%s %d]could not create fifo\n", __FUNCTION__, __LINE__);
            return -1;
        }
    }
	
	fd = open(USBPIPE_TEST_FILE_NAME, O_RDWR);
#else
   	fd = open(USBPIPE_FILE_NAME, O_RDWR);
    if (-1 == fd) 
    {
       err("[%s %d]\n", __FUNCTION__, __LINE__);
       return -1;
    }

    int rc = ioctl(fd, IOCTL_OPEN_USBPIPE, 0);
	if (rc) {
		err("ioctl error cmd:%d return val:%d\n", IOCTL_OPEN_USBPIPE, rc);
		close(fd);
        fd = -1;
        return -1;
	}
    rc = ioctl(fd, IOCTL_STREAM_ON, 0);
	if (rc) {
		err("ioctl error cmd:%d return val:%d\n", IOCTL_STREAM_ON, rc);
		close(fd);
        fd = -1;
        return -1;
	}
#endif

    return fd;
}

int as_usbpipe_send(int fd, char* buf, int len)
{
    info("[%s %d]\n", __FUNCTION__, __LINE__);
#ifdef PIPE_TEST
	write(fd, buf, len);
#else
	int status;
	struct usbpipe_ioctl_arg data;

	if (NULL == buf || len <= 0)
	{
		return -1;
	}

	data.addr = (unsigned long)buf;
	data.len = len;
	status = ioctl(g_USBPipeFd, IOCTL_QUEUE_PHY_BUFF, &data);
	if (status) {
		debug("sending ioctl failed status:%d phy:%x, len:%d\n",
				status, data.addr, data.len);
		return -1;
	}
#endif

    return 0;
}

int as_usbpipe_recv(int fd, char** buf, int* len)
{
    info("[%s %d]\n", __FUNCTION__, __LINE__);
#ifdef PIPE_TEST
	char *buffer = NULL;
	int bufLen = 0;
	int readLen = 0;

	buffer = (char*)malloc(USBPIPE_MAXSIZE);
	bufLen = USBPIPE_MAXSIZE;
	
	readLen = read(fd, buffer, bufLen);

	*buf = buffer;
	*len = readLen;
#else
	int status;
	struct usbpipe_ioctl_arg result;

	result.addr =  (unsigned long)malloc(USBPIPE_MAXSIZE);
	result.len = USBPIPE_MAXSIZE;

	status = ioctl(fd, IOCTL_DEQUEUE_RESULT, &result);
	if (status) {
		debug("receiving data failed\n");
		free((void*)result.addr);
		return -1;
	}
#endif

    return 0;
}

int as_usbpipe_close(int fd)
{
    info("[%s %d]\n", __FUNCTION__, __LINE__);
#ifdef PIPE_TEST
	close(fd);
	fd = -1;
#else
    int rc = ioctl(fd, IOCTL_STREAM_OFF, 0);
	if (rc) {
		err("ioctl error cmd:%d return val:%d\n", IOCTL_STREAM_OFF, rc);
		close(fd);
        fd = -1;
        return -1;
	}
    rc = ioctl(fd, IOCTL_CLOSE_USBPIPE, 0);
	if (rc) {
		err("ioctl error cmd:%d return val:%d\n", IOCTL_CLOSE_USBPIPE, rc);
		close(fd);
        fd = -1;
        return -1;
	}
#endif

    return 0;
}
