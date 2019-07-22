#include <string>
#include <fstream>

#include "zw_usbpipe_util.h"
#include "zw_usbpipe_uptp_api.h"
#include "zw_aistick_sys.h"
#include "zw_aistick_api.h"

void query_all_group()
{
	FaceGroupInfo_t group[10];
	memset(group, 0, sizeof(group));
	int groupNum = 10;
	AS_FaceReco_GroupQueryAll(group, &groupNum);
	for (int i=0; i<groupNum; i++)
	{
		info("Idx[%d] GroupId[%llu] groupName[%s] createTime[%s], Size[%u] nValidState[%d]\n", 
			i, group[i].faceGroupId, group[i].groupName, group[i].createTime, group[i].enrolment, group[i].nValidState);
	}

	return;
}

void test_xml()
{
	int rc = 0;
	// 查询脸库
	if (1)
	{
		FaceGroupInfo_t group[10];
		memset(group, 0, sizeof(group));
		int groupNum = 10;
		AS_FaceReco_GroupQueryAll(group, &groupNum);
		for (int i=0; i<groupNum; i++)
		{
			info("Idx[%d] GroupId[%llu] groupName[%s] createTime[%s], Size[%u] nValidState[%d]\n", 
				i, group[i].faceGroupId, group[i].groupName, group[i].createTime, group[i].enrolment, group[i].nValidState);
		}
	}

	// 创建脸库
	if (0)
	{
		FaceGroupInfo_t info;
		memset(&info, 0, sizeof(info));
		snprintf(info.groupName, sizeof(info.groupName), "FaceGroupName02");
		rc = AS_FaceReco_GroupCreate(&info);
		if (0 == rc)
		{
			info("Create GroupID[%llu]\n", info.faceGroupId);
		}
	}

	// 查询脸库-ID
	if (0)
	{
		FaceGroupInfo_t info;
		memset(&info, 0, sizeof(info));
		info.faceGroupId = (unsigned long long)7;
		rc = AS_FaceReco_GroupQueryByGroupID(&info);
		if (0 == rc)
		{
			info("Create GroupID[%llu] GroupName[%s] CreatTime[%s] enrolment[%u] nValidState[%d]\n", 
			info.faceGroupId, info.groupName, info.createTime, info.enrolment, info.nValidState);
		}
	}

	// 修改人脸库
	if (0)
	{
		FaceGroupInfo_t info;
		memset(&info, 0, sizeof(info));
		info.faceGroupId = (unsigned long long)7;
		snprintf(info.groupName, sizeof(info.groupName), "ModifyName01");
		rc = AS_FaceReco_GroupModify(&info);
		if (0 == rc)
		{
			info("Modify OK!\n");
		}
	}

	// 查询脸库-ID
	if (0)
	{
		FaceGroupInfo_t info;
		memset(&info, 0, sizeof(info));
		info.faceGroupId = (unsigned long long)7;
		rc = AS_FaceReco_GroupQueryByGroupID(&info);
		if (0 == rc)
		{
			info("Create GroupID[%llu] GroupName[%s] CreatTime[%s] enrolment[%u] nValidState[%d]\n", 
			info.faceGroupId, info.groupName, info.createTime, info.enrolment, info.nValidState);
		}
	}

	// 删除人脸库
	if (0)
	{
		AS_FaceReco_GroupDelete(8, 1);
	}

	if (0)
	{
		FaceGroupInfo_t group[10];
		memset(group, 0, sizeof(group));
		int groupNum = 10;
		AS_FaceReco_GroupQueryAll(group, &groupNum);
		for (int i=0; i<groupNum; i++)
		{
			info("Idx[%d] GroupId[%llu] groupName[%s] createTime[%s], Size[%u] nValidState[%d]\n", 
				i, group[i].faceGroupId, group[i].groupName, group[i].createTime, group[i].enrolment, group[i].nValidState);
		}
	}

	return;
}

// 1. 查询全部脸库
// 2. 新增脸库 -> 查询脸库-ID -> 修改人脸库 -> 查询脸库-ID -> 删除脸库 -> 查询全部脸库

void group_test()
{
	return;
}

