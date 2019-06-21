#ifndef __ZW_AI_STICK_TASK_H__
#define __ZW_AI_STICK_TASK_H__

#include "zw_aistick_util.h"

extern struct testQueue g_msgQueue;

extern int g_seq;

//!\brief 创建任务队列
//!\param phQueue 输出参数, 创建的队列句柄, AS_HANDLE_INVALID 表示非法句柄
//!\return 0表示创建成功, 其他表示错误码
AS_API AS_S32 AS_TASK_CreateQueue(AS_HANDLE* phQueue);

AS_API AS_S32 AS_TASK_TEST_CreateQueue(AS_HANDLE* phQueue);

AS_API AS_S32 AS_TASK_TEST_Add(AS_HANDLE hQueue, struct taskNode* pstTaskAttr);

//AS_API AS_S32 AS_TASK_CreateQueue(hQueue1, cbQueue1);

//!\brief 创建支持人脸跟踪的任务队列
//!       只对人脸抓拍、人脸抓拍比对等任务支持人脸跟踪
//!\param phQueue 输出参数, 创建的队列句柄, AS_HANDLE_INVALID 表示非法句柄
//!\return 0表示创建成功, 其他表示错误码
AS_API AS_S32 AS_TASK_CreateFaceTrackQueue(AS_HANDLE* phQueue);

//!\brief 销毁任务队列
//!\param phQueue 输入参数, 要销毁的队列句柄, AS_HANDLE_INVALID 表示非法句柄
//!\return 0表示执行成功, 其他表示错误码
AS_API AS_S32 AS_TASK_DestroyQueue(AS_HANDLE hQueue);

//!\brief 向任务队列中增加任务
//!\param phQueue 输入参数, 要销毁的队列句柄, AS_HANDLE_INVALID 表示非法句柄
//!\return 0表示执行成功, 其他表示错误码
AS_API AS_S32 AS_TASK_Add(AS_HANDLE hQueue, struct taskNode* pstTaskAttr);

//!\brief 同步执行任务，不增加到任务队列中
//!\param pstTaskAttr 输入参数, 任务相关参数
//!\param pstTaskResult 输出参数,执行结果
//!\return 0表示执行成功, 其他表示错误码
AS_API AS_S32 AS_TASK_Run(AS_TASK_REQUEST* pstTaskAttr, AS_TASK_RESULT* pstTaskResult);

#endif