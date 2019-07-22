#include <fstream>
#include "zw_usbpipe_util.h"
#include "zw_aistick_task.h"
#include "zw_aistick_api.h"
#include "CGINewbornAPI.h"
#include "zw_aistick_httpclient.h"

using namespace CGI_ApplicationLayer;

struct cardTypeMap
{
	FACEGROUPCARDTYPE_E eCardType;
	char *strCardType; 
};

static cardTypeMap g_arrCardType[4] = {
	{FACEGROUPCARDTYPE_NULL, (char*)"none"},
	{FACEGROUPPASSORD, (char*)"passport"},
	{FACEGROUPIDENTITYCARD, (char*)"IC"},
	{FACEGROUPOTHERCARD, (char*)"other"}
};

static char* zw_cardtype_enum2str(FACEGROUPCARDTYPE_E e)
{
	// 默认设为 none
	char* res = g_arrCardType[0].strCardType;

	for (int i=0; i < (int)(sizeof(g_arrCardType)/sizeof(cardTypeMap)); i++)
	{
		if (e == g_arrCardType[i].eCardType)
		{
			res = g_arrCardType[i].strCardType;
			break;
		}
	}

	return res;
}

static FACEGROUPCARDTYPE_E zw_cardtype_str2enum(const char *s)
{
	FACEGROUPCARDTYPE_E res = FACEGROUPCARDTYPE_NULL;
	
	if (s != NULL)
	{
		for (int i=0; i < (int)(sizeof(g_arrCardType)/sizeof(cardTypeMap)); i++)
		{
			if (strcasecmp(s, g_arrCardType[i].strCardType) == 0)
			{
				res = g_arrCardType[i].eCardType;
				break;
			}
		}
	}

	return res;
}

struct genedrMap
{
	FACEGROUPGRENDER_E eGender;
	char *strGender; 
};

static genedrMap g_arrGender[3] = {
	{FACEGROUPGRENDER_NULL, (char*)"none"},
	{FACEGROUPGRENDER_MAN, (char*)"male"},
	{FACEGROUPGRENDER_WOMAN, (char*)"female"}
};

static char* zw_gender_enum2str(FACEGROUPGRENDER_E e)
{
	// 默认设为 none
	char* res = g_arrGender[0].strGender;

	for (int i=0; i < (int)(sizeof(g_arrGender)/sizeof(genedrMap)); i++)
	{
		if (e == g_arrGender[i].eGender)
		{
			res = g_arrGender[i].strGender;
			break;
		}
	}

	return res;
}

static FACEGROUPGRENDER_E zw_gender_str2enum(const char *s)
{
	FACEGROUPGRENDER_E res = FACEGROUPGRENDER_NULL;

	if (s != NULL)
	{
		for (int i=0; i < (int)(sizeof(g_arrGender)/sizeof(genedrMap)); i++)
		{
			if (strcasecmp(s, g_arrGender[i].strGender) == 0)
			{
				res = g_arrGender[i].eGender;
				break;
			}
		}
	}

	return res;
}

struct faceCapModeMap
{
	FACECAPMODE_T eCapMode;
	char *strCapMode; 
};

static faceCapModeMap g_arrCapMode[4] = {
	{FACE_CAP_MODE_CUSTOM, (char*)"custom"},
	{FACE_CAP_MODE_AUTO, (char*)"auto"},
	{FACE_CAP_MODE_OPTIMAL, (char*)"optimal"},
	{FACE_CAP_MODE_FAST, (char*)"fast"}
};

static char* zw_capmode_enum2str(FACECAPMODE_T e)
{
	// 默认设为 none
	char* res = g_arrCapMode[0].strCapMode;

	for (int i=0; i < (int)(sizeof(g_arrCapMode)/sizeof(faceCapModeMap)); i++)
	{
		if (e == g_arrCapMode[i].eCapMode)
		{
			res = g_arrCapMode[i].strCapMode;
			break;
		}
	}

	return res;
}

static FACECAPMODE_T zw_capmode_str2enum(const char *s)
{
	FACECAPMODE_T res = FACE_CAP_MODE_CUSTOM;

	if (s != NULL)
	{
		for (int i=0; i < (int)(sizeof(g_arrCapMode)/sizeof(faceCapModeMap)); i++)
		{
			if (strcasecmp(s, g_arrCapMode[i].strCapMode) == 0)
			{
				res = g_arrCapMode[i].eCapMode;
				break;
			}
		}
	}

	return res;
}

struct faceCutModeMap
{
	int iCutMode;
	char *strCutMode; 
};

static faceCutModeMap g_arrCutMode[4] = {
	{0, (char*)"face_only"},
	{1, (char*)"half_body"},
	{2, (char*)"whole_body"}
};

static char* zw_cutmode_enum2str(int e)
{
	// 默认设为 none
	char* res = g_arrCutMode[0].strCutMode;

	for (int i=0; i < (int)(sizeof(g_arrCutMode)/sizeof(faceCutModeMap)); i++)
	{
		if (e == g_arrCutMode[i].iCutMode)
		{
			res = g_arrCutMode[i].strCutMode;
			break;
		}
	}

	return res;
}

static int zw_cutmode_str2enum(const char *s)
{
	int res = 0;

	if (s != NULL)
	{
		for (int i=0; i < (int)(sizeof(g_arrCutMode)/sizeof(faceCutModeMap)); i++)
		{
			if (strcasecmp(s, g_arrCutMode[i].strCutMode) == 0)
			{
				res = g_arrCutMode[i].iCutMode;
				break;
			}
		}
	}

	return res;
}

static const char* g_arrWorkState[4] = {
	"Initializing",
	"normal",
	"recoverying",
	"modelling"
};

static int zw_workState2int(const char *s)
{
	int i = 0;

	if (s != NULL)
	{
		for (i = 0; i < (int)(sizeof(g_arrWorkState)/sizeof(char*)); i++)
		{
			if (strcasecmp(s, g_arrWorkState[i]) == 0)
			{
				break;
			}
		}
	}

	return i;
}

static const char* g_arrBool[2] = {
	"false",
	"true"
};

static const char* zw_int2bool(int i)
{
	int idx = 0;
	idx = i == 0?0 : 1;
	return g_arrBool[idx];
}

static int zw_bool2int(const char *s)
{
	int i = 0;

	if ((s != NULL) && (strcasecmp(s, "true") == 0))
	{
		i = 1;
	}

	return i;
}

static int zw_validstate_str2int(const char *s)
{
	int i = 0;

	if ((s != NULL) && (strcasecmp(s, "available") == 0))
	{
		i = 1;
	}

	return i;
}

// static const char* g_arrResultType[4] = {
// 	"all",
// 	"Comparison pass",
// 	"Comparison fail",
// 	"Failure of biopsy"
// };

// static const char* zw_int2ResultType(int i)
// {
// 	if (i < 0 || i > 3)
// 	{
// 		i = 0;
// 	}

// 	return g_arrResultType[i];
// }

static const char* g_arrLinkageStrategy[2] = {
	"Comparison pass",
	"Comparison fail"
};

static const char* zw_int2LinkageStrategy(int i)
{
	if (i < 0 || i > 1)
	{
		i = 0;
	}

	return g_arrLinkageStrategy[i];
}

static int zw_LinkageStrategy2int(const char *s)
{
	int i = 0;

	if ((s != NULL) && (strcasecmp(s, "Comparison fail") == 0))
	{
		i = 1;
	}

	return i;
}


