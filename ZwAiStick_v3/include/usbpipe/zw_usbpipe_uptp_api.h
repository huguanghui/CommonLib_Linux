#ifndef __ZW_USB_PIPE_UPTP_API_H__
#define __ZW_USB_PIPE_UPTP_API_H__

#include "zw_usbpipe_util.h"

#if defined(__cplusplus)
extern "C" {
#endif

// uptp协议初始化
int zw_usbpipe_uptp_init();

// 监听请求的回调
typedef void (*EventHandler_Callback)(struct zw_uptp_message* msg, void* user_data);
int zw_usbpipe_uptp_bind_opt(EventHandler_Callback cb, void* user_data);

// 发送/响应
int zw_usbpipe_uptp_write(struct zw_uptp_message* msg);

// 接收
int zw_usbpipe_uptp_read(struct zw_uptp_message* msg);

// uptp协议反初始化
int zw_usbpipe_uptp_uninit();

#if defined(__cplusplus)
}
#endif

#endif
