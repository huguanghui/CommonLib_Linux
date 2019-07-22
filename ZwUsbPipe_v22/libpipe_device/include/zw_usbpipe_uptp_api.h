#ifndef __ZW_USB_PIPE_UPTP_API_H__
#define __ZW_USB_PIPE_UPTP_API_H__

#include "zw_usbpipe_util.h"

#if defined(__cplusplus)
extern "C" {
#endif

//!\brief 请求数据
typedef struct
{
	int cmd;  // 操作类型
    int paramLen;
    void* param;
} AS_TASK_REQUEST;

//!\brief 响应数据
typedef struct
{
    void* param;
    int paramLen;
} AS_TASK_RESULT;

typedef enum
{
	//!< 抓拍任务
	AS_TASK_CAPTURE_FACE = 0,
	//!< 抽取特征
	AS_TASK_EXTRACT_FEATURE,
	//!< 抓拍并抽取特征
	AS_TASK_CAPTURE_EXTRACT_FEATURE,
	//!< 抓拍并比对
	AS_TASK_CAPTURE_SEARCH,
	//!< 按图片比对
	AS_TASK_SEARCH_BY_JPG,
	//!< 按特征比对
	AS_TASK_SEARCH_BY_FEATURE,
	//!< HTTP配置
	AS_TASK_HTTP_CONFIG,
	//!< 比对流
	AS_TASK_STREAM_FRAME,
	//!< 比对事件上报
	AS_TASK_EVENT_REPORT,
	//!< 智能数据上报
	AS_TASK_SMART_DATA,
	//!< 检测设备是否可操作
	AS_TASK_CHECK_ONLINE,
} AS_TASK_TYPE;

typedef struct
{
	int imageType;  // 0 - ZWSDK_IMAGE_YUV420SP 1 - ZWSDK_IMAGE_JPEG
	int nChannelNo;
	unsigned long long nFrameNo;
	unsigned int nWidth;
	unsigned int nHeight;
	unsigned int nDataLen;
}__attribute__((packed)) AiStickFrameHeader_t;

typedef struct
{
	unsigned long dwEventType; // 主事件类型
	unsigned long dwSubEventType; // 子事件类型
	unsigned long dwAlertType; // alerttype 
	unsigned long dwChannel; // 通道号
	unsigned long dwReserve; // 保留
	unsigned long dwDataLen; //事件数据长度
}__attribute__((packed)) AiStickEventHeadr_t;

// uptp协议初始化
int zw_usbpipe_uptp_init();

// 监听请求的回调
typedef void (*EventHandler_Callback)(struct zw_uptp_message* msg, void* user_data);
int zw_usbpipe_uptp_bind_opt(EventHandler_Callback cb, void* user_data);

// 发送/响应
int zw_usbpipe_uptp_write(struct zw_uptp_message* msg);

// uptp协议反初始化
int zw_usbpipe_uptp_uninit();

#if defined(__cplusplus)
}
#endif

#endif
