#ifndef __AI_STICK_H__
#define __AI_STICK_H__

#include <sys/types.h>

#if defined(__cplusplus)
#define AS_API extern "C"
#else
#define AS_API
#endif

typedef uint32_t AS_BOOL;
typedef int8_t AS_S8;
typedef uint8_t AS_U8;
typedef int16_t AS_S16;
typedef uint16_t AS_U16;
typedef int32_t AS_S32;
typedef uint32_t AS_U32;
typedef int64_t AS_S64;
typedef uint64_t AS_U64;

typedef AS_U64 AS_FACEGRP;
typedef AS_U64 AS_FACE;
typedef AS_S64 AS_INDEX;
typedef void * AS_HANDLE;
typedef void AS_VOID;

#define AS_HANDLE_INVALID ((void *)(-1))

typedef enum
{
	AS_OK = 0,
	AS_PROBE_NODEV = 0x80010001,

	AS_INIT_NODEV		= 0x80020001, // 设备不存在
	AS_INIT_BUSY		= 0x80020002, // 设备已经在正常工作
	AS_INIT_NORESPONSE	= 0x80020003, // 启动智能棒失败,设备无响应
	AS_INIT_SYNCTIME	= 0x80020004, // 对时失败
	AS_INIT_HEARTBEAT   = 0x80020005, // 心跳启动失败
	AS_INIT_PIPE		= 0x80020006, // 创建消息管道失败
	AS_INIT_ASYNCSEND	= 0x80020007, // 创建异步消息发送线程失败
	AS_INIT_ASYNCRECV 	= 0x80020008, // 创建异步消息接受线程失败
	AS_INIT_GETCAP		= 0x80020009, // 获取能力失败

	AS_UNINIT_NOTINIT	= 0x80030001, // 设备未初始化

	
} AS_ERRNO;

typedef enum
{
	AS_STAT_OK,
	AS_STAT_NODEV,
	AS_STAT_BOOTING,
	AS_STAT_UPGRADING
} AS_STATE;

typedef enum
{
} AS_FACE_STATE;

typedef struct 
{ 
	char groupName[32];/*人脸库名称：utf-8*/
	//unsigned int actionState;/*布防状态:布防-0；不布防-1*/
	//unsigned int threshold;/*识别阈值*/
	//unsigned int compareLimit;/*对比上限*/
	char createTime[24];/*创建时间*/
	unsigned int enrolment;/*注册人数:通过读其下人脸个数*/ 
	int nValidState;/*脸库可用状态:0-->不可用；1--可用*/
} AS_FACEGROUP_ATTR;

/*人脸身份信息*/
typedef struct strFaceProfile
{
	char name[32];/*姓名*/
	unsigned int gender;/*性别:男-0；女-1*/
	char birthday[16];/*生日:字符串存储*/
	char country[32];/*国家*/
	char provinceCity[48];/*省份-城市*/
	FACEGROUPCARDTYPE_E cardType;/*证件类型*/
	char cardId[20];/*证件号:目前见到最长为身份证号18字节*/
	unsigned int featrueState;/*建模状态：成功-0；失败-1*/
	int isValid;
} AS_FACE_PROFILE;

/*人脸特征信息*/
typedef struct strFaceFeature
{
	char facePicPath[80];/*原图路径*/
	char feature[2048];/*特征值*/
	int validLen;/*特征值有效长度*/
	char featureVersion[8];/*特征值版本:字节长度待定？*/
	unsigned int featrueState;/*建模状态：成功-0；失败-1*/
} AS_FACE_FEATURE;

/*人脸查询参数*/
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
} AS_FACE_SEARCH_BY_PROFILE_PARAM;

/*人脸特征信息*/
typedef struct strFaceFeature
{
	char facePicPath[80];/*原图路径*/
	char feature[2048];/*特征值*/
	int validLen;/*特征值有效长度*/
	char featureVersion[8];/*特征值版本:字节长度待定？*/
	unsigned int featrueState;/*建模状态：成功-0；失败-1*/
} AS_FACE_FEATURE;

/*人脸查询参数*/
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
} AS_FACE_SEARCH_BY_FEATURE_PARAM;

typedef enum
{
	//!< 抓拍任务
	AS_TASK_CAPTURE_FACE,
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
} AS_TASK_TYPE;


//!\brief 人脸抓拍任务
typedef struct
{
	//!< TODO
} TASK_CAPTURE_FACE_ATTR;

//!\brief 人脸抓拍结构
typedef struct
{
	//!< TODO
} TASK_CAPTURE_FACE_RESULT;

//!\brief 抽取人脸属性任务
typedef struct
{
	//!< TODO
} TASK_EXTRACT_FEATURE_ATTR;