static void HK_SCHEDULE_TranslateTo_FaceSchedule(HK_SCHEDULE *src, FaceSchedule *dst)
{
	if (NULL == src || NULL == dst)
	{
		err("param err! src[%p] dst[%p]\n", src, dst);
		return;
	}
	dst->bytState = (src->nValid==0)?0 : 1;
	if (zw_bool2int(src->EnableAllDay) == 1)
	{
		dst->bytState = 0;
	}
	else
	{
		dst->bytState = 1;
		int i,j;
		for (i = 0; i < MAX_DAYS; i++)
		{
			for (j = 0; j < HY_DVR_FACESEG_DAY; j++)
			{
				dst->dwTimeSeg[i][j] = (unsigned char)(src->struTimeBlockList.struTimeBlock[i].strTimeBlock[j] - '0');
			}
		}		
	}
	

	return;
}

static void FaceSchedule_TranslateTo_HK_SCHEDULE(FaceSchedule *src, HK_SCHEDULE *dst)
{
	if (NULL == src || NULL == dst)
	{
		err("param err! src[%p] dst[%p]\n", src, dst);
		return;
	}

	dst->nValid = 1;
	int bAllDay = src->bytState == 0;
	snprintf(dst->EnableAllDay, sizeof(dst->EnableAllDay), "%s", zw_int2bool(bAllDay));
	if (0 == bAllDay)
	{
		dst->struTimeBlockList.nValid = 1;
		int i,j;
		for (i = 0; i < MAX_DAYS; i++)
		{
			dst->struTimeBlockList.struTimeBlock[i].nValid = 1;
			for (j = 0; j < HY_DVR_FACESEG_DAY; j++)
			{
				dst->struTimeBlockList.struTimeBlock[i].strTimeBlock[j] = (char)('0'+src->dwTimeSeg[i][j]);
			}
		}
	}

	return;
}

typedef enum {
	OPT_Begin,
	OPT_GET,
	OPT_PUT,
	OPT_POST,
	OPT_End
} eOPTMethod;

int AS_CGIOperator(eOPTMethod opt, CGIParamBase &pCfg)
{
	int ret = 0;
	int rc;

	// Auth
	userAuth auth;
	memset(&auth, 0, sizeof(auth));
	auth.type = 1;
	snprintf(auth.username, sizeof(auth.username), "admin");

	// url
	char *url = pCfg.GetURL();

	switch (opt)
	{
		case OPT_GET: {
			char *resp = NULL;
			int respLen = 0;

			rc = HTTP_ON_USBPIPE_GET(&auth, url, NULL, (void *)NULL, 0, (void **)&resp, &respLen);
			if (0 == rc)
			{
				if (resp != NULL && respLen > 0)
				{
					//info("resp[\n%.*s]\n", respLen, resp);
					pCfg.ToMe(resp, respLen);
				}
			}
			else
			{
				ret = -1;
			}
			if (resp != NULL)
			{
				free(resp);
			}
			break;
		}
		case OPT_PUT: {
			char *req = (char*)malloc(1*1024*1024);
			int parseLen = 1*1024*1024;
			char *resp = NULL;
			int respLen = 0;

			pCfg.ToXML(req, parseLen);
			rc = HTTP_ON_USBPIPE_PUT(&auth, url, NULL, (void *)req, parseLen, (void **)&resp, &respLen);
			if (0 == rc)
			{
				if (resp != NULL && respLen > 0)
				{
					// 根据类构造条件决定是否检测返回消息状态
					int bCheckStatus = pCfg.GetCheckRespStatus();
					int respStatus = 0;
					if (1 == bCheckStatus)
					{
						rc = pCfg.m_cRespStatus.ToMe(resp, respLen);
						if (0 == rc)
						{
							pCfg.m_cRespStatus.GetRespStatus(respStatus);
						}
					}
					if (0 == respStatus)
					{
						pCfg.ToMe(resp, respLen);
						ret = 0;
					}
					else
					{
						ret = respStatus;
						info("respStatus[%d]\n", ret);
					}
				}
			}
			else
			{
				ret = -1;
			}
			if (resp != NULL)
			{
				free(resp);
			}
			free(req);
			break;
		}
		case OPT_POST: {
			
			break;
		}
		default:{
			err("unknonw opt!\n");
			ret = -1;
		}
	}

	return ret;
}

/************************************************* 系统相关 ****************************************************/

int AS_FaceReco_GetCap(FaceRecoCap_t *cap)
{
	if (NULL == cap)
	{
		err("param err! cap[%p]\n", cap);
		return -1;
	}

	int ret = -1;
	HK_FACE_RECO_CAP stCap;
	memset(&stCap, 0, sizeof(stCap));

	FaceRecoCap recoCap;
	int rc = AS_CGIOperator(OPT_GET, recoCap);
	if (0 == rc)
	{
		recoCap.GetParam(&stCap, sizeof(stCap));
		if (1 == stCap.nValid)
		{
			cap->bSupportFaceReco = zw_bool2int(stCap.strSupportFaceReco);
			cap->nMaxFaceGroupNum = stCap.faceGroupNum;
			cap->nMaxPersonNumPerGroup = stCap.personPerFaceGroup;
			cap->nMaxPersonNumTotal = stCap.personByAllFaceGroup;
			cap->nMaxImageSize = stCap.personImageSize;
			cap->snapShot.max.width = stCap.faceSnapshot.max.width;
			cap->snapShot.max.height = stCap.faceSnapshot.max.height;
			cap->snapShot.min.width = stCap.faceSnapshot.min.width;
			cap->snapShot.min.height = stCap.faceSnapshot.min.height;
			ret = 0;
		}
	}

	return ret;	
}

int AS_FaceReco_GetWorkState(int *state, int *errCode)
{
	if (NULL == state || NULL == errCode)
	{
		err("param err! state[%p] errCode[%p]\n", state, errCode);
		return -1;
	}

	int ret = -1;
	HK_FACE_RECO_MODULE_WORK_STATE recoState;
	memset(&recoState, 0, sizeof(recoState));

	FaceRecoWorkState workState;
	int rc = AS_CGIOperator(OPT_PUT, workState);
	if (0 == rc)
	{
		workState.GetParam(&recoState, sizeof(recoState));
		if (1 == recoState.nValid)
		{
			*state = zw_workState2int(recoState.strWorkState);
			*errCode = recoState.nErrorCode;
			ret = 0;
		}
	}

	return ret;	
}

/************************************************* 脸库相关 ****************************************************/

int AS_FaceReco_GroupCreate(FaceGroupInfo_t *info)
{
	if (NULL == info)
	{
		err("param err! info[%p]\n", info);
		return -1;
	}

	int ret = -1;
	HK_CREATE_FACE_GROUP group;
	memset(&group, 0, sizeof(group));
	group.nValid = 1;
	snprintf(group.strGroupName, sizeof(group.strGroupName), "%s", info->groupName);

	FaceRecoFaceGroupCreate faceGroupCreate;
	faceGroupCreate.SetParam(&group, sizeof(group));
	int rc = AS_CGIOperator(OPT_PUT, faceGroupCreate);
	if (0 == rc)
	{
		faceGroupCreate.GetParam(&group, sizeof(group));
		if (1 == group.nValid)
		{
			info->faceGroupId = (unsigned long long)group.nGroupID;
			ret = 0;
		}
	}

	return ret;
}

int AS_FaceReco_GroupModify(FaceGroupInfo_t *info)
{
	if (NULL == info)
	{
		err("param err! info[%p]\n", info);
		return -1;
	}

	int ret = -1;
	HK_MODIFY_FACE_GROUP group;
	memset(&group, 0, sizeof(group));
	group.nValid = 1;
	group.nGroupID = (long long)info->faceGroupId;
	snprintf(group.strGroupName, sizeof(group.strGroupName), "%s", info->groupName);

	FaceRecoFaceGroupModify faceGroupModify;
	faceGroupModify.SetParam(&group, sizeof(group));
	int rc = AS_CGIOperator(OPT_PUT, faceGroupModify);
	if (0 == rc)
	{
		ret = 0;
	}

	return ret;
}

