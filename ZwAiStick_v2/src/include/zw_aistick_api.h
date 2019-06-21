#ifndef __ZW_AISTICK_API_H__
#define __ZW_AISTICK_API_H__

#include "zw_aistick_struct.h"

/** @brief   AI Stick相关的API定义
 *  @file    zw_aistick_api.h
 *  @author  huguanghui
 *  @version v1.0.0
 *  @date    2019/06/20
 *  @note    与智能棒交互相关接口定义
 *  @since
 */

/************************************************* 脸库相关 ****************************************************/

/**  这里写这个函数是干什么用的
 @param i1[in]  输入参数1
 @param i2[in]  输入参数2
 @param o3[out] 输出参数1
 @return        返回值解释一下
 @warning       警告: 例如: 参数不能为空
 @note          注解
 @see           相当于是请参考xxoo函数之类的
*/

/**  查询所有脸库信息
 @param pFg[out]  脸库信息
 @param FgSize[out]  脸库个数
 @return        0 - 成功 非0 - 失败
 @warning       pFg不能为NULL, 由外部传入
 @note
 @see
*/
int AS_FaceGroup_GetAll(FaceGroupInfo_t* pFg, int* FgSize);

#endif