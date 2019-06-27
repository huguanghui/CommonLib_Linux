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

	AS_INIT_NODEV		= 0x80020001, // �豸������
	AS_INIT_BUSY		= 0x80020002, // �豸�Ѿ�����������
	AS_INIT_NORESPONSE	= 0x80020003, // �������ܰ�ʧ��,�豸����Ӧ
	AS_INIT_SYNCTIME	= 0x80020004, // ��ʱʧ��
	AS_INIT_HEARTBEAT   = 0x80020005, // ��������ʧ��
	AS_INIT_PIPE		= 0x80020006, // ������Ϣ�ܵ�ʧ��
	AS_INIT_ASYNCSEND	= 0x80020007, // �����첽��Ϣ�����߳�ʧ��
	AS_INIT_ASYNCRECV 	= 0x80020008, // �����첽��Ϣ�����߳�ʧ��
	AS_INIT_GETCAP		= 0x80020009, // ��ȡ����ʧ��

	AS_UNINIT_NOTINIT	= 0x80030001, // �豸δ��ʼ��

	
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
	char groupName[32];/*���������ƣ�utf-8*/
	//unsigned int actionState;/*����״̬:����-0��������-1*/
	//unsigned int threshold;/*ʶ����ֵ*/
	//unsigned int compareLimit;/*�Ա�����*/
	char createTime[24];/*����ʱ��*/
	unsigned int enrolment;/*ע������:ͨ����������������*/ 
	int nValidState;/*�������״̬:0-->�����ã�1--����*/
} AS_FACEGROUP_ATTR;

/*���������Ϣ*/
typedef struct strFaceProfile
{
	char name[32];/*����*/
	unsigned int gender;/*�Ա�:��-0��Ů-1*/
	char birthday[16];/*����:�ַ����洢*/
	char country[32];/*����*/
	char provinceCity[48];/*ʡ��-����*/
	FACEGROUPCARDTYPE_E cardType;/*֤������*/
	char cardId[20];/*֤����:Ŀǰ�����Ϊ���֤��18�ֽ�*/
	unsigned int featrueState;/*��ģ״̬���ɹ�-0��ʧ��-1*/
	int isValid;
} AS_FACE_PROFILE;

/*����������Ϣ*/
typedef struct strFaceFeature
{
	char facePicPath[80];/*ԭͼ·��*/
	char feature[2048];/*����ֵ*/
	int validLen;/*����ֵ��Ч����*/
	char featureVersion[8];/*����ֵ�汾:�ֽڳ��ȴ�����*/
	unsigned int featrueState;/*��ģ״̬���ɹ�-0��ʧ��-1*/
} AS_FACE_FEATURE;

/*������ѯ����*/
typedef struct
{	
	char name[32];/*������������-32�ֽ�*/
	unsigned int gender;/*�Ա�:��-0��Ů-1*/
	char startBirthday[16];/*���գ���С����*/
	char endBirthday[16];/*���գ��������*/
	char country[32];/*����*/
	char provinceCity[48];/*ʡ��-����*/
	FACEGROUPCARDTYPE_E cardType;/*֤������*/
	char cardId[20];/*֤����:Ŀǰ�����Ϊ���֤��18�ֽ�*/
} AS_FACE_SEARCH_BY_PROFILE_PARAM;

/*����������Ϣ*/
typedef struct strFaceFeature
{
	char facePicPath[80];/*ԭͼ·��*/
	char feature[2048];/*����ֵ*/
	int validLen;/*����ֵ��Ч����*/
	char featureVersion[8];/*����ֵ�汾:�ֽڳ��ȴ�����*/
	unsigned int featrueState;/*��ģ״̬���ɹ�-0��ʧ��-1*/
} AS_FACE_FEATURE;

/*������ѯ����*/
typedef struct
{
	char name[32];/*������������-32�ֽ�*/
	unsigned int gender;/*�Ա�:��-0��Ů-1*/
	char startBirthday[16];/*���գ���С����*/
	char endBirthday[16];/*���գ��������*/
	char country[32];/*����*/
	char provinceCity[48];/*ʡ��-����*/
	FACEGROUPCARDTYPE_E cardType;/*֤������*/
	char cardId[20];/*֤����:Ŀǰ�����Ϊ���֤��18�ֽ�*/
} AS_FACE_SEARCH_BY_FEATURE_PARAM;

