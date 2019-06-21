#ifndef __ZW_AI_STICK_SYS_H__
#define __ZW_AI_STICK_SYS_H__

#include "zw_aistick_util.h"

//!\brief 智能棒初始化
// 初始化智能棒
// 启动智能棒
// 智能棒认证，激活
// 时间同步
// 启动心跳
// 启动同步消息传输
// 启动异步消息传输
// 启动异步消息监听
// 获取能力
//!\return   0 成功；-1 失败
AS_API AS_S32 AS_SYS_Init(AS_SYS_INFO* info);

//!\brief   关闭智能棒，释放应用程序相关资源
//!\return   成功为0，否则为错误码
AS_API AS_S32 AS_SYS_UnInit();

//!\brief   获取智能棒状态
//!\return  成功为0，否则为错误码
AS_API AS_S32 AS_SYS_GetState();

//!\brief 从文件升级
//!\param pstTaskResult 输入参数,任务执行结果
//!\return 0表示执行成功, 其他表示错误码
//AS_API AS_S32 AS_SYS_Upgrade(AS_S8 *pszPackageFile, AS_UPGRADE_CALLBACK cbUpgrade);

//!\brief 从内存升级，内存中需包含完整的升级包
//!\param pstTaskResult 输入参数,任务执行结果
//!\return 0表示执行成功, 其他表示错误码
// AS_API AS_S32 AS_SYS_UpgradeFromMemory(AS_U8 *pPackageBuffer, AS_S32 s32BufLength, AS_UPGRADE_CALLBACK cbUpgrade);


#endif