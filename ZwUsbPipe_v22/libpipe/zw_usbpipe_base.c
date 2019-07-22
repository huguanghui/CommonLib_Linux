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

#include "mpi_sys.h"
#include "zw_usbpipe_base.h"

#define USBPIPE_FILE_NAME_HOST	"/dev/usbpipe0"

// cat /proc/media-mem 查看 PIPE 信息
#define PIPE_HOST_PHYS_ADDR	0x87D00000
#define PIPE_HOST_PHYS_SIZE	(2*1024*1024)

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

char *g_vm = NULL;

int as_usbpipe_open()
{
	int fd;

	fd = open(USBPIPE_FILE_NAME_HOST, O_RDWR);
	if (-1 == fd) 
	{
		err("\n");
		return -1;
	}

	int rc = ioctl(fd, IOCTL_OPEN_USBPIPE, 0);
	if (rc) 
	{
		err("ioctl error cmd:%d return val:%d\n", IOCTL_OPEN_USBPIPE, rc);
		close(fd);
		fd = -1;
		return -1;
	}
	rc = ioctl(fd, IOCTL_STREAM_ON, 0);
	if (rc) 
	{
		err("ioctl error cmd:%d return val:%d\n", IOCTL_STREAM_ON, rc);
		close(fd);
		fd = -1;
		return -1;
	}
	g_vm = HI_MPI_SYS_Mmap(PIPE_HOST_PHYS_ADDR, PIPE_HOST_PHYS_SIZE);

	return fd;
}
static pthread_mutex_t g_write_mutex = PTHREAD_MUTEX_INITIALIZER;
int as_usbpipe_send(int fd, char *buf, int len)
{
	int status;
	int sendLen;
	struct usbpipe_ioctl_arg data;

	if (NULL == buf || len <= 0)
	{
		return -1;
	}
	if (NULL == g_vm)
	{
		err("g_vm[NULL]!\n");
		return -1;
	}
	info("g_vm[0x%p]\n", g_vm);
	pthread_mutex_lock(&g_write_mutex);
	sendLen = len <= PIPE_HOST_PHYS_SIZE?len : PIPE_HOST_PHYS_SIZE;
	memset(g_vm, 0, PIPE_HOST_PHYS_SIZE);
	memcpy(g_vm, buf, sendLen);
	data.addr = PIPE_HOST_PHYS_ADDR;
	data.len = sendLen;
	status = ioctl(fd, IOCTL_QUEUE_PHY_BUFF, &data);
	pthread_mutex_unlock(&g_write_mutex);
	if (status) {
		err("sending ioctl failed status:%d phy:%x, len:%d\n",
				status, data.addr, data.len);
		return -1;
	}
	return 0;
}

int as_usbpipe_recv(int fd, char *buf, int len)
{
	int status;
	struct usbpipe_ioctl_arg result;

	result.addr =  (unsigned long)buf;
	result.len = len;
	status = ioctl(fd, IOCTL_DEQUEUE_RESULT, &result);
	if (status) {
		err("receiving data failed\n");
		return -1;
	}
	return result.len;
}

int as_usbpipe_close(int fd)
{
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
	close(fd);
	return 0;
}