int AS_FaceReco_GroupDelete(unsigned long long groupId, int bDelBindFace)
{
	int ret = -1;
	HK_DELETE_FACE_GROUP group;
	memset(&group, 0, sizeof(group));
	group.nValid = 1;
	group.nGroupID = (long long)groupId;
	snprintf(group.strIsDelBindFace, sizeof(group.strIsDelBindFace), "%s", zw_int2bool(bDelBindFace));

	FaceRecoFaceGroupDelete faceGroupDelete;
	faceGroupDelete.SetParam(&group, sizeof(group));
	int rc = AS_CGIOperator(OPT_PUT, faceGroupDelete);
	if (0 == rc)
	{
		ret = 0;
	}

	return ret;
}

int AS_FaceReco_GroupQueryAll(FaceGroupInfo_t *pFaceGroups, int *pCount)
{
	if (NULL == pFaceGroups || NULL == pCount)
	{
		err("param err! pFaceGroups[%p] pCount[%p]\n", pFaceGroups, pCount);
		return -1;
	}

	int ret = -1;
	int cnt = 0;
	HK_FACE_GROUP_INFO_LIST group;
	HK_FACE_GROUP_INFO *node = NULL;
	FaceGroupInfo_t *info = NULL;
	
	memset(&group, 0, sizeof(group));
	
	FaceRecoFaceGroupQueryAll faceGroupQueryAll;
	faceGroupQueryAll.SetParam(&group, sizeof(group));
	int rc = AS_CGIOperator(OPT_PUT, faceGroupQueryAll);
	if (0 == rc)
	{
		faceGroupQueryAll.GetParam(&group, sizeof(group));
		if (1 == group.nValid)
		{
			for (int i = 0; i < HK_MAX_GROUP_FACE_COUNT; i++)
			{
				node = &group.struFaceGroupInfo[i];
				if (1 == node->nValid)
				{
					if (cnt >= *pCount)
					{
						break;
					}
					info = pFaceGroups + cnt;
					info->faceGroupId = (unsigned long long)node->nGroupID;
					snprintf(info->groupName, sizeof(info->groupName), "%s", node->strGroupName);
					snprintf(info->createTime, sizeof(info->createTime), "%s", node->strCreateTime);
					info->enrolment = (unsigned int)node->nPersonInfoCount;
					info->nValidState = zw_validstate_str2int(node->strAvailableStatus);
					cnt++;
				}
				else
				{
					break;
				}
				
			}
			ret = 0;
		}
	}
	*pCount = cnt;

	return ret;
}

int AS_FaceReco_GroupQueryByGroupID(FaceGroupInfo_t *pFaceGroup)
{
	if (NULL == pFaceGroup)
	{
		err("param err! pFaceGroup[%p]\n", pFaceGroup);
		return -1;
	}

	int ret = -1;
	HK_FACE_GROUP_INFO group;
	memset(&group, 0, sizeof(group));
	group.nValid = 1;
	group.nGroupID = (long long)pFaceGroup->faceGroupId;

	FaceRecoFaceGroupQueryByID faceGroupQueryByID;
	faceGroupQueryByID.SetParam(&group, sizeof(group));
	int rc = AS_CGIOperator(OPT_PUT, faceGroupQueryByID);
	if (0 == rc)
	{
		faceGroupQueryByID.GetParam(&group, sizeof(group));
		if (1 == group.nValid)
		{
			snprintf(pFaceGroup->groupName, sizeof(pFaceGroup->groupName), "%s", group.strGroupName);
			snprintf(pFaceGroup->createTime, sizeof(pFaceGroup->createTime), "%s", group.strCreateTime);
			pFaceGroup->enrolment = (unsigned int)group.nPersonInfoCount;
			pFaceGroup->nValidState = zw_validstate_str2int(group.strAvailableStatus);
			ret = 0;
		}
	}

	return ret;
}

/************************************************* 人脸相关 ****************************************************/

int AS_FaceReco_PersonInfoCheck(FaceGroupFaceInfo_t *pInfo)
{
	if (NULL == pInfo)
	{
		err("param err! Info[%p]\n", pInfo);
		return -1;
	}

	int ret = -1;
	HK_CHECK_PERSON_INFO personInfo;
	memset(&personInfo, 0, sizeof(personInfo));
	personInfo.nValid = 1;
	personInfo.nPersonID = (long long)pInfo->faceId;
	snprintf(personInfo.strName, sizeof(personInfo.strName), "%s", pInfo->name);
	snprintf(personInfo.strSex, sizeof(personInfo.strSex), "%s", 
						((pInfo->gender == FACEGROUPGRENDER_MAN)?"male" : "female"));
	snprintf(personInfo.strCertificateType, sizeof(personInfo.strCertificateType), "%s", 
						((pInfo->cardType == FACEGROUPPASSORD)?"passport" : "IC"));
	snprintf(personInfo.strCertificateID, sizeof(personInfo.strCertificateID), "%s", 
						pInfo->cardId);
	snprintf(personInfo.strCountry, sizeof(personInfo.strCountry), "%s", 
						pInfo->country);
	snprintf(personInfo.strAddress, sizeof(personInfo.strAddress), "%s", 
						pInfo->provinceCity);
	snprintf(personInfo.strBirthday, sizeof(personInfo.strBirthday), "%s", 
						pInfo->birthday);

	FaceRecoFacePersonInfoCheck infoCheck;
	infoCheck.SetParam(&personInfo, sizeof(personInfo));
	int rc = AS_CGIOperator(OPT_PUT, infoCheck);
	if (0 == rc)
	{
		infoCheck.GetParam(&personInfo, sizeof(personInfo));
		pInfo->isValid = personInfo.nValid;
		ret = 0;
	}

	return ret;
}

static void mbuf_append_form_long_long(struct mbuf *m, const char *name, long long value)
{
	if (NULL == m || NULL == name)
	{
		err("param err!m[%p] name[%p]\n", m, name);
		return;
	}
	char buf[256];
	
	snprintf(buf, sizeof(buf), "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
		"Content-Disposition: form-data; name=\"%s\"\r\n\r\n%llu\r\n", name, value);
	
	mbuf_append(m, buf, strlen(buf));

	return;
}

static void mbuf_append_form_int(struct mbuf *m, const char *name, int value)
{
	if (NULL == m || NULL == name)
	{
		err("param err!m[%p] name[%p]\n", m, name);
		return;
	}
	char buf[256];
	
	snprintf(buf, sizeof(buf), "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
		"Content-Disposition: form-data; name=\"%s\"\r\n\r\n%d\r\n", name, value);
	
	mbuf_append(m, buf, strlen(buf));

	return;
}

static void mbuf_append_form_str(struct mbuf *m, const char *name, const char *value)
{
	if (NULL == m || NULL == name || NULL == value)
	{
		err("param err!m[%p] name[%p] value[%p]\n", m, name, value);
		return;
	}
	char buf[256];
	
	snprintf(buf, sizeof(buf), "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
		"Content-Disposition: form-data; name=\"%s\"\r\n\r\n%s\r\n", name, value);
	
	mbuf_append(m, buf, strlen(buf));

	return;
}

static void mbuf_append_form_jpg(struct mbuf *m, const char *name, const char *filename)
{
	if (NULL == m || NULL == filename || NULL == name)
	{
		err("param err!m[%p] filename[%p] name[%p]\n", m, filename, name);
		return;
	}
	char buf[256];
	
	snprintf(buf, sizeof(buf), "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
		"Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n"
		"Content-Type: image/jpeg\r\n\r\n", name, filename);
	mbuf_append(m, buf, strlen(buf));

	return;
}

