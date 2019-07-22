#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "zw_usbpipe_base.h"

#define USBPIPE_FILE_NAME_GADGET "/dev/usbpipe"

int as_usbpipe_open()
{
	int fd;
	fd = open(USBPIPE_FILE_NAME_GADGET, O_RDWR);
	if (-1 == fd) 
	{
		err("\n");
		return -1;
	}
	return fd;
}

int as_usbpipe_send(int fd, char* buf, int len)
{
	int ret = 0;
	ret = write(fd, buf, len);
	return ret;
}

int as_usbpipe_recv(int fd, char* buf, int len)
{
	int ret = 0;
	ret = read(fd, buf, len);
	return ret;
}

int as_usbpipe_close(int fd)
{
	close(fd);
	return 0;
}
