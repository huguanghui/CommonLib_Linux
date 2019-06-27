typedef struct tagHKFaceRecoFaceGroupName
{
    char groupName[32];
} HK_FACE_RECO_FACE_GROUP_NAME;

typedef struct tagHKFaceRecoFaceGroupID
{
    int groupID;
} HK_FACE_RECO_FACE_GROUP_ID;

typedef struct tagHKFaceRecoFaceGroupModify
{
    int groupID;
    char groupName[32];
} HK_FACE_RECO_FACE_GROUP_MODIFY;

typedef struct tagHKFaceRecoFaceGroupDel
{
    int groupID;
    int bDelBindFace;
} HK_FACE_RECO_FACE_GROUP_DEL;

typedef struct tagHKFaceRecoFaceGroupInfo
{
    int groupID;
    char groupName[32];
    char createTime[32];
    int personInfoCount;
    int bAvailableStatus;  //0 - unavailable 1 - available
} HK_FACE_RECO_FACE_GROUP_INFO;

typedef struct tagHKFaceRecoFaceGroupInfos
{
    int size;
    HK_FACE_RECO_FACE_GROUP_INFO infos[10];
} HK_FACE_RECO_FACE_GROUP_QUERY_INFOS;

typedef struct tagHKFaceRecoWorkState
{
    int workState;  // 0 - Initializing 1 - normal 2 - recoverying 3 - modelling
    itn errCode;
} HK_FACE_RECO_WORK_STATE;

typedef struct tagHKFaceRecoRect
{
    int width;
    int height;
} HK_FACE_RECO_RECT;

typedef struct tagHKFaceRecoSnapshot
{
    HK_FACE_RECO_RECT max;
    HK_FACE_RECO_RECT min;
} HK_FACE_RECO_SNAPSHOT;

typedef struct tagHKFaceRecoCap
{
    int bSupportFaceReco;
    int faceGroupNum;
    int personPerFaceGroup;
    int personByAllFaceGroup;
    int personImageSize;
    HK_FACE_RECO_SNAPSHOT faceSnapshot;
} HK_FACE_RECO_CAP;

typedef struct tagHKFaceRecoCheckPersonInfo
{
    int personID;
    char name[32];
    int sex; // 0 - male 1 - female;
    int certificatType; // 0 - password 1 - ic
    char certificatID[32];
    char country[64];
    char address[256];
    char birthday[32];
} HK_FACE_RECO_CHECK_PERSON_INFO;

typedef struct tagHKFaceRecoPersonInfoT1
{
    int groupID;
    int personID;
} HK_FACE_RECO_PERSON_INFO_T1;

typedef struct tagHKFaceRecoPersonInfosT1
{
    int size;
    HK_FACE_RECO_PERSON_INFO_T1 infos[256];
} HK_FACE_RECO_PERSON_INFOS_T1;

typedef struct tagHKFaceRecoPersonInfoT2
{
    int personID;
    char name[32];
    int sex; // 0 - male 1 - female;
    int certificatType; // 0 - password 1 - ic
    char certificatID[32];
    char country[64];
    char address[256];
    char birthday[32];
} HK_FACE_RECO_PERSON_INFO_T2;

typedef struct tagHKFaceRecoPersonInfoT3
{
    int personID;
    char name[32];
    int sex; // 0 - male 1 - female;
    int certificatType; // 0 - password 1 - ic
    char certificatID[32];
    char country[64];
    char address[256];
    char birthday[32];
    int faceImageID;
} HK_FACE_RECO_PERSON_INFO_T3;

typedef struct tagHKFaceRecoPersonInfoFilterT1
{
    int groupID;
    char name[32];
    int sex;
    int certificatType; // 0 - password 1 - ic
    char certificatID[32];
    char country[64];
    char address[256];
    char startBirthday[32];
    char stopBirthday[32];
} HK_FACE_RECO_PERSON_INFO_FILTER_T1;

typedef struct tagHKFaceRecoPersonInfoCount
{
    int personCount;
} HK_FACE_RECO_PERSON_INFO_COUNT;

typedef struct tagHKFaceRecoPersonInfoFilterT2
{
    int groupID;
    char name[32];
    int sex;
    int certificatType; // 0 - password 1 - ic
    char certificatID[32];
    char country[64];
    char address[256];
    char startBirthday[32];
    char stopBirthday[32];
    int pageSize;
    int pageNum;
} HK_FACE_RECO_PERSON_INFO_FILTER_T2;

typedef struct tagHKFaceRecoPersonInfoT4
{
    int groupID;
    int personID;
    char name[32];
    int sex; // 0 - male 1 - female;
    int certificatType; // 0 - password 1 - ic
    char certificatID[32];
    char country[64];
    char address[256];
    char birthday[32];
    int faceImageID;
} HK_FACE_RECO_PERSON_INFO_T4;

typedef struct tagHKFaceRecoPersonInfosT2
{
    int size;
    HK_FACE_RECO_PERSON_INFO_T4 infos[256];
} HK_FACE_RECO_PERSON_INFOS_T2;

typedef struct tagHKFaceRecoPersonInfoT5
{
    int bIgnoreError;
    char name[32];
    int sex; // 0 - male 1 - female;
    int certificatType; // 0 - password 1 - ic
    char certificatID[32];
    char country[64];
    char address[256];
    char birthday[32];
    int faceImageID;
} HK_FACE_RECO_PERSON_INFO_T5;

