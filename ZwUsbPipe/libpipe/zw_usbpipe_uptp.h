#ifndef __ZW_UTP_H__
#define __ZW_UTP_H__

#include "zw_usbpipe_util.h"

// 封装发送消息的接口
int zw_uptp_send(int fd, struct zw_uptp_message* msg);

// 封装接收消息的接口
int zw_uptp_recive(int fd, struct zw_uptp_message* msg);

#endif
