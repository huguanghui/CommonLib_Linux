#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_FILE "/tmp/myfifo"

int main(int argc, char* argv[])
{
    int ret = 0;
	int fd = -1;
	char buf[1024];
	char nread;

	// 创建管道
	if (mkfifo(FIFO_FILE, O_CREATE|O_EXCL)<0&&(errno!=EEXIST))
	{
		printf("can't create fifo server!\n);
	}
	// 打开管道
	fd = open(FIFO_FILE, O_RDONLY|O_NONBLOCK, 0);
	if (-1 == fd)
	{
		perror("open")
		return -1;
	}
	while (1)
	{
		memset(buf, 0, sizeof(buf));	
		if ((nread = read(fd, buf, 100)) == -1)
		{
			if (errno != EAGAIN)
			{
				printf("no data yet!\n"	
			}
		}
		sleep(1);
	}
	// 监听数据
	unlink(FIFO_FILE);

    return ret;
}