void faceinfo_test_basic()
{
	int rc = 0;

	// 查询脸库
	if (1)
	{
		FaceGroupInfo_t group[10];
		memset(group, 0, sizeof(group));
		int groupNum = 10;
		AS_FaceReco_GroupQueryAll(group, &groupNum);
		for (int i=0; i<groupNum; i++)
		{
			info("Idx[%d] GroupId[%llu] groupName[%s] createTime[%s], Size[%u] nValidState[%d]\n", 
				i, group[i].faceGroupId, group[i].groupName, group[i].createTime, group[i].enrolment, group[i].nValidState);
		}
	}

	// 检验人员信息是否合法
	if (0)
	{
		FaceGroupFaceInfo_t info;
		memset(&info, 0, sizeof(FaceGroupFaceInfo_t));
		rc = AS_FaceReco_PersonInfoCheck(&info);
		if (0 == rc)
		{
			info("FaceInfo Check Info!\n");
		}
	}
	// 人员信息查询 - PersonID
	if (1)
	{
		FaceGroupFaceInfo_t info;
		memset(&info, 0, sizeof(FaceGroupFaceInfo_t));
		rc = AS_FaceReco_PersonInfoQueryBasicById(7, 2, &info);
		if (0 == rc)
		{
			info("FaceID[%llu] Name[%s] gender[%d] birthday[%s] country[%s] provinceCity[%s] cardType[%d] cardId[%s]\n", 
				info.faceId, info.name, info.gender, info.birthday, info.country, info.provinceCity, info.cardType, info.cardId);
		}
	}
	// 人员信息修改 - 图片
	if (0)
	{
			char *buffer = NULL;
			int length = 0;
			{
				std::ifstream t;  
				t.open("face_modify.jpg");      // open input file
				t.seekg(0, std::ios::end);    // go to the end
				length = t.tellg();           // report location (this is the length)
				t.clear();
				t.seekg(0, std::ios::beg);    // go back to the beginning
				buffer = new char[length];    // allocate memory for a buffer of appropriate dimension
				t.read(buffer, length);       // read the whole file into the buffer

				t.close();
			}
		rc = AS_FaceReco_PersonInfoModifyPicture(2, 1, buffer, length);
		if (0 == rc)
		{
			info("FaceInfo Modify Picture!\n");
		}
	}
	// 人员信息查询 - 图片
	if (0)
	{
		char buf[25*1024];
		int bufLen = sizeof(buf);
		rc = AS_FaceReco_PersonInfoQueryPictureById(2, 1, buf, &bufLen);
		if (0 == rc)
		{
			info("Query Picture!\n");
		}
	}
	// 人员信息修改 - 基本信息
	if (0)
	{
		FaceGroupFaceInfo_t info;
		memset(&info, 0, sizeof(FaceGroupFaceInfo_t));
		rc = AS_FaceReco_PersonInfoQueryBasicById(7, 2, &info);
		if (0 == rc)
		{
			info("FaceID[%llu] Name[%s] gender[%d] birthday[%s] country[%s] provinceCity[%s] cardType[%d] cardId[%s]\n", 
				info.faceId, info.name, info.gender, info.birthday, info.country, info.provinceCity, info.cardType, info.cardId);
		}
		{
			info.gender = FACEGROUPGRENDER_MAN;
			snprintf(info.provinceCity, sizeof(info.provinceCity), "湖北省#十堰市");
			snprintf(info.birthday, sizeof(info.birthday), "20080101");
		}
		AS_FaceReco_PersonInfoCheck(&info);
		rc = AS_FaceReco_PersonInfoModifyBasic(&info);
		if (0 == rc)
		{
			info("FaceInfo Modify Basic!\n");
		}
		memset(&info, 0, sizeof(FaceGroupFaceInfo_t));
		rc = AS_FaceReco_PersonInfoQueryBasicById(7, 2, &info);
		if (0 == rc)
		{
			info("FaceID[%llu] Name[%s] gender[%d] birthday[%s] country[%s] provinceCity[%s] cardType[%d] cardId[%s]\n", 
				info.faceId, info.name, info.gender, info.birthday, info.country, info.provinceCity, info.cardType, info.cardId);
		}
	}
	// 人员信息解绑
	if (0)
	{
		rc = AS_FaceReco_PersonInfoUnbind(7, 2);
		if (0 == rc)
		{
			info("FaceInfo Unbind!\n");
		}
	}
	// 人员信息绑定
	if (0)
	{
		rc = AS_FaceReco_PersonInfoBind(7, 2);
		if (0 == rc)
		{
			info("FaceInfo Bind!\n");
		}
	}
	// 人员信息删除
	if (0)
	{
		rc = AS_FaceReco_PersonInfoDelete(7, 3, 1);
		if (0 == rc)
		{
			info("FaceInfo Del!\n");
		}
	}

	// 查询脸库
	if (0)
	{
		FaceGroupInfo_t group[10];
		memset(group, 0, sizeof(group));
		int groupNum = 10;
		AS_FaceReco_GroupQueryAll(group, &groupNum);
		for (int i=0; i<groupNum; i++)
		{
			info("Idx[%d] GroupId[%llu] groupName[%s] createTime[%s], Size[%u] nValidState[%d]\n", 
				i, group[i].faceGroupId, group[i].groupName, group[i].createTime, group[i].enrolment, group[i].nValidState);
		}
	}
	//人员信息查询 - 数量(过滤选中脸库)
	if (0)
	{
		int count = 0;
		FaceGroupSearchInfo_t filter;
		memset(&filter, 0, sizeof(FaceGroupSearchInfo_t));
		// snprintf(filter.name, sizeof(filter.name), " ");
		snprintf(filter.country, sizeof(filter.country), "中国");
		// snprintf(filter.startBirthday, sizeof(filter.startBirthday), " ");
		// snprintf(filter.endBirthday, sizeof(filter.endBirthday), " ");
		// snprintf(filter.provinceCity, sizeof(filter.provinceCity), " ");
		// snprintf(filter.cardId, sizeof(filter.cardId), " ");
		rc = AS_FaceReco_PersonInfoQueryListCount(7, &filter, &count);
		if (0 == rc)
		{
			info("FaceGroup[7] Count[%d]\n", count);
		}
	}
	//人员信息查询 - 基本信息(过滤选中脸库)
	if (1)
	{
		FaceGroupSearchInfo_t filter;
		memset(&filter, 0, sizeof(FaceGroupSearchInfo_t));
		snprintf(filter.country, sizeof(filter.country), "中国");
		FaceGroupFaceInfo_t pFaceInfo[30];
		memset(pFaceInfo, 0, sizeof(pFaceInfo));
		int pageSize = 30;
		rc = AS_FaceReco_PersonInfoQueryListBasic(7, &filter, 0, &pageSize, pFaceInfo);
		if (0 == rc)
		{
			FaceGroupFaceInfo_t *node = NULL;
			info("Size[%d]\n", pageSize);
			for (int i=0; i<pageSize; i++)
			{
				node = &pFaceInfo[i];
				info("Idx[%d] FaceID[%llu] Name[%s] gender[%d] birthday[%s] country[%s] provinceCity[%s] cardType[%d] cardId[%s]\n", 
					i, node->faceId, node->name, node->gender, node->birthday, node->country, node->provinceCity, node->cardType, node->cardId);

			}
		}
	}

	return;
}

