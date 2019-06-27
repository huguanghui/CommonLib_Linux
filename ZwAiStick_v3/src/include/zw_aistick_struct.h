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

/*证件类型*/
typedef enum
{
	FACEPASSORD = 0,/*护照*/
	FACEIDENTITYCARD ,/*身份证*/
	FACECERTITYPEMAX,
}FACECARDTYPE_E;

/*人脸信息表结构*/
typedef struct strFaceInfo_t
{
	unsigned long long faceId;/*人脸id：UUID*/
	char name[32];/*姓名*/
	unsigned int gender;/*性别:男-0；女-1*/
	char birthday[16];/*生日：字符串存储*/
	char country[32];/*国家*/
	char provinceCity[48];/*省份-城市*/
	FACECARDTYPE_E cardType;/*证件类型*/
	char cardId[20];/*证件号:目前见到最长为身份证号18字节*/
	char facePicPath[80];/*原图路径*/
	char feature[2048];/*特征值*/
	int validLen;/*特征值有效长度*/
	char featureVersion[8];/*特征值版本:字节长度待定？*/
	unsigned int featrueState;/*建模状态：成功-0；失败-1*/
}FaceInfo_bak_t;

/************************************************* 脸库相关 ****************************************************/
#define MAX_FACE_GROUP_COUNT 10
#define UNBINDFACEGROUPID 1

/*人脸库中人脸操作类型*/
typedef enum
{
	FACEGROUPFACEADD = 0,/*添加人脸信息*/
	FACEGROUPFACEDELETE,/*删除人脸信息*/
	FACEGROUPFACECHANGE,/*修改人脸信息*/
	FACEGROUPFACEACTIONMAX,
}FACEGROUPFACEACTIONTYPE_E;

/*证件类型*/
typedef enum
{
	FACEGROUPPASSORD = 0,/*护照*/
	FACEGROUPIDENTITYCARD ,/*身份证*/
	FACEGROUPOTHERCARD,/*其他*/
	FACEGROUPCERTITYPEMAX,
}FACEGROUPCARDTYPE_E;

/*人脸缓存信息*/
typedef struct
{
	unsigned long long faceId;/*人脸id：数据库自动增长*/
	char name[32];/*姓名*/
	unsigned int gender;/*性别:男-0；女-1*/
	char birthday[16];/*生日:字符串存储*/
	char country[32];/*国家*/
	char provinceCity[48];/*省份-城市*/
	FACEGROUPCARDTYPE_E cardType;/*证件类型*/
	char cardId[20];/*证件号:目前见到最长为身份证号18字节*/
	unsigned int featrueState;/*建模状态：成功-0；失败-1*/
	int isValid;
}FaceInfo_t;

/*人脸限制查询时信息*/
typedef struct
{
	char name[32];/*姓名：按旷视-32字节*/
	unsigned int gender;/*性别:男-0；女-1*/
	char startBirthday[16];/*生日：最小年龄*/
	char endBirthday[16];/*生日：最大年龄*/
	char country[32];/*国家*/
	char provinceCity[48];/*省份-城市*/
	FACEGROUPCARDTYPE_E cardType;/*证件类型*/
	char cardId[20];/*证件号:目前见到最长为身份证号18字节*/
}FaceGroupSearchInfo_t;

typedef struct 
{
	unsigned long long faceGroupId;/*人脸库id:UUID*/  
	char groupName[32];/*人脸库名称：utf-8*/
	//unsigned int actionState;/*布防状态:布防-0；不布防-1*/
	//unsigned int threshold;/*识别阈值*/
	//unsigned int compareLimit;/*对比上限*/
	char createTime[24];/*创建时间*/
	unsigned int enrolment;/*注册人数:通过读其下人脸个数*/ 
	int nValidState;/*脸库可用状态:0-->不可用；1--可用*/
}FaceGroupInfo_t;

//人脸库实例缓存信息
typedef struct 
{
	unsigned char used;		          //是否使用
	unsigned long long faceGroupId;/*人脸库id:UUID*/  
	//CFaceGroup *theFaceGroupHandle;         //一个人脸库数据句柄
	//    FaceGroupInfo_t faceGroupInfo;
}FaceGroupCacheInfo_t;

/************************************************* 人脸识别 ****************************************************/

#endif
