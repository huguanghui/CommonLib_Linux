#include <endian.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>

#define USBPIPE_FILE_NAME	"/dev/usbpipe0"

#define IOCTL_OPEN_USBPIPE      0x1
#define IOCTL_CLOSE_USBPIPE     0x2
#define IOCTL_STREAM_ON         0x3
#define IOCTL_STREAM_OFF        0x4
#define IOCTL_SEND_PHY_BUFF    0x5
#define IOCTL_GET_RESULT    0x6

static int do_ioctl(int fd, int cmd, void *arg)
{
	int ret;

	ret = ioctl(fd, cmd, arg);
	if (ret) {
		close(fd);
		return -EINVAL;
	}

	return 0;
}

int main(int argc, char **argv)
{
	int fd;

	(void)argc;
	(void)argv;

	fd = open(USBPIPE_FILE_NAME, O_RDWR);

	/* create usbpipe */
	if (do_ioctl(fd, IOCTL_OPEN_USBPIPE, 0))
		return -EINVAL;

	if (do_ioctl(fd, IOCTL_STREAM_ON, 0))
		return -EINVAL;


	if (do_ioctl(fd, IOCTL_STREAM_OFF, 0))
		return -EINVAL;

	if (do_ioctl(fd, IOCTL_CLOSE_USBPIPE, 0))
		return -EINVAL;

	return 0;
}