void test_form_data()
{
	char *buffer = NULL;
	int length = 0;
	{
		std::ifstream t;  
		t.open("face02.jpg");      // open input file
		t.seekg(0, std::ios::end);    // go to the end
		length = t.tellg();           // report location (this is the length)
		t.clear();
		t.seekg(0, std::ios::beg);    // go back to the beginning
		buffer = new char[length];    // allocate memory for a buffer of appropriate dimension
		t.read(buffer, length);       // read the whole file into the buffer

		t.close();
	}
	//白奔杉#S2#B1985-01-01#C中国#P湖北省#武汉市#T2#M420585198501014231
	FaceGroupFaceInfo_t info;
	memset(&info, 0, sizeof(info));
	snprintf(info.name, sizeof(info.name), "白奔杉");
	info.gender = FACEGROUPGRENDER_WOMAN;
	snprintf(info.birthday, sizeof(info.birthday), "19850101");
	snprintf(info.country, sizeof(info.country), "中国");
	snprintf(info.provinceCity, sizeof(info.provinceCity), "湖北省#武汉市");
	info.cardType = FACEGROUPIDENTITYCARD;
	snprintf(info.cardId, sizeof(info.cardId), "420585198501014231");
	AS_FaceReco_PersonInfoCreate(7, 1, &info, 0, buffer, length);

	return;
}

void test_sys()
{
	int rc = 0;
	// 查询人脸识别模块的工作状态
	{
		int state = 0;
		int errCode = 0;

		rc = AS_FaceReco_GetWorkState(&state, &errCode);
		if (0 == rc)
		{
			info("State[%d], errCode[%d]\n", state, errCode);
		}
	}
	// 获取人脸识别的能力集
	{
		FaceRecoCap_t cap;
		memset(&cap, 0, sizeof(FaceRecoCap_t));
		rc = AS_FaceReco_GetCap(&cap);
		if (0 == rc)
		{
			info("bSupport[%d] nMaxFaceGroupNum[%d] nMaxPersonNumPerGroup[%d] nMaxPersonNumTotal[%d] nMaxImageSize[%d]\n", 
				cap.bSupportFaceReco, cap.nMaxFaceGroupNum, cap.nMaxPersonNumPerGroup, cap.nMaxPersonNumTotal, cap.nMaxImageSize);
			info("max[%d:%d] min[%d %d]\n", cap.snapShot.max.width, cap.snapShot.max.height, cap.snapShot.min.width, cap.snapShot.min.height);
		}
	}

	return;
}

