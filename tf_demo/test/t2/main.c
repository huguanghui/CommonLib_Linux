#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>

#include "TinyFrame.h"
#include "utils.h"

#define UART_DEV "/dev/ttyUSB0"
#define BUFFER_SIZE 64

int serial_port;
TinyFrame *demo_tf;

void TF_WriteImpl(TinyFrame *tf, const uint8_t *buff, uint32_t len)
{
    printf("--------------------\n");
    printf("\033[32mTF_WriteImpl - sending frame:\033[0m\n");

    uint8_t *xbuff = (uint8_t *)buff;

    // dumpFrame(xbuff, len);

    // Send it back as if we received it
    // TF_Accept(tf, xbuff, len);

    return;
}

void *readSerial(void *arg)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while (1) {
        bytes_read = read(serial_port, buffer, BUFFER_SIZE - 1);
            printf("[HGH-TEST][%s %d] bytes: %ld\n", __FUNCTION__, __LINE__, bytes_read);
        if (bytes_read > 0) {
            TF_Accept(demo_tf, (uint8_t *)buffer, bytes_read);
        }
    }

    return NULL;
}

TF_Result myListener(TinyFrame *tf, TF_Msg *msg)
{
    dumpFrameInfo(msg);
    return TF_STAY;
}

int main()
{
    struct termios tty;
    pthread_t thread;

    demo_tf = TF_Init(TF_MASTER);
    TF_AddGenericListener(demo_tf, myListener);

    // 打开串口设备文件
    serial_port = open(UART_DEV, O_RDWR);
    if (serial_port < 0) {
        perror("Error opening serial port");
        return 1;
    }

    // 配置串口参数
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(serial_port, &tty) != 0) {
        perror("Error getting serial port attributes");
        return 1;
    }

    // 设置波特率
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);

    // 设置数据位、停止位和校验位
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8; // 8位数据位
    tty.c_cflag &= ~PARENB; // 无校验位
    tty.c_cflag &= ~CSTOPB; // 1位停止位
    printf("[HGH-TEST][%s %d]\n", __FUNCTION__, __LINE__);

    // 应用设置
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        perror("Error setting serial port attributes");
        return 1;
    }

    printf("[HGH-TEST][%s %d]\n", __FUNCTION__, __LINE__);

    // 创建线程，监听串口数据
    if (pthread_create(&thread, NULL, readSerial, NULL) != 0) {
        perror("Error creating thread");
        return 1;
    }

    // 主线程可以执行其他操作
    while (1) {
        sleep(1);
    }

    // 等待线程结束
    pthread_join(thread, NULL);

    // 关闭串口
    close(serial_port);

    return 0;
}