int AS_FaceReco_PersonInfoCreate(unsigned long long faceGroupId, int faceImageID, FaceGroupFaceInfo_t *pInfo, int ifIgnoreError, char *PicBuf,int bufLen)
{
	if (NULL == pInfo || NULL == PicBuf || bufLen <= 0)
	{
		err("param err! Info[%p] PicBuf[%p] bufLen[%d]\n", pInfo, PicBuf, bufLen);
		return -1;
	}

	int ret = -1;
	
	// auth
	userAuth auth;
	memset(&auth, 0, sizeof(auth));
	auth.type = 1;
	snprintf(auth.username, sizeof(auth.username), "admin");

	// recv-data
	char *recvBuf = NULL;
	int recvLen = 0;
	// send-data
	struct mbuf send_mbuf;
	mbuf_init(&send_mbuf, 0);
	{
		mbuf_append_form_str(&send_mbuf, "IgnoreError", zw_int2bool(ifIgnoreError));
		mbuf_append_form_str(&send_mbuf, "Name", pInfo->name);
		mbuf_append_form_str(&send_mbuf, "Sex", zw_gender_enum2str(pInfo->gender));
		mbuf_append_form_str(&send_mbuf, "CertificateType", zw_cardtype_enum2str(pInfo->cardType));
		mbuf_append_form_str(&send_mbuf, "CertificateID", pInfo->cardId);
		mbuf_append_form_long_long(&send_mbuf, "GroupID", (long long)faceGroupId);
		mbuf_append_form_str(&send_mbuf, "Country", pInfo->country);
		mbuf_append_form_str(&send_mbuf, "Address", pInfo->provinceCity);
		mbuf_append_form_str(&send_mbuf, "Birthday", pInfo->birthday);
		mbuf_append_form_jpg(&send_mbuf, "file", "face.jpg");
		mbuf_append(&send_mbuf, PicBuf, bufLen);
		char form_end[64] = "\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW--";
		mbuf_append(&send_mbuf, form_end, strlen(form_end));
	}
	int rc = HTTP_ON_USBPIPE_POST(&auth, "/FaceGroup/AddPersonInfoAndFaceImage", 
						"content-type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW", 
						NULL, (void*)send_mbuf.buf, send_mbuf.len, (void**)&recvBuf, &recvLen);
	if (0 == rc)
	{
		ret = 0;
		int status = 0;
		CGIRespStatus respStatus;
		rc = respStatus.ToMe(recvBuf, recvLen);
		if (0 == rc)
		{
			respStatus.GetRespStatus(status);
			ret = status;
			info("status[%d]\n", status);
		}
	}
	if (recvBuf != NULL)
	{
		free(recvBuf);
	}
	mbuf_free(&send_mbuf);

	return ret;
}

int AS_FaceReco_PersonInfoDelete(long long faceGroupId, long long faceId, int bIsDelPicture)
{
	int ret = -1;

	HK_DELETE_PERSON_INFO_LIST personDel;
	memset(&personDel, 0, sizeof(personDel));
	personDel.nValid = 1;

	personDel.struDeletePersonInfo[0].nValid = 1;
	personDel.struDeletePersonInfo[0].nGroupID = faceGroupId;
	personDel.struDeletePersonInfo[0].nPersonID = faceId;
	snprintf(personDel.struDeletePersonInfo[0].strDeletePicture, sizeof(personDel.struDeletePersonInfo[0].strDeletePicture),
			"%s", zw_int2bool(bIsDelPicture));

	FaceRecoFacePersonInfoDel FaceDel;
	FaceDel.SetParam(&personDel, sizeof(personDel));
	int rc = AS_CGIOperator(OPT_PUT, FaceDel);
	if (0 == rc)
	{
		ret = 0;
	}

	return ret;
}

int AS_FaceReco_PersonInfoUnbind(long long faceGroupId, long long faceId)
{
	int ret = -1;

	HK_BIND_AND_UNBIND_PERSON_INFO_LIST personUnbind;
	memset(&personUnbind, 0, sizeof(personUnbind));
	personUnbind.nValid = 1;
	personUnbind.struBindAndUnBindPersonInfo[0].nValid = 1;
	personUnbind.struBindAndUnBindPersonInfo[0].nSrcGroupID = 0;
	personUnbind.struBindAndUnBindPersonInfo[0].nGroupID = faceGroupId;
	personUnbind.struBindAndUnBindPersonInfo[0].nPersonID = faceId;

	FaceRecoFacePersonInfoUnbind FaceUnbind;
	FaceUnbind.SetParam(&personUnbind, sizeof(personUnbind));
	int rc = AS_CGIOperator(OPT_PUT, FaceUnbind);
	if (0 == rc)
	{
		ret = 0;
	}

	return ret;
}

int AS_FaceReco_PersonInfoBind(long long faceGroupId, long long faceId)
{
	int ret = -1;

	HK_BIND_AND_UNBIND_PERSON_INFO_LIST personBind;
	memset(&personBind, 0, sizeof(personBind));
	personBind.nValid = 1;
	personBind.struBindAndUnBindPersonInfo[0].nValid = 1;
	personBind.struBindAndUnBindPersonInfo[0].nSrcGroupID = 1;
	personBind.struBindAndUnBindPersonInfo[0].nGroupID = faceGroupId;
	personBind.struBindAndUnBindPersonInfo[0].nPersonID = faceId;

	FaceRecoFacePersonInfoBind FaceBind;
	FaceBind.SetParam(&personBind, sizeof(personBind));
	int rc = AS_CGIOperator(OPT_PUT, FaceBind);
	if (0 == rc)
	{
		ret = 0;
	}

	return ret;
}

int AS_FaceReco_PersonInfoModifyBasic(FaceGroupFaceInfo_t *pInfo)
{
	if (NULL == pInfo)
	{
		err("param err! Info[%p]\n", pInfo);
		return -1;
	}

	int ret = -1;
	HK_MODIFY_PERSON_INFO personInfo;
	memset(&personInfo, 0, sizeof(personInfo));
	personInfo.nValid = 1;
	personInfo.nPersonID = (long long)pInfo->faceId;
	snprintf(personInfo.strName, sizeof(personInfo.strName), "%s", pInfo->name);
	snprintf(personInfo.strSex, sizeof(personInfo.strSex), "%s", 
						zw_gender_enum2str(pInfo->gender));
	snprintf(personInfo.strCertificateType, sizeof(personInfo.strCertificateType), "%s", 
						zw_cardtype_enum2str(pInfo->cardType));
	snprintf(personInfo.strCertificateID, sizeof(personInfo.strCertificateID), "%s", 
						pInfo->cardId);
	snprintf(personInfo.strCountry, sizeof(personInfo.strCountry), "%s", 
						pInfo->country);
	snprintf(personInfo.strAddress, sizeof(personInfo.strAddress), "%s", 
						pInfo->provinceCity);
	snprintf(personInfo.strBirthday, sizeof(personInfo.strBirthday), "%s", 
						pInfo->birthday);

	FaceRecoFacePersonInfoModifyBase infoModifyBasic;
	infoModifyBasic.SetParam(&personInfo, sizeof(personInfo));
	int rc = AS_CGIOperator(OPT_PUT, infoModifyBasic);
	if (0 == rc)
	{
		ret = 0;
	}

	return ret;
}

