#ifndef __USB_PIPE_H__
#define __USB_PIPE_H__

#include "zw_usbpipe_util.h"

#define PIPE_TEST

//!\brief 启动usb pipe,只能启动一次
//!\return 返回句柄
int as_usbpipe_open();

//!\brief 通过usb pipe发送信息
//!\param fd  文件句柄
//!\param buf 发送缓冲区
//!\param len 信息长度
//!\return 0表示成功，其他表示错误码
int as_usbpipe_send(int fd, char* buf, int len);

//!\brief 通过usb pipe接收
//!\param fd  文件句柄
//!\param buf 输出参数, 接收缓冲区指针
//!\param len 输出参数, 接收的数据长度
//!\return 0表示成功，其他表示错误码
int as_usbpipe_recv(int fd, char** buf, int* len);

//!\brief 关闭usb pipe,只能启动一次
//!\param fd  文件句柄
//!\return 0表示成功，其他表示错误码
int as_usbpipe_close(int fd);

#endif