typedef enum
{
	//!< ץ������
	AS_TASK_CAPTURE_FACE,
	//!< ��ȡ����
	AS_TASK_EXTRACT_FEATURE,
	//!< ץ�Ĳ���ȡ����
	AS_TASK_CAPTURE_EXTRACT_FEATURE,
	//!< ץ�Ĳ��ȶ�
	AS_TASK_CAPTURE_SEARCH,
	//!< ��ͼƬ�ȶ�
	AS_TASK_SEARCH_BY_JPG,
	//!< �������ȶ�
	AS_TASK_SEARCH_BY_FEATURE,
	//!< HTTP����
	AS_TASK_HTTP_CONFIG,
} AS_TASK_TYPE;


//!\brief ����ץ������
typedef struct
{
	//!< TODO
} TASK_CAPTURE_FACE_ATTR;

//!\brief ����ץ�Ľṹ
typedef struct
{
	//!< TODO
} TASK_CAPTURE_FACE_RESULT;

//!\brief ��ȡ������������
typedef struct
{
	//!< TODO
} TASK_EXTRACT_FEATURE_ATTR;

//!\brief ��ȡ�������Խ��
typedef struct
{
	//!< TODO
} TASK_EXTRACT_FEATURE_RESULT;

//!\brief ����ץ�Ĳ���ȡ��������
typedef struct
{
	//!< TODO
} TASK_CAPTURE_EXTRACT_FEATURE_ATTR;

//!\brief ����ץ�Ĳ���ȡ���Խṹ
typedef struct
{
	//!< TODO
} TASK_CAPTURE_EXTRACT_FEATURE_RESULT;

//!\brief ����ץ�Ĳ��ȶ�����
typedef struct
{
	//!< TODO
} TASK_CAPTURE_SEARCH_ATTR;

//!\brief ����ץ�Ĳ��ȶ�����
typedef struct
{
	//!< TODO
} TASK_CAPTURE_SEARCH_RESULT;

//!\brief ����ͼƬ�ȶ�����
typedef struct
{
	//!< TODO
} TASK_SEARCH_BY_JPG_ATTR;

//!\brief ����ͼƬ�ȶ�����
typedef struct
{
	//!< TODO
} TASK_SEARCH_BY_JPG_RESULT;

//!\brief ����ͼƬ�ȶ�����
typedef struct
{
	//!< TODO
} TASK_SEARCH_BY_FEATURE_ATTR;

//!\brief ����ͼƬ�ȶ�����
typedef struct
{
	//!< TODO
} TASK_SEARCH_BY_FEATURE_RESULT;

//!\brief http��������
typedef struct
{
	//!< TODO
} TASK_HTTP_CONFIG_ATTR;

//!\brief http��������
typedef struct
{
	//!< TODO
} TASK_HTTP_CONFIG_RESULT;

//!\brief ����ͳһ�ṹ��
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

//!\brief ���ܰ����
// ����Ƿ�������ܰ�
//!\return 0��ʾ�������, AS_PROBE_NODEV��ʾû�м�⵽���ܰ�
AS_API AS_S32 AiStick_Probe(void);


//!\brief ���ļ�����
//!\param pstTaskResult �������,����ִ�н��
//!\return 0��ʾִ�гɹ�, ������ʾ������
AS_API AS_S32 AS_SYS_Upgrade(AS_S8 *pszPackageFile, AS_UPGRADE_CALLBACK cbUpgrade);

//!\brief ���ڴ��������ڴ��������������������
//!\param pstTaskResult �������,����ִ�н��
//!\return 0��ʾִ�гɹ�, ������ʾ������
AS_API AS_S32 AS_SYS_UpgradeFromMemory(AS_U8 *pPackageBuffer, AS_S32 s32BufLength, AS_UPGRADE_CALLBACK cbUpgrade);

//!\brief  ����ģ��
AS_API AS_S32 AS_FACE_Init(AS_VOID);

//!\brief    �����������Ϣ������������������������������ӳ���
//!\param    pstFaceGroupInfo Ҫ��ӵ���������Ϣ
//!\param    ������ӳɹ�������ID
//!\return   0 �ɹ���������ʾ������
AS_API AS_S32 AS_FACE_CreateGroup(AS_FACEGROUP_ATTR *pstFaceGroupInfo, AS_U64 *pFaceGroup);

//!\brief    �����������ɾ��������������Ϣ��ɾ�������������������ӳ���
//!\param    FaceGroup		������ID
//!\param    delBindFaceFlag   �Ƿ�ɾ��������������Ϣ
//!\return   0 �ɹ���-1 ʧ��
AS_API AS_S32 AS_FACE_DeleteGroup(AS_FACEGRP FaceGroup, AS_BOOL bDelBindFace);