int AS_FaceReco_PersonInfoModifyPicture(long long faceId, int faceImageID, char *PicBuf, int bufLen)
{
	if (NULL == PicBuf || bufLen <= 0)
	{
		err("param err! PicBuf[%p] bufLen[%d]\n", PicBuf, bufLen);
		return -1;
	}

	int ret = -1;
	
	// auth
	userAuth auth;
	memset(&auth, 0, sizeof(auth));
	auth.type = 1;
	snprintf(auth.username, sizeof(auth.username), "admin");

	// recv-data
	char *recvBuf = NULL;
	int recvLen = 0;
	// send-data
	struct mbuf send_mbuf;
	mbuf_init(&send_mbuf, 0);
	{
		mbuf_append_form_long_long(&send_mbuf, "PersonID", faceId);
		mbuf_append_form_int(&send_mbuf, "PictureID", faceImageID);
		mbuf_append_form_jpg(&send_mbuf, "file", "face.jpg");
		mbuf_append(&send_mbuf, PicBuf, bufLen);
		char form_end[64] = "\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW--";
		mbuf_append(&send_mbuf, form_end, strlen(form_end));
	}
	int rc = HTTP_ON_USBPIPE_POST(&auth, "/FaceGroup/UpdateFaceImageInfoByPerson", 
				"content-type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW", 
				NULL, (void*)send_mbuf.buf, send_mbuf.len, (void**)&recvBuf, &recvLen);
	if (0 == rc)
	{
		ret = 0;
	}
	if (recvBuf != NULL)
	{
		free(recvBuf);
	}
	mbuf_free(&send_mbuf);

	return ret;
}

/************************************************* 记录查询相关 ****************************************************/

int AS_FaceReco_PersonInfoQueryListCount(long long faceGroupId, FaceGroupSearchInfo_t *faceGroupSearchInfo, int *pFaceCount)
{
	if (NULL == faceGroupSearchInfo || NULL == pFaceCount)
	{
		err("param err! faceGroupSearchInfo[%p] pFaceCount[%p]\n", faceGroupSearchInfo, pFaceCount);
		return -1;
	}

	int ret = -1;
	int cnt = 0;

	HK_PERSON_INFO_COUNT infoCount;
	memset(&infoCount, 0, sizeof(infoCount));
	infoCount.nValid = 1;
	infoCount.nGroupID = faceGroupId;
	snprintf(infoCount.strName, sizeof(infoCount.strName), "%s", faceGroupSearchInfo->name);
	snprintf(infoCount.strSex, sizeof(infoCount.strSex), "%s", zw_gender_enum2str(faceGroupSearchInfo->gender));
	snprintf(infoCount.strCertificateType, sizeof(infoCount.strCertificateType), "%s", 
						zw_cardtype_enum2str(faceGroupSearchInfo->cardType));
	snprintf(infoCount.strCertificateID, sizeof(infoCount.strCertificateID), "%s", 
						faceGroupSearchInfo->cardId);
	snprintf(infoCount.strCountry, sizeof(infoCount.strCountry), "%s", 
						faceGroupSearchInfo->country);
	snprintf(infoCount.strAddress, sizeof(infoCount.strAddress), "%s", 
						faceGroupSearchInfo->provinceCity);
	snprintf(infoCount.strStartBirthday, sizeof(infoCount.strStartBirthday), "%s", 
						faceGroupSearchInfo->startBirthday);
	snprintf(infoCount.strStopBirthday, sizeof(infoCount.strStopBirthday), "%s", 
						faceGroupSearchInfo->endBirthday);

	FaceRecoFacePersonInfoQueryCount infoQueryCount;
	infoQueryCount.SetParam(&infoCount, sizeof(infoCount));
	int rc = AS_CGIOperator(OPT_PUT, infoQueryCount);
	if (0 == rc)
	{
		infoQueryCount.GetParam(&infoCount, sizeof(infoCount));
		if (1 == infoCount.nValid)
		{
			cnt = infoCount.nPersonInfoCount;
			ret = 0;
		}
	}
	*pFaceCount= cnt; 

	return ret;
}

int AS_FaceReco_PersonInfoQueryListBasic(long long faceGroupId, FaceGroupSearchInfo_t *filter, int pageNum, int *pageSize, FaceGroupFaceInfo_t *pFaceInfo)
{
	if (NULL == filter || NULL == pFaceInfo || NULL == pageSize || *pageSize <= 0)
	{
		err("param err! filter[%p] pFaceInfo[%p] pageSize[%p] *pageSize[%d]\n", filter, pFaceInfo, pageSize, *pageSize);
		return -1;
	}

	int ret = -1;
	int listSize = 0;
	HK_PERSON_INFO_LIST infoList;
	memset(&infoList, 0, sizeof(infoList));
	infoList.nValid = 1;

	infoList.nGroupID = faceGroupId;
	snprintf(infoList.strName, sizeof(infoList.strName), "%s", filter->name);
	snprintf(infoList.strSex, sizeof(infoList.strSex), "%s", zw_gender_enum2str(filter->gender));
	snprintf(infoList.strCertificateType, sizeof(infoList.strCertificateType), "%s", 
						zw_cardtype_enum2str(filter->cardType));
	snprintf(infoList.strCertificateID, sizeof(infoList.strCertificateID), "%s", 
						filter->cardId);
	snprintf(infoList.strCountry, sizeof(infoList.strCountry), "%s", 
						filter->country);
	snprintf(infoList.strAddress, sizeof(infoList.strAddress), "%s", 
						filter->provinceCity);
	snprintf(infoList.strStartBirthday, sizeof(infoList.strStartBirthday), "%s", 
						filter->startBirthday);
	snprintf(infoList.strStopBirthday, sizeof(infoList.strStopBirthday), "%s", 
						filter->endBirthday);
	infoList.nPageNum = pageNum;
	infoList.nPageSize = *pageSize;
	infoList.pPersonInfoList = (HK_PERSON_INFO*)malloc(infoList.nPageSize*sizeof(HK_PERSON_INFO));
	memset(infoList.pPersonInfoList, 0, infoList.nPageSize*sizeof(HK_PERSON_INFO));

	FaceRecoFacePersonInfoQueryBasicList infoQueryInfo;
	infoQueryInfo.SetParam(&infoList, sizeof(infoList));
	int rc = AS_CGIOperator(OPT_PUT, infoQueryInfo);
	if (0 == rc)
	{
		infoQueryInfo.GetParam(&infoList, sizeof(infoList));
		if (1 == infoList.nValid)
		{
			listSize = 0;
			HK_PERSON_INFO *node = NULL;
			
			for (int i=0; i < infoList.nPageSize; i++)
			{
				node = &infoList.pPersonInfoList[i];
				if (0 == node->nValid)
				{
					break;
				}
				pFaceInfo[listSize].faceId = node->nPersonID;
				snprintf(pFaceInfo[listSize].name, sizeof(pFaceInfo[listSize].name), "%s",
							node->strName);
				pFaceInfo[listSize].gender = zw_gender_str2enum(node->strSex);
				snprintf(pFaceInfo[listSize].birthday, sizeof(pFaceInfo[listSize].birthday), "%s",
							node->strBirthday);
				snprintf(pFaceInfo[listSize].country, sizeof(pFaceInfo[listSize].country), "%s",
							node->strCountry);
				snprintf(pFaceInfo[listSize].provinceCity, sizeof(pFaceInfo[listSize].provinceCity), "%s",
							node->strAddress);
				pFaceInfo[listSize].cardType = zw_cardtype_str2enum(node->strCertificateType);
				snprintf(pFaceInfo[listSize].cardId, sizeof(pFaceInfo[listSize].cardId), "%s",
							node->strCertificateID);
				// pFaceInfo[listSize].featrueState
				listSize++;
			}
			ret = 0;
		}
	}
	if (infoList.pPersonInfoList != NULL)
	{
		free(infoList.pPersonInfoList);
	}
	*pageSize = listSize;

	return ret;
}