static void test_frame()
{
	char abc[1536*1024];
	memset(abc, 1, sizeof(abc));

	AS_FaceReco_SendFrame(abc, sizeof(abc));

	return;
}

static void test_rule()
{
	char tmpBuf[1024];
	FACEPRO_RULE_CFG rule;

	memset(&rule, 0, sizeof(FACEPRO_RULE_CFG));
	int rc = AS_FaceGroup_Rule_Get(&rule);
	if (0 == rc)
	{
		// info("#=========================\n");
		// snprintf(tmpBuf, sizeof(tmpBuf), 
		// "# Enable - %d\n", rule.bEnable);
		// info("%s\n", tmpBuf);
		// FACEPRO_RULE *node = NULL;
		// for (int i = 0; i < 1; i++)
		// {
		// 	node = &rule.stRules[i];
		// 	snprintf(tmpBuf, sizeof(tmpBuf), 
		// 	"# Idx - %d\n"
		// 	"# Enable - %d\n"
		// 	"# TrigerRule - %d\n"
		// 	"# CompareLimit - %d\n",
		// 	i, node->bEnable, node->nTrigerRule,
		// 	node->nCompareLimit);
		// 	info("%s\n", tmpBuf);
		// 	// 阈值
		// 	snprintf(tmpBuf, sizeof(tmpBuf), 
		// 	"# Threshold-Idx - %d\n"
		// 	"# Threshold-Enable - %d\n"
		// 	"# Threshold-StartTime - %d\n"
		// 	"# Threshold-StopTime - %d\n"
		// 	"# Threshold-Thres - %d\n",
		// 	0, node->stThres[0].bEnable, node->stThres[0].nStartTime,
		// 	node->stThres[0].nStopTime, node->stThres[0].nThres);
		// 	info("%s\n", tmpBuf);

		// 	// 脸库
		// 	snprintf(tmpBuf, sizeof(tmpBuf), 
		// 	"# Threshold-Idx - %d\n"
		// 	"# Threshold-Enable - %d\n"
		// 	"# Threshold-GroupId - %llu\n"
		// 	"# Threshold-GroupName - %s\n",
		// 	0, node->stFaceGrp[0].bEnable, node->stFaceGrp[0].u64FaceGrpId,
		// 	(char*)node->stFaceGrp[0].szFaceGrpName);
		// 	info("%s\n", tmpBuf);
		// 	// 布放时间
		// 	{
		// 		info("# Schedule-State - %d\n", node->stSchedule.bytState);
		// 		for (int m = 0; m < 7; m++)
		// 		{
		// 			printf("Day[%d]: ", m);
		// 			for (int n = 0; n < 48; n++)
		// 			{
		// 				printf("%d ", node->stSchedule.dwTimeSeg[m][n]);
		// 			}
		// 			printf("\n");
		// 		}
		// 	}
		//}
		info("#=========================\n");
	}
	rule.stRules[0].stThres[0].nThres = 40;
	for (int i = 0; i < PUB_MAX_FACE_GROUP_COUNT; i++)
	{
		FACEPRO_RULE_GRP_INFO *grpInfo = &rule.stRules[0].stFaceGrp[i];
		if (grpInfo->bValid)
		{
			if (0 == i)
			{
				grpInfo->bEnable = 1;
			}
			else
			{
				grpInfo->bEnable = 0;
			}
		}
	}
	rule.stRules[0].nTrigerRule = 1;
	rc = AS_FaceGroup_Rule_Set(&rule);
	if (0 == rc)
	{
		info("set ok!\n");
	}

	memset(&rule, 0, sizeof(FACEPRO_RULE_CFG));
	rc = AS_FaceGroup_Rule_Get(&rule);

	return;
}

int main(int argc, char const *argv[])
{
	AS_SYS_Init();

	test_rule();
	//test_frame();
	// test_xml();
	//test_form_data();
	//faceinfo_test_basic();
	//query_all_group();
	//test_sys();

	getchar();
	info("\n");

	AS_SYS_UnInit();

	return 0;
}