//!\brief 抽取人脸属性结果
typedef struct
{
	//!< TODO
} TASK_EXTRACT_FEATURE_RESULT;

//!\brief 人脸抓拍并抽取属性任务
typedef struct
{
	//!< TODO
} TASK_CAPTURE_EXTRACT_FEATURE_ATTR;

//!\brief 人脸抓拍并抽取属性结构
typedef struct
{
	//!< TODO
} TASK_CAPTURE_EXTRACT_FEATURE_RESULT;

//!\brief 人脸抓拍并比对任务
typedef struct
{
	//!< TODO
} TASK_CAPTURE_SEARCH_ATTR;

//!\brief 人脸抓拍并比对任务
typedef struct
{
	//!< TODO
} TASK_CAPTURE_SEARCH_RESULT;

//!\brief 人脸图片比对任务
typedef struct
{
	//!< TODO
} TASK_SEARCH_BY_JPG_ATTR;

//!\brief 人脸图片比对任务
typedef struct
{
	//!< TODO
} TASK_SEARCH_BY_JPG_RESULT;

//!\brief 人脸图片比对任务
typedef struct
{
	//!< TODO
} TASK_SEARCH_BY_FEATURE_ATTR;

//!\brief 人脸图片比对任务
typedef struct
{
	//!< TODO
} TASK_SEARCH_BY_FEATURE_RESULT;

//!\brief http配置任务
typedef struct
{
	//!< TODO
} TASK_HTTP_CONFIG_ATTR;

//!\brief http配置任务
typedef struct
{
	//!< TODO
} TASK_HTTP_CONFIG_RESULT;

//!\brief 任务统一结构体
typedef struct
{
	AS_TASK_TYPE enTaskType;
	union {
		TASK_CAPTURE_FACE_ATTR stCaptureFaceTask;
		TASK_EXTRACT_FEATURE_ATTR stExtractFeatureTask;
		TASK_CAPTURE_EXTRACT_FEATURE_ATTR stCaptureExtractFeatureTask;
		TASK_CAPTURE_SEARCH_ATTR stCaptureSearchTask;
		TASK_SEARCH_BY_JPG_ATTR stSearchByJpgTask;
		TASK_SEARCH_BY_FEATURE_ATTR stSearchByFeatureTask;
	};
} AS_TASK_ATTR;

typedef struct
{
	AS_HANDLE hTask;
	AS_TASK_TYPE enTaskType;
	union {
		TASK_CAPTURE_FACE_ATTR stCaptureFaceResult;
		TASK_EXTRACT_FEATURE_ATTR stExtractFeatureResult;
		TASK_CAPTURE_EXTRACT_FEATURE_ATTR stCaptureExtractFeatureResult;
		TASK_CAPTURE_SEARCH_ATTR stCaptureSearchResult;
		TASK_SEARCH_BY_JPG_ATTR stSearchByJpgResult;
		TASK_SEARCH_BY_FEATURE_ATTR stSearchByFeatureResult;
	}
	
} AS_TASK_RESULT;

#define AS_CAP_FACE_DETECT	0x00000001UL
#define AS_CAP_FACE_COMPARE	0x00000002UL

typedef enum
{
} AS_UPGRADE_STATE;

typedef enum
{
} AS_UPGRADE_STAGE;

typedef AS_VOID (*AS_UPGRADE_CALLBACK)(AS_UPGRADE_STATE enState, AS_UPGRADE_STAGE enStage, AS_S32 s32Progress);

//!\brief 智能棒检测
// 检测是否存在智能棒
//!\return 0表示检测正常, AS_PROBE_NODEV表示没有检测到智能棒
AS_API AS_S32 AiStick_Probe(void);


//!\brief 从文件升级
//!\param pstTaskResult 输入参数,任务执行结果
//!\return 0表示执行成功, 其他表示错误码
AS_API AS_S32 AS_SYS_Upgrade(AS_S8 *pszPackageFile, AS_UPGRADE_CALLBACK cbUpgrade);

//!\brief 从内存升级，内存中需包含完整的升级包
//!\param pstTaskResult 输入参数,任务执行结果
//!\return 0表示执行成功, 其他表示错误码
AS_API AS_S32 AS_SYS_UpgradeFromMemory(AS_U8 *pPackageBuffer, AS_S32 s32BufLength, AS_UPGRADE_CALLBACK cbUpgrade);

//!\brief  人脸模块
AS_API AS_S32 AS_FACE_Init(AS_VOID);

//!\brief    添加人脸库信息到人脸库表，创建该人脸库和其下人脸映射表
//!\param    pstFaceGroupInfo 要添加的人脸库信息
//!\param    返回添加成功的脸库ID
//!\return   0 成功；其他表示错误吗
AS_API AS_S32 AS_FACE_CreateGroup(AS_FACEGROUP_ATTR *pstFaceGroupInfo, AS_U64 *pFaceGroup);