int AS_FaceReco_PersonInfoQueryPictureById(long long personId, int faceImageID, char *buf, int *bufLen)
{
	if (NULL == buf || NULL == bufLen || *bufLen <= 0)
	{
		err("param err! buf[%p] bufLen[%p] *bufLen[%d]\n", buf, bufLen, *bufLen);
		return -1;
	}

	int ret = -1;

	HK_IMAGE_INFO_PARAM infoParam;
	memset(&infoParam, 0, sizeof(infoParam));
	infoParam.nValid = 1;
	infoParam.nPersonID = personId;
	infoParam.nPictureID = faceImageID;

	// auth
	userAuth auth;
	memset(&auth, 0, sizeof(auth));
	auth.type = 1;
	snprintf(auth.username, sizeof(auth.username), "admin");

	// recv-data
	char *recvBuf = NULL;
	int recvLen = 0;
	char *req = (char*)malloc(10*1024);
	int parseLen = 10*1024;

	FaceRecoFacePersonInfoQueryPicture infoQueryPic;
	infoQueryPic.SetParam(&infoParam, sizeof(infoParam));
	infoQueryPic.ToXML(req, parseLen);
	int rc = HTTP_ON_USBPIPE_POST(&auth, "/FaceGroup/QueryPersonImageInfo", "content-type: text/plain;charset=UTF-8", 
									NULL, (void*)req, parseLen, (void**)&recvBuf, &recvLen);
	if (0 == rc)
	{
		if (*bufLen >= recvLen)
		{
			// {
			// 	std::ofstream openfile("abc_name01.jpg");
			// 	openfile.write(recvBuf, recvLen);
			// 	openfile.close();
			// }
			memcpy(buf, recvBuf, recvLen);
			*bufLen = recvLen;
			ret = 0;
		}
		else
		{
			err("bufLen too small! buflen[%d] want[%d]\n", *bufLen, recvLen);
		}
	}
	if (recvBuf != NULL)
	{
		free(recvBuf);
	}

	return ret;
}

int AS_FaceReco_PersonInfoQueryBasicById(long long faceGroupId, long long personId, FaceGroupFaceInfo_t *pFaceInfo)
{
	if (NULL == pFaceInfo)
	{
		err("param err! pFaceInfo[%p]\n", pFaceInfo);
		return -1;
	}

	int ret = -1;

	HK_PERSON_INFO infoParam;
	memset(&infoParam, 0, sizeof(infoParam));
	infoParam.nValid = 1;
	infoParam.nGroupID = faceGroupId;
	infoParam.nPersonID = personId;

	FaceRecoFacePersonInfoQueryByID infoQueryByID;
	infoQueryByID.SetParam(&infoParam, sizeof(infoParam));
	int rc = AS_CGIOperator(OPT_PUT, infoQueryByID);
	if (0 == rc)
	{
		infoQueryByID.GetParam(&infoParam, sizeof(infoParam));
		if (1 == infoParam.nValid)
		{
			pFaceInfo->faceId = (unsigned long long)infoParam.nPersonID;
			snprintf(pFaceInfo->name, sizeof(pFaceInfo->name), "%s", infoParam.strName);
			pFaceInfo->gender = (strcasecmp(infoParam.strSex, "male") == 0)?FACEGROUPGRENDER_MAN : FACEGROUPGRENDER_WOMAN;
			snprintf(pFaceInfo->birthday, sizeof(pFaceInfo->birthday), "%s", infoParam.strBirthday);
			snprintf(pFaceInfo->country, sizeof(pFaceInfo->country), "%s", infoParam.strCountry);
			snprintf(pFaceInfo->provinceCity, sizeof(pFaceInfo->provinceCity), "%s", infoParam.strAddress);
			pFaceInfo->cardType = (strcasecmp(infoParam.strCertificateType, "passport") == 0)?FACEGROUPPASSORD : FACEGROUPIDENTITYCARD;
			snprintf(pFaceInfo->cardId, sizeof(pFaceInfo->cardId), "%s", infoParam.strCertificateID);

			ret = 0;
		}
	}

	return ret;
}

int AS_FaceReco_SendFrame(void *buf, int len)
{
	AS_TASK_REQUEST req;
	AS_TASK_RESULT result;

	memset(&req, 0, sizeof(AS_TASK_REQUEST));
	memset(&result, 0, sizeof(AS_TASK_RESULT));

	req.cmd = AS_TASK_STREAM_FRAME;
	req.paramLen = len;
	req.param = buf;

	int rc = AS_TASK_Run(&req, &result);
	if (0 == rc)
	{
		if (result.param)
		{
			free(result.param);
		}
	}

	return rc;
}

int AS_FaceReco_Check_Online(int *status)
{
	if (NULL == status)
	{
		err("Param Error! status[%p]\n", status);
		return -1;
	}

	int ret = -1;
	AS_TASK_REQUEST req;
	AS_TASK_RESULT result;

	memset(&req, 0, sizeof(AS_TASK_REQUEST));
	memset(&result, 0, sizeof(AS_TASK_RESULT));

	req.cmd = AS_TASK_CHECK_ONLINE;

	int rc = AS_TASK_Run(&req, &result);
	if (0 == rc)
	{
		if (result.paramLen == sizeof(int))
		{
			*status = *(int*)result.param;
		}
		if (result.param)
		{
			free(result.param);
		}
		ret = 0;
	}

	return ret;
}

int AS_FaceGroup_Rule_Get(FACEPRO_RULE_CFG *rule)
{
	if (NULL == rule)
	{
		err("param err! rule[%p]\n", rule);
		return -1;
	}

	int ret = -1;

	HK_FACE_RECO_RULE_LIST infoParam;
	memset(&infoParam, 0, sizeof(infoParam));
	infoParam.nValid = 1;
	
	FaceRecoRule recoRule;
	recoRule.SetParam(&infoParam, sizeof(infoParam));
	int rc = AS_CGIOperator(OPT_GET, recoRule);
	if (0 == rc)
	{
		recoRule.GetParam(&infoParam, sizeof(infoParam));
		if (1 == infoParam.nValid)
		{
			rule->bEnable = infoParam.nValid;
			FACEPRO_RULE *node = NULL;
			HK_FACE_RECO_RULE *recoRule = NULL;
			for (int i = 0; i < MAX_CHANNEL_RULE_COUNT; i++)
			{
				recoRule = &infoParam.struFaceRecoRule[i];
				node = &rule->stRules[i];
				node->bEnable = zw_bool2int(recoRule->strEnable);
				// 触发机制 
				node->nTrigerRule = zw_LinkageStrategy2int(recoRule->strRecognitionRule);
				// 阈值
				memset(node->stThres, 0, sizeof(node->stThres));
				if (1 == recoRule->struScheduleRecoThresholdList.nValid)
				{
					HK_SCHEDULE_RECO_THRESHOLD *recoThreshold = NULL;
					for (int j = 0; j < MAX_RECO_THRESHOLD_SCHEDULE_COUNT; j++)
					{
						recoThreshold = &recoRule->struScheduleRecoThresholdList.struScheduleRecoThreshold[j];
						node->stThres[j].bEnable = recoThreshold->nValid;
						node->stThres[j].nStartTime = 0;
						node->stThres[j].nStopTime = 0;
						node->stThres[j].nThres = recoThreshold->nRecoThreshold;
					}
				}
				// 比对上限
				node->nCompareLimit = recoRule->nCompareLimit;
				// 脸库
				memset(node->stFaceGrp, 0, sizeof(node->stFaceGrp));
				if (1 == recoRule->struFaceGroupList.nValid)
				{
					HK_RULE_FACE_GROUP *recoGroup = NULL;
					for (int j = 0; j < PUB_MAX_FACE_GROUP_COUNT; j++)
					{
						recoGroup = &recoRule->struFaceGroupList.struRuleFaceGroup[j];
						node->stFaceGrp[j].bValid = recoGroup->nValid;
						node->stFaceGrp[j].bEnable = zw_bool2int(recoGroup->strEnable);
						node->stFaceGrp[j].u64FaceGrpId = (uint64_t)recoGroup->nGroupID;
						snprintf((char*)node->stFaceGrp[j].szFaceGrpName, sizeof(node->stFaceGrp[j].szFaceGrpName),
							"%s", recoGroup->strGroupName);
					}
				}
				// 布放时间
				HK_SCHEDULE_TranslateTo_FaceSchedule(&recoRule->struSchedule, &node->stSchedule);
			}
			ret = 0;
		}
	}

	return ret;
}