//!\brief    �������������Ƿ���ڸ�����������
//!\param    pFaceLibName Ҫ��������������
//!\return   0 ��ʾ������ 1��ʾ����
AS_API AS_BOOL AS_FACE_IsGroupExist(AS_S8 *pszFaceGroupName);

//!\brief    ��ȡ����������
//!\param    FaceGroup		������ID
//!\param    delBindFaceFlag   �Ƿ�ɾ��������������Ϣ
//!\return   0 �ɹ���-1 ʧ��
AS_API AS_S32 AS_FACE_GetGroupCount(void);

//!\brief    ��ȡָ����������Ϣ
//!\param    faceGroupId ָ��������������
//!\param    pFaceLibInfo Ҫ���ص���������Ϣ
//!\return   0 �ɹ���-1 ʧ��
//!\ע�����ڶ��ò�ѯ��������Ϣʱֱ�Ӵ��ڴ��п�����ֻ����������������ɾ���Ĳ���ʱ������ͬ��
AS_API AS_S32 AS_FACE_GetGroupAttr(AS_FACEGRP FaceGroup, AS_FACEGROUP_ATTR *pstFaceGroupInfo);    

//!\brief    �޸���������д�����������Ϣ
//!\param    groupId   ����������
//!\param    pstFaceGroupInfo Ҫ�޸ĵ���������Ϣ
//!\return   0 �ɹ���-1 ʧ��
AS_API AS_S32 AS_FACE_SetGroupAttr(AS_FACEGRP FaceGroup, AS_FACEGROUP_ATTR *pstFaceGroupInfo);

//!\brief    ���������Ϣ�ĺ�����
//!\param    faceId   ��������
//!\param    faceInfo Ҫ��ӵ�������Ϣ
//!\return   0 �ɹ���-1 ʧ��
AS_API AS_S32 AS_FACE_CheckProfile(AS_FACE_PROFILE stFaceProfile);

//!\brief    ���������Ϣ��������Ϣ��
//!\param    faceId   ��������
//!\param    faceInfo Ҫ��ӵ�������Ϣ
//!\return   0 �ɹ���-1 ʧ��
AS_API AS_S32 AS_FACE_AddToGroup(AS_FACEGRP FaceGroup,
								AS_FACE_PROFILE *pstFaceProfile, AS_FACE_FEATURE *pstFaceFeature,
								AS_BOOL bIgnoreError, AS_S8 *szFacePicPath);

//!\brief    ��δ���������а󶨵�������������
//!\param    unBindFaceGroupId   δ������������
//!\param    faceGroupId   Ҫ�󶨵�����������
//!\param    faceId Ҫ�󶨵���������
//!\return   0 �ɹ���-1 ʧ��
AS_API AS_S32 AS_FACE_BindGroup(AS_FACEGRP  , AS_FACE Face);

//!\brief    ���������������
//!\param    faceGroupId   ����������
//!\param    faceId ��������
//!\return   0 �ɹ���-1 ʧ��
AS_API AS_S32 AS_FACE_UnbindGroup(AS_FACEGRP FaceGroup, AS_FACE Face, AS_S32 bDelete);

//!\brief    ���������������
//!\param    faceGroupId   ����������
//!\param    faceId ��������
//!\return   0 �ɹ���-1 ʧ��
AS_API AS_S32 AS_FACE_SetProfile(AS_FACE Face, AS_FACE_PROFILE *pstFaceProfile);

//!\brief    �޸�������Ϣ����������Ϣ�������������䣬Ҳ����˵�����������и�������Ϣͬʱ����
//!\param    faceId   ��������
//!\param    faceInfo Ҫ��ӵ���������Ϣ
//!\return   0 �ɹ���-1 ʧ��
AS_API AS_S32 AS_FACE_SetFeature(AS_FACE Face, AS_FACE_FEATURE *pstFaceFeature);

//!\brief    ������ѯĳ������������������Ϣ
//!\param    faceGroupSearchInfo ��ѯ����
//!\param    pFaceInfo Ҫ���ص�ĳ��������������Ϣ
//!\return   0 �ɹ���-1 ʧ��
AS_API AS_S32 AS_FACE_SearchByProfile(AS_FACEGRP FaceGroup, FaceGroupSearchInfo_t faceGroupSearchInfo,AS_S32 pageNum,AS_S32 *pageSize,FaceInfo_t *pFaceInfo);


#endif
