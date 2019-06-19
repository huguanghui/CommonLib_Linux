#ifndef __ZW_USB_PIPE_UPTP_API_H__
#define __ZW_USB_PIPE_UPTP_API_H__

#include "zw_usbpipe_util.h"

// uptp协议初始化
int zw_usbpipe_uptp_init();

// 监听请求的回调
typedef void (*EventHandler_Callback)(void* data, int len, void* user_data);
int zw_usbpipe_uptp_bind_opt(EventHandler_Callback cb, void* user_data);

// 异步的回调接口
typedef void (*Response_Callback)(char *result, int len);
// 异步调用接口
int zw_usbpipe_uptp_call_async(const void* req, size_t reqLen, Response_Callback cb);

// 同步接口
int zw_usbpipe_uptp_call_sync(const void* req, size_t reqLen, const void** response, size_t* resLen);

// uptp协议反初始化
int zw_usbpipe_uptp_uninit();

#endif