int AS_FaceGroup_Rule_Set(const FACEPRO_RULE_CFG *rule)
{
	if (NULL == rule)
	{
		err("param err! rule[%p]\n", rule);
		return -1;
	}

	int ret = -1;
	HK_FACE_RECO_RULE *recoRule = NULL;

	HK_FACE_RECO_RULE_LIST infoParam;
	memset(&infoParam, 0, sizeof(infoParam));
	
	infoParam.nValid = rule->bEnable;
	for (int i = 0; i < MAX_CHANNEL_RULE_COUNT; i++)
	{
		recoRule = &infoParam.struFaceRecoRule[i];
		
		recoRule->nValid = 1;
		// 使能
		snprintf(recoRule->strEnable, sizeof(recoRule->strEnable), "%s", zw_int2bool(rule->stRules[i].bEnable));
		// 触发机制
		snprintf(recoRule->strRecognitionRule, sizeof(recoRule->strRecognitionRule), 
					"%s", zw_int2LinkageStrategy(rule->stRules[i].nTrigerRule));
		// 阈值
		recoRule->struScheduleRecoThresholdList.nValid = 1;
		HK_SCHEDULE_RECO_THRESHOLD *recoThreshold = NULL;
		for (int j = 0; j < MAX_RECO_THRESHOLD_SCHEDULE_COUNT; j++)
		{
			recoThreshold = &recoRule->struScheduleRecoThresholdList.struScheduleRecoThreshold[j];
			recoThreshold->nValid = rule->stRules[i].stThres[j].bEnable;
			//recoThreshold->strStartTime;
			snprintf(recoThreshold->strStartTime, sizeof(recoThreshold->strStartTime),
					"000000");
			//recoThreshold->strStopTime;
			snprintf(recoThreshold->strStopTime, sizeof(recoThreshold->strStopTime),
					"000000");
			recoThreshold->nRecoThreshold = (int)rule->stRules[i].stThres[j].nThres;
		}
		// 比对上限
		recoRule->nCompareLimit = rule->stRules[i].nCompareLimit;
		// 脸库
		recoRule->struFaceGroupList.nValid = 1;
		HK_RULE_FACE_GROUP *recoGroup = NULL;
		for (int j = 0; j < HK_MAX_GROUP_FACE_COUNT; j++)
		{
			recoGroup = &recoRule->struFaceGroupList.struRuleFaceGroup[j];
			recoGroup->nValid = rule->stRules[i].stFaceGrp[j].bValid;
			snprintf(recoGroup->strEnable, sizeof(recoGroup->strEnable),
				"%s", zw_int2bool(rule->stRules[i].stFaceGrp[j].bEnable));
			recoGroup->nGroupID = (long long)rule->stRules[i].stFaceGrp[j].u64FaceGrpId;
			snprintf(recoGroup->strGroupName, sizeof(recoGroup->strGroupName),
					"%s", (char*)rule->stRules[i].stFaceGrp[j].szFaceGrpName);
		}
		// 布放时间
		FaceSchedule_TranslateTo_HK_SCHEDULE((FaceSchedule*)&rule->stRules[i].stSchedule, &recoRule->struSchedule);
	}

	FaceRecoRule Rule;
	Rule.SetParam(&infoParam, sizeof(infoParam));
	int rc = AS_CGIOperator(OPT_PUT, Rule);
	if (0 == rc)
	{
		ret = 0;
	}

	return ret;
}

int AS_System_Time_Get(time_t *t)
{
	if (NULL == t)
	{
		err("param err! t[%p]\n", t);
		return -1;
	}
	int ret = -1;

	HK_SYSTEM_TIME infoParam;
	memset(&infoParam, 0, sizeof(infoParam));
	infoParam.nValid = 1;
	
	SystemTime sysTime;
	sysTime.SetParam(&infoParam, sizeof(infoParam));
	int rc = AS_CGIOperator(OPT_GET, sysTime);
	if (0 == rc)
	{
		sysTime.GetParam(&infoParam, sizeof(infoParam));
		if (1 == infoParam.nValid)
		{
			info("strDateTimeFormat[%s] strTimeFormat[%s] strSystemTime[%s]\n",
				infoParam.strDateTimeFormat, infoParam.strTimeFormat, infoParam.strSystemTime);
			ret = 0;
		}
	}
	return ret;
}

int AS_System_Time_Set(time_t *t)
{
	if (NULL == t)
	{
		err("param err! t[%p]\n", t);
		return -1;
	}
	int ret = -1;

	HK_SYSTEM_TIME infoParam;
	memset(&infoParam, 0, sizeof(infoParam));
	{
		// 填充参数
	}
	SystemTime sysTime;
	sysTime.SetParam(&infoParam, sizeof(infoParam));
	int rc = AS_CGIOperator(OPT_PUT, sysTime);
	if (0 == rc)
	{
		ret = 0;
	}
	return ret;
}

int AS_System_DevInfo_Get(int *info)
{
	if (NULL == info)
	{
		err("param err! info[%p]\n", info);
		return -1;
	}
	int ret = -1;

	HK_SYSTEM_DEVICEINFO infoParam;
	memset(&infoParam, 0, sizeof(infoParam));
	infoParam.nValid = 1;
	
	SystemInfo sysInfo;
	sysInfo.SetParam(&infoParam, sizeof(infoParam));
	int rc = AS_CGIOperator(OPT_GET, sysInfo);
	if (0 == rc)
	{
		sysInfo.GetParam(&infoParam, sizeof(infoParam));
		if (1 == infoParam.nValid)
		{
			info("SoftVersion[%s]\n", infoParam.strSoftWareVersion);
			ret = 0;
		}
	}
	return ret;
}

int AS_FaceSnap_Filter_Get(int ch, FaceDetectRegion *f)
{
	if (NULL == f)
	{
		err("param err! f[%p]\n", f);
		return -1;
	}
	int ret = -1;

	HK_FACE_FILTER_REGION infoParam;
	memset(&infoParam, 0, sizeof(infoParam));
	infoParam.nValid = 1;
	
	FaceSnapshotFilterRegion filterRegion(ch, 0);
	filterRegion.SetParam(&infoParam, sizeof(infoParam));
	int rc = AS_CGIOperator(OPT_GET, filterRegion);
	if (0 == rc)
	{
		filterRegion.GetParam(&infoParam, sizeof(infoParam));
		if (1 == infoParam.nValid)
		{
			f->m_MaxWidth = infoParam.struMaxFaceRegionSize.nWidth;
			f->m_MaxHight = infoParam.struMaxFaceRegionSize.nHeight;
			f->m_MinWidth = infoParam.struMinFaceRegionSize.nWidth;
			f->m_MinHight = infoParam.struMinFaceRegionSize.nHeight;
			ret = 0;
		}
	}
	return ret;
}

