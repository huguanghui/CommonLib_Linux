#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>
#include <unistd.h>

#define HGH_DBG(fmt, args...) printf("\033[40;33m HGH_DBG(%s %s %d):\t\033[0m"fmt, __FILE__, __func__, __LINE__, ## args)

int main(int argc, char const *argv[])
{
    int fd;
    int clifd;
    struct sockaddr_un un;
    socklen_t len;

    // 1.创建套接字
    fd = socket(AF_UNIX, SOCK_STREAM, 0);

    // 2.connect
    un.sun_family = AF_UNIX;
    snprintf(un.sun_path, sizeof(un.sun_path), "/tmp/unix.tmp.1");
    len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
    clifd = connect(fd, (struct sockaddr *)&un, len);
    // 3.write
    {
        ssize_t writeLen;
        char buf[1024];
        
        snprintf(buf, sizeof(buf), "Hello World!");
        writeLen = write(clifd, buf, strlen(buf));
    }
    // 4.close
    close(fd);

    return 0;
}
