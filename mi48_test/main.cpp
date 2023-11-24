#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int fd = open("/dev/ttyACM0", O_RDWR);
    if (fd < 0) {
        printf("Fail to Open UART device\n");
        return -1;
    }
    tcflush(fd, TCIOFLUSH);

    struct termios opt;
    tcgetattr(fd, &opt);

    cfsetospeed(&opt, B115200);
    cfsetispeed(&opt, B115200);

    opt.c_cflag &= ~CSIZE;
    opt.c_cflag |= CS8;
    opt.c_cflag &= ~CSTOPB;
    opt.c_cflag &= ~PARENB;
    opt.c_iflag &= ~INPCK;

    opt.c_cflag &= ~CRTSCTS;
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* Input  */
    opt.c_oflag &= ~OPOST; /* Output */
    opt.c_iflag &= ~(INLCR | ICRNL | IGNCR | IXON);

    tcsetattr(fd, TCSANOW, &opt);
    const char *send_buf = "   #000CWREGB102XXXX";
    printf("1\n");
    write(fd, send_buf, strlen(send_buf));
    printf("2\n");
    sleep(1);
    char rx_buff[1024];
    while (1) {
        fd_set set;
        struct timeval timeout;
        int rv;
        FD_ZERO(&set); /* clear the set */
        FD_SET(fd, &set); /* add our file descriptor to the set */
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        rv = select(fd + 1, &set, NULL, NULL, &timeout);
        if (rv == -1) {
            printf("select err\n");
            continue;
        } else if (rv == 0) {
            printf("timeout err\n");
            continue;
        } else {
            int len = read(
                fd, rx_buff, sizeof(rx_buff)); /* there was data to read */
            printf("read ....len=%d\n", len);
        }
    }
    return 0;
}