//!\brief    从人脸库表中删除该条人脸库信息，删除该人脸库和其下人脸映射表
//!\param    FaceGroup		人脸库ID
//!\param    delBindFaceFlag   是否删除人脸库人脸信息
//!\return   0 成功；-1 失败
AS_API AS_S32 AS_FACE_DeleteGroup(AS_FACEGRP FaceGroup, AS_BOOL bDelBindFace);

//!\brief    检查人脸库表中是否存在该人脸库名称
//!\param    pFaceLibName 要检测的人脸库名称
//!\return   0 表示不存在 1表示存在
AS_API AS_BOOL AS_FACE_IsGroupExist(AS_S8 *pszFaceGroupName);

//!\brief    获取人脸库数字
//!\param    FaceGroup		人脸库ID
//!\param    delBindFaceFlag   是否删除人脸库人脸信息
//!\return   0 成功；-1 失败
AS_API AS_S32 AS_FACE_GetGroupCount(void);

//!\brief    获取指定人脸库信息
//!\param    faceGroupId 指定的人脸库索引
//!\param    pFaceLibInfo 要返回的人脸库信息
//!\return   0 成功；-1 失败
//!\注：由于定好查询人脸库信息时直接从内存中拷贝，只需做好人脸库增、删、改操作时的数据同步
AS_API AS_S32 AS_FACE_GetGroupAttr(AS_FACEGRP FaceGroup, AS_FACEGROUP_ATTR *pstFaceGroupInfo);    

//!\brief    修改人脸库表中此条人脸库信息
//!\param    groupId   人脸库索引
//!\param    pstFaceGroupInfo 要修改的人脸库信息
//!\return   0 成功；-1 失败
AS_API AS_S32 AS_FACE_SetGroupAttr(AS_FACEGRP FaceGroup, AS_FACEGROUP_ATTR *pstFaceGroupInfo);

//!\brief    检测人脸信息的合理性
//!\param    faceId   人脸索引
//!\param    faceInfo 要添加的人脸信息
//!\return   0 成功；-1 失败
AS_API AS_S32 AS_FACE_CheckProfile(AS_FACE_PROFILE stFaceProfile);

//!\brief    添加人脸信息到人脸信息表
//!\param    faceId   人脸索引
//!\param    faceInfo 要添加的人脸信息
//!\return   0 成功；-1 失败
AS_API AS_S32 AS_FACE_AddToGroup(AS_FACEGRP FaceGroup,
								AS_FACE_PROFILE *pstFaceProfile, AS_FACE_FEATURE *pstFaceFeature,
								AS_BOOL bIgnoreError, AS_S8 *szFacePicPath);

//!\brief    将未绑定人脸库中绑定到其他人脸库中
//!\param    unBindFaceGroupId   未绑定人脸库索引
//!\param    faceGroupId   要绑定的人脸库索引
//!\param    faceId 要绑定的人脸索引
//!\return   0 成功；-1 失败
AS_API AS_S32 AS_FACE_BindGroup(AS_FACEGRP  , AS_FACE Face);

//!\brief    解绑人脸库中人脸
//!\param    faceGroupId   人脸库索引
//!\param    faceId 人脸索引
//!\return   0 成功；-1 失败
AS_API AS_S32 AS_FACE_UnbindGroup(AS_FACEGRP FaceGroup, AS_FACE Face, AS_S32 bDelete);

//!\brief    解绑人脸库中人脸
//!\param    faceGroupId   人脸库索引
//!\param    faceId 人脸索引
//!\return   0 成功；-1 失败
AS_API AS_S32 AS_FACE_SetProfile(AS_FACE Face, AS_FACE_PROFILE *pstFaceProfile);

//!\brief    修改人脸信息表中人脸信息：人脸索引不变，也就是说所有人脸库中该人脸信息同时更新
//!\param    faceId   人脸索引
//!\param    faceInfo 要添加的人脸库信息
//!\return   0 成功；-1 失败
AS_API AS_S32 AS_FACE_SetFeature(AS_FACE Face, AS_FACE_FEATURE *pstFaceFeature);

//!\brief    条件查询某人脸库下所有人脸信息
//!\param    faceGroupSearchInfo 查询条件
//!\param    pFaceInfo 要返回的某人脸库下人脸信息
//!\return   0 成功；-1 失败
AS_API AS_S32 AS_FACE_SearchByProfile(AS_FACEGRP FaceGroup, FaceGroupSearchInfo_t faceGroupSearchInfo,AS_S32 pageNum,AS_S32 *pageSize,FaceInfo_t *pFaceInfo);


#endif
