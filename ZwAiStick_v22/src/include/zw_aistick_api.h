#ifndef __ZW_AISTICK_API_H__
#define __ZW_AISTICK_API_H__

#include "zw_aistick_struct.h"
#include "PrivateXMLCfg.h"
#include "faceRule.h"
#include "tagFaceGroup.h"
#include "AVL/tagEncCfg.h"

/** @brief   AI Stick相关的API定义
 *  @file    zw_aistick_api.h
 *  @author  huguanghui
 *  @version v1.0.0
 *  @date    2019/06/20
 *  @note    与智能棒交互相关接口定义
 *  @since
 */

#ifdef __cplusplus
extern "C" {
#endif

/************************************************* 系统相关 ****************************************************/

/**  查询人脸识别模块的能力集
 @param info[in|out]  脸库信息
 @return        0 - 成功 非0 - 失败
 @warning
 @note
 @see
*/
int AS_FaceReco_GetCap(FaceRecoCap_t *cap);

/**  查询人脸识别模块的工作状态
 @param state[out]  状态
 @param errCode[out]  错误码
 @return        0 - 成功 非0 - 失败
 @warning
 @note
 @see
*/
int AS_FaceReco_GetWorkState(int *state, int *errCode);


/************************************************* 脸库相关 ****************************************************/

/**  创建人脸库
 @param info[in|out]  脸库信息
 @return        0 - 成功 非0 - 失败
 @warning
 @note
 @see
*/
int AS_FaceReco_GroupCreate(FaceGroupInfo_t *info);

/**  修改人脸库
 @param info[in]  脸库信息
 @return        0 - 成功 非0 - 失败
 @warning
 @note
 @see
*/
int AS_FaceReco_GroupModify(FaceGroupInfo_t *info);

/**  删除人脸库
 @param groupId[in]  脸库ID
 @param bDelBindFace[in]  删除绑定人脸的标识
 @return        0 - 成功 非0 - 失败
 @warning
 @note
 @see
*/
int AS_FaceReco_GroupDelete(unsigned long long groupId, int bDelBindFace);

/**  查询人脸库-全部
 @param pFaceGroups[out]  脸库库信息数组
 @param pCount[out]  数组个数
 @return        0 - 成功 非0 - 失败
 @warning        pFaceGroup的由外部分配
 @note
 @see
*/
int AS_FaceReco_GroupQueryAll(FaceGroupInfo_t *pFaceGroups,int *pCount);

/**  查询人脸库-ID
 @param pFaceGroup[in|out]  脸库库信息
 @return        0 - 成功 非0 - 失败
 @warning        pFaceGroup的由外部分配
 @note
 @see
*/
int AS_FaceReco_GroupQueryByGroupID(FaceGroupInfo_t *pFaceGroup);

/************************************************* 人脸相关 ****************************************************/

/**  检验人员信息是否合法
 @param pFaceGroup[in|out]  脸库库信息
 @return        0 - 成功 非0 - 失败
 @warning        pFaceGroup的由外部分配
 @note
 @see
*/
int AS_FaceReco_PersonInfoCheck(FaceGroupFaceInfo_t *pInfo);

/**  人员信息新增 - 基本信息 + 图片
 @param faceGroupId[in]  脸库ID
 @param faceImageID[in]  图片ID(一个人员信息可以保存多张图片)
 @param pInfo[in]  人脸信息
 @param ifIgnoreError[in]  忽略错误的标识
 @param PicBuf[in]  图片buf
 @param bufLen[in]  图片buf长度
 @return        0 - 成功 非0 - 失败
 @warning        pFaceGroup的由外部分配
 @note
 @see
*/
int AS_FaceReco_PersonInfoCreate(unsigned long long faceGroupId, int faceImageID, FaceGroupFaceInfo_t *pInfo, int ifIgnoreError, char *PicBuf,int bufLen);

/**  人员信息删除
 @param faceGroupId[in]  脸库ID
 @param faceId[in]  人脸ID
 @param bIsDelPicture[in]  删除图片标识
 @return        0 - 成功 非0 - 失败
 @warning        pFaceGroup的由外部分配
 @note
 @see
*/
int AS_FaceReco_PersonInfoDelete(long long faceGroupId, long long faceId, int bIsDelPicture);

/**  人员信息解绑
 @param faceGroupId[in]  脸库ID
 @param faceId[in]  人脸ID
 @return        0 - 成功 非0 - 失败
 @warning        pFaceGroup的由外部分配
 @note
 @see
*/
int  AS_FaceReco_PersonInfoUnbind(long long faceGroupId, long long faceId);

/**  人员信息绑定
 @param faceGroupId[in]  脸库ID
 @param faceId[in]  人脸ID
 @return        0 - 成功 非0 - 失败
 @warning        pFaceGroup的由外部分配
 @note
 @see
*/
int AS_FaceReco_PersonInfoBind(long long faceGroupId, long long faceId);

/**  人员信息修改 - 基本信息
 @param pInfo[in]  人脸信息
 @return        0 - 成功 非0 - 失败
 @warning        pFaceGroup的由外部分配
 @note
 @see
*/
int AS_FaceReco_PersonInfoModifyBasic(FaceGroupFaceInfo_t *pInfo);

/**  人员信息修改 - 图片
 @param faceId[in]  人脸ID
 @param faceImageID[in]  图片ID(一个人员信息可以保存多张图片)
 @param PicBuf[in]  图片buf
 @param bufLen[in]  图片buf长度
 @return        0 - 成功 非0 - 失败
 @warning        pFaceGroup的由外部分配
 @note
 @see
*/
int AS_FaceReco_PersonInfoModifyPicture(long long faceId, int faceImageID, char *PicBuf, int bufLen);

/**  人员信息查询 - 数量
 @param faceGroupId[in]  脸库ID
 @param faceGroupSearchInfo[in] 过滤信息
 @param pFaceCount[out]  查询到的数目
 @return        0 - 成功 非0 - 失败
 @warning        pFaceGroup的由外部分配
 @note
 @see
*/
int AS_FaceReco_PersonInfoQueryListCount(long long faceGroupId, FaceGroupSearchInfo_t *faceGroupSearchInfo, int *pFaceCount);

/**  人员信息查询 - 基本信息
 @param faceGroupId[in]  脸库ID
 @param filter[in] 过滤信息
 @param pageNum[in] 指定每页的大小
 @param pageSize[out] 结果页数
 @param pFaceInfo[out] 信息
 @return        0 - 成功 非0 - 失败
 @warning        pFaceGroup的由外部分配
 @note
 @see
*/
int AS_FaceReco_PersonInfoQueryListBasic(long long faceGroupId, FaceGroupSearchInfo_t *filter, int pageNum, int *pageSize, FaceGroupFaceInfo_t *pFaceInfo);

/**  人员信息查询 - 图片
 @param personId[in]  人脸ID
 @param faceImageID[in] 图片ID(一个人员信息可以保存多张图片)
 @param buf[in] 保存图片的buf
 @param bufLen[in|out] 传入buf的长度, 传出图片的大小
 @return        0 - 成功 非0 - 失败
 @warning        pFaceGroup的由外部分配
 @note
 @see
*/
int AS_FaceReco_PersonInfoQueryPictureById(long long personId, int faceImageID, char *buf, int *bufLen);

/**  人员信息查询 - PersonID
 @param faceGroupId[in] 脸库ID
 @param personId[in]  人脸ID
 @param pFaceInfo[out] 人脸信息
 @return        0 - 成功 非0 - 失败
 @warning        pFaceGroup的由外部分配
 @note
 @see
*/
int AS_FaceReco_PersonInfoQueryBasicById(long long faceGroupId, long long personId, FaceGroupFaceInfo_t *pFaceInfo);

/**  发送流 - 按帧发送
 @param faceGroupId[in] 脸库ID
 @return        0 - 成功 非0 - 失败
 @warning        pFaceGroup的由外部分配
 @note
 @see
*/
int AS_FaceReco_SendFrame(void *buf, int len);

/**  检测智能棒是否可操作
 @param status[out] 返回状态
 @return	0 - 成功 非0 - 失败
 @warning
 @note
 @see
*/
int AS_FaceReco_Check_Online(int *status);

/**  规则 - 获取
 @param rule[out] 获取规则
 @return        0 - 成功 非0 - 失败
 @warning        pFaceGroup的由外部分配
 @note
 @see
*/
int AS_FaceGroup_Rule_Get(FACEPRO_RULE_CFG *rule);

/**  规则 - 设置
 @param rule[in] 设置的规则
 @return        0 - 成功 非0 - 失败
 @warning        pFaceGroup的由外部分配
 @note
 @see
*/
int AS_FaceGroup_Rule_Set(const FACEPRO_RULE_CFG *rule);

/**  系统配置 - 获取
 @param t[out] 当前时间
 @return        0 - 成功 非0 - 失败
 @warning
 @note
 @see
*/
int AS_System_Time_Get(time_t *t);

/**  系统配置 - 对时
 @param t[in] 设置的当前时间
 @return        0 - 成功 非0 - 失败
 @warning
 @note
 @see
*/
int AS_System_Time_Set(time_t *t);

/**  系统配置 - 获取版本信息
 @param info[out] 获取的版本信息
 @return        0 - 成功 非0 - 失败
 @warning
 @note
 @see
*/
int AS_System_DevInfo_Get(int *info);

/** 抓拍规则 - 最小和最大人脸 - 获取
 @param f[out] 范围
 @return        0 - 成功 非0 - 失败
 @warning
 @note
 @see
*/
int AS_FaceSnap_Filter_Get(int ch, FaceDetectRegion *f);

/** 抓拍规则 - 最小和最大人脸 - 设置
 @param f[in] 范围
 @return        0 - 成功 非0 - 失败
 @warning
 @note
 @see
*/
int AS_FaceSnap_Filter_Set(int ch, const FaceDetectRegion *f);

/** 抓拍规则 - 抓怕人脸区域 - 获取
 @param f[out] 人脸区域
 @return        0 - 成功 非0 - 失败
 @warning
 @note
 @see
*/
int AS_FaceSnap_Region_Get(int ch, FilerFaceDetectRegionParam *f);

/** 抓拍规则 - 抓怕人脸区域 - 设置
 @param f[in] 人脸区域
 @return        0 - 成功 非0 - 失败
 @warning
 @note
 @see
*/
int AS_FaceSnap_Region_Set(int ch, const FilerFaceDetectRegionParam *f);

/** 抓拍规则 - 抓怕模式 - 获取
 @param f[out] 抓拍模式信息
 @return        0 - 成功 非0 - 失败
 @warning
 @note
 @see
*/
int AS_FaceSnap_Config_Get(int ch, FaceConfig *f);

/** 抓拍规则 - 抓怕模式 - 设置
 @param channel[in] 通道号(0开始)
 @param f[in] 抓拍模式信息
 @return        0 - 成功 非0 - 失败
 @warning
 @note
 @see
*/
int AS_FaceSnap_Config_Set(int ch, const FaceConfig *f);

#ifdef __cplusplus
}
#endif

#endif
