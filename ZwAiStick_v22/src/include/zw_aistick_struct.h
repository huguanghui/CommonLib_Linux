#ifndef __ZW_AI_STICK_STRUCT_H__
#define __ZW_AI_STICK_STRUCT_H__

/** @brief   AI Stick相关的结构体定义
 *  @file    zw_aistick_struct.h
 *  @author  huguanghui
 *  @version v1.0.0
 *  @date    2019/06/20
 *  @note    
 *  @since
 */

/************************************************* 人脸相关 ****************************************************/


/************************************************* 脸库相关 ****************************************************/

/************************************************* Record相关 ****************************************************/

typedef struct
{
	int width;
	int height;
}FaceRecoCapRect_t;

typedef struct
{
	FaceRecoCapRect_t max;
	FaceRecoCapRect_t min;
}FaceRecoCapSnapShot_t;

typedef struct
{
	int bSupportFaceReco;
	int nMaxFaceGroupNum;
	int nMaxPersonNumPerGroup;
	int nMaxPersonNumTotal;
	int nMaxImageSize;
	FaceRecoCapSnapShot_t snapShot;
}FaceRecoCap_t;

#endif