int AS_FaceSnap_Filter_Set(int ch, const FaceDetectRegion *f)
{
	if (NULL == f)
	{
		err("param err! t[%p]\n", f);
		return -1;
	}
	int ret = -1;

	HK_FACE_FILTER_REGION infoParam;
	memset(&infoParam, 0, sizeof(infoParam));
	{
		// 填充参数
		infoParam.nValid = 1;
		infoParam.nChannelID = ch;
		infoParam.struMaxFaceRegionSize.nWidth = f->m_MaxWidth;
		infoParam.struMaxFaceRegionSize.nHeight = f->m_MaxHight;
		infoParam.struMinFaceRegionSize.nWidth = f->m_MaxWidth;
		infoParam.struMinFaceRegionSize.nHeight = f->m_MinHight;
	}
	FaceSnapshotFilterRegion filterRegion(ch, 0);
	filterRegion.SetParam(&infoParam, sizeof(infoParam));
	int rc = AS_CGIOperator(OPT_PUT, filterRegion);
	if (0 == rc)
	{
		ret = 0;
	}
	return ret;
}

int AS_FaceSnap_Region_Get(int ch, FilerFaceDetectRegionParam *f)
{
	if (NULL == f)
	{
		err("param err! f[%p]\n", f);
		return -1;
	}
	int ret = -1;

	HK_FACE_DETECT_REGION infoParam;
	memset(&infoParam, 0, sizeof(infoParam));
	infoParam.nValid = 1;
	
	FaceSnapshotDetectRegion detectRegion(ch, 0);
	detectRegion.SetParam(&infoParam, sizeof(infoParam));
	int rc = AS_CGIOperator(OPT_GET, detectRegion);
	if (0 == rc)
	{
		detectRegion.GetParam(&infoParam, sizeof(infoParam));
		if (1 == infoParam.nValid)
		{
			f->FaceDetectionArea.left = infoParam.struFaceDetectRegion.nTopLeftX;
			f->FaceDetectionArea.top = infoParam.struFaceDetectRegion.nTopLeftY;
			f->FaceDetectionArea.right = infoParam.struFaceDetectRegion.nBottomRightX;
			f->FaceDetectionArea.bottom = infoParam.struFaceDetectRegion.nBottomRightY;
			ret = 0;
		}
	}
	return ret;
}

int AS_FaceSnap_Region_Set(int ch, const FilerFaceDetectRegionParam *f)
{
	if (NULL == f)
	{
		err("param err! t[%p]\n", f);
		return -1;
	}
	int ret = -1;

	HK_FACE_DETECT_REGION infoParam;
	memset(&infoParam, 0, sizeof(infoParam));
	{
		infoParam.nValid = 1;
		infoParam.nChannelID = ch;
		infoParam.struFaceDetectRegion.nTopLeftX = f->FaceDetectionArea.left;
		infoParam.struFaceDetectRegion.nTopLeftY = f->FaceDetectionArea.top;
		infoParam.struFaceDetectRegion.nBottomRightX = f->FaceDetectionArea.right;
		infoParam.struFaceDetectRegion.nBottomRightY = f->FaceDetectionArea.bottom;
	}
	FaceSnapshotDetectRegion detectRegion(ch, 0);
	detectRegion.SetParam(&infoParam, sizeof(infoParam));
	int rc = AS_CGIOperator(OPT_PUT, detectRegion);
	if (0 == rc)
	{
		ret = 0;
	}
	return ret;
}

int AS_FaceSnap_Config_Get(int ch, FaceConfig *f)
{
	if (NULL == f)
	{
		err("param err! f[%p]\n", f);
		return -1;
	}
	int ret = -1;

	HK_FACE_SNAP_CONFIG infoParam;
	memset(&infoParam, 0, sizeof(infoParam));
	infoParam.nValid = 1;
	
	FaceSnapshotConfig config(ch, 0);
	config.SetParam(&infoParam, sizeof(infoParam));
	int rc = AS_CGIOperator(OPT_GET, config);
	if (0 == rc)
	{
		config.GetParam(&infoParam, sizeof(infoParam));
		if (1 == infoParam.nValid)
		{
			f->nFaceSnapMode = zw_capmode_str2enum(infoParam.cFaceSnapMode);
			f->CaptureRange = zw_cutmode_str2enum(infoParam.cCutoutMode);
			// 自动模式
			if (1 == infoParam.struFaceSnapModeAuto.nValid)
			{
				f->Auto.nSnapshotCount = infoParam.struFaceSnapModeAuto.nSnapshotCount;
				f->Auto.nSnapShotInterval = infoParam.struFaceSnapModeAuto.nSnapShotInterval;
			}
			// 快速模式
			if (1 == infoParam.struFaceSnapModeFast.nValid)
			{
				f->Fast.nSnapshotCount = infoParam.struFaceSnapModeFast.nSnapshotCount;
			}
			// 最优选择模式
			if (1 == infoParam.struFaceSnapModeOptimal.nValid)
			{
				f->Optimal.nSnapshotCount = infoParam.struFaceSnapModeOptimal.nSnapshotCount;
			}
			// 自定义模式
			if (1 == infoParam.struFaceSnapModeCustom.nValid)
			{
				f->Custom.nSnapshotCount = infoParam.struFaceSnapModeCustom.nSnapshotCount;
				f->Custom.nPicQualityThreshold = infoParam.struFaceSnapModeCustom.nPicQualityThreshold;
			}
			ret = 0;
		}
	}
	return ret;
}

int AS_FaceSnap_Config_Set(int ch, const FaceConfig *f)
{
	if (NULL == f)
	{
		err("param err! t[%p]\n", f);
		return -1;
	}
	int ret = -1;

	HK_FACE_SNAP_CONFIG infoParam;
	memset(&infoParam, 0, sizeof(infoParam));
	{
		infoParam.nValid = 1;
		infoParam.nChannelID = ch;
		// 抓图模式
		snprintf(infoParam.cFaceSnapMode, sizeof(infoParam.cFaceSnapMode),
				"%s", zw_capmode_enum2str((FACECAPMODE_T)f->nFaceSnapMode));
		// 抠图模式
		snprintf(infoParam.cCutoutMode, sizeof(infoParam.cCutoutMode),
				"%s", zw_cutmode_enum2str(f->CaptureRange));
		// 自动模式
		infoParam.struFaceSnapModeAuto.nValid = 1;
		infoParam.struFaceSnapModeAuto.nSnapshotCount = f->Auto.nSnapshotCount;
		infoParam.struFaceSnapModeAuto.nSnapShotInterval = f->Auto.nSnapShotInterval;
		// 快速模式
		infoParam.struFaceSnapModeFast.nValid = 1;
		infoParam.struFaceSnapModeFast.nSnapshotCount = f->Fast.nSnapshotCount;
		// 最优选择模式
		infoParam.struFaceSnapModeOptimal.nValid = 1;
		infoParam.struFaceSnapModeOptimal.nSnapshotCount = f->Optimal.nSnapshotCount;
		// 自定义模式
		infoParam.struFaceSnapModeCustom.nValid = 1;
		infoParam.struFaceSnapModeCustom.nSnapshotCount = f->Custom.nSnapshotCount;
		infoParam.struFaceSnapModeCustom.nPicQualityThreshold = f->Custom.nPicQualityThreshold;
	}
	FaceSnapshotConfig config(ch, 0);
	config.SetParam(&infoParam, sizeof(infoParam));
	int rc = AS_CGIOperator(OPT_PUT, config);
	if (0 == rc)
	{
		ret = 0;
	}
	return ret;
}