typedef struct tagHKFaceRecoPersonFaceImageInfo
{
	int personID;
	int pictureID;
} HK_FACE_RECO_PERSON_FACE_IMAGE_INFO;

typedef struct tagHKFaceRecoThresholdSchedule
{
    char startTime[32];
    char endTime[32];
    int threshold;
} HK_FACE_RECO_THRESHOLD_SCHEDULE;

typedef struct tagHKFaceRecoFaceGroup
{
    int groupID;
    char groupName[32];
} HK_FACE_RECO_FACE_GROUP;

typedef struct tagHKFaceRecoFaceGroupList
{
    int size;
    HK_FACE_RECO_FACE_GROUP groups[10];
} HK_FACE_RECO_FACE_GROUP_LIST;

typedef struct tagHKFaceRecoTimeBlockList
{
    char timeBlock0[52];
    char timeBlock1[52];
    char timeBlock2[52];
    char timeBlock3[52];
    char timeBlock4[52];
    char timeBlock5[52];
    char timeBlock6[52];
} HK_FACE_RECO_TIME_BLOCK_LIST;

typedef struct tagHKFaceRecoSchedule
{
    int bAllDay;
    HK_FACE_RECO_TIME_BLOCK_LIST timeBlockList;
} HK_FACE_RECO_SCHEDULE;

typedef struct tagHKFaceRecoTriggerParam
{
    int bEnable;
    char mask[32];
} HK_FACE_RECO_TRIGGER_PARAM;

typedef struct tagHKFaceRecoTriggerParamPTZ
{
    int bEnable;
    int PTZChannelID;
    int actionType;  // 0 - none 1 - preset 2 - pattern 3 - patrol
    int actionNum;   // 根据 actionType 参数决定
} HK_FACE_RECO_TRIGGER_PARAM_PTZ;

typedef struct tagHKFaceRecoTrigger
{
    int bNotifyAMS;
    int bBeepAlert;
    int bFullScreen;
    int bMail;
    int bFTP;
    int bPush;
    HK_FACE_RECO_TRIGGER_PARAM alarmOut;
    HK_FACE_RECO_TRIGGER_PARAM snapshot;
    HK_FACE_RECO_TRIGGER_PARAM record;
    HK_FACE_RECO_TRIGGER_PARAM_PTZ ptz;
} HK_FACE_RECO_TRIGGER;

typedef struct tagHKFaceRecoRule
{
    int bEnable;
    int recognitionRule; // 0 - Comparison pass 1 - Comparison fail
    HK_FACE_RECO_THRESHOLD_SCHEDULE recoThresholdSchedule;
    int compareLimit;
    HK_FACE_RECO_FACE_GROUP_LIST faceGroup;
    HK_FACE_RECO_SCHEDULE schedule;
    HK_FACE_RECO_TRIGGER trigger;
} HK_FACE_RECO_RULE;

typedef struct tagHKFaceRecoRuleList
{
    int personID;
    int pictureID;
} HK_FACE_RECO_RULE_LIST;

typedef struct tagHKFaceRecoRecordFilterT1
{
    int groupID;
    char startTime[32];
    char stopTime[32];
    int recognitionResultType; // 0 - all 1 - Comparison pass 2 - Comparison fail 3 - Failure of biopsy
} HK_FACE_RECO_RECORD_FILTER_T1;

typedef struct tagHKFaceRecoRecordCount
{
    int count;
} HK_FACE_RECO_RECORD_COUNT;

typedef struct tagHKFaceRecoRecordFilterT2
{
    int groupID;
    char startTime[32];
    char stopTime[32];
    int pageSize;
    int pageNum;
    int bHasPath;
    int recognitionResultType; // 0 - all 1 - Comparison pass 2 - Comparison fail 3 - Failure of biopsy
} HK_FACE_RECO_RECORD_FILTER_T2;

typedef struct tagHKFaceRecoRecordNode
{
    int logID;
    int personID;
    char groupName[32];
    char time[32];
    int bPass;
    char snapshotPath[64];
    int similar;
} HK_FACE_RECO_RECORD_NODE;

typedef struct tagHKFaceRecoRecordLIST
{
    int size;
    HK_FACE_RECO_RECORD_NODE[256];
} HK_FACE_RECO_RECORD_LIST;

typedef struct tagHKFaceRecoRecordRecognitionInfoT1
{
    int logID;
    int bIsPicture;
} HK_FACE_RECO_RECORD_RECOGNITION_INFO_T1;

typedef struct tagHKFaceRecoPersonInfoT6
{
    int personID;
    char name[32];
    int sex; // 0 - male 1 - female;
    int certificatType; // 0 - password 1 - ic
    char certificatID[32];
    char country[64];
    char address[256];
    char birthday[32];
    int bRecoResult;
} HK_FACE_RECO_PERSON_INFO_T6;

typedef struct tagHKFaceRecoRecordRecognitionInfoT2
{
    char snapshotPath[64];
} HK_FACE_RECO_RECORD_RECOGNITION_INFO_T2;

typedef struct tagHKFaceRecoRecordRecognitionInfoT3
{
    int LogID;
} HK_FACE_RECO_RECORD_RECOGNITION_INFO_T3;
