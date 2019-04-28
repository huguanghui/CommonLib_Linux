#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stddef.h>

#define HGH_DBG(fmt, args...) printf("\033[40;33m HGH_DBG(%s %s %d):\t\033[0m"fmt, __FILE__, __func__, __LINE__, ## args)

#define QLEN (10)

int main(int argc, char const *argv[])
{
    int fd;
    int listenfd;
    socklen_t size;
    struct sockaddr_un un;

    // 1.创建套接字
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    un.sun_family = AF_UNIX;
    snprintf(un.sun_path, sizeof(un.sun_path), "/tmp/unix.tmp.1");

    size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
    bind(fd, (struct sockaddr *)&un, size);

    listenfd = listen(fd, QLEN);

    while (1)
    {
        struct sockaddr_un uncli;
        socklen_t len;
        int clifd;

        len = sizeof(uncli);
        if (clifd = accept(fd, (struct sockaddr *)&uncli, &len))
        {
            char buf[1024];
            ssize_t readlen;
            readlen = read(clifd, buf, sizeof(buf));
            buf[readlen] = '\0';
            HGH_DBG("Read[%ld]:%s\n", readlen, buf);
        }
        close(clifd);
    }

    close(listenfd);
    close(fd);

    return 0;
}
