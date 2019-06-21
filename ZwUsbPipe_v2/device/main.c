#include "zw_usbpipe_base.h"
#include "zw_usbpipe_uptp.h"

//!\brief 请求数据
typedef struct
{
	int cmd;  // 操作类型
    int paramLen;
    void* param;
} AS_TASK_REQUEST;

//!\brief 响应数据
typedef struct
{
    void* param;
    int paramLen;
} AS_TASK_RESULT;

typedef enum
{
	//!< 抓拍任务
	AS_TASK_CAPTURE_FACE = 0,
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
	//!< 查询所有脸库信息
	AS_TASK_FACEGROUP_GET_ALL,
	//!< HTTP配置
	AS_TASK_HTTP_CONFIG,
} AS_TASK_TYPE;

static void test01()
{
    int deviceFd = -1;
    int rc = 0;
    char buf[1024];
    int bufLen;
    char *result = NULL;
    int resLen = 0;

    deviceFd = as_usbpipe_open();

    struct zw_uptp_message msg;
    info("[%s %d]Recv...\n", __FUNCTION__, __LINE__);
    zw_uptp_recive(deviceFd, &msg);

    info("[%s %d]Send...\n", __FUNCTION__, __LINE__);
    
    msg.msgType = MSG_TYPE_RESPONSE;
    msg.dataLen = strlen("hello back!");
    snprintf(msg.data, sizeof(msg.data), "hello back!");

    zw_uptp_send(deviceFd, &msg);

    as_usbpipe_close(deviceFd);

    return;
}

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

void CB_EventHandler(struct zw_uptp_message* msg, void* user_data)
{
    (void)user_data;

    info("msgType-[%d]\n", msg->msgType);
    if (MSG_TYPE_REQUEST == msg->msgType)
    {
        AS_TASK_REQUEST req; 
        req.cmd = (int)msg->data[0];

        info("cmd[%d]\n", req.cmd);
        switch (req.cmd)
        {
            case AS_TASK_FACEGROUP_GET_ALL: {
                
                struct zw_uptp_message response;
                memset(&response, 0 , sizeof(struct zw_uptp_message));
                response.msgType = MSG_TYPE_RESPONSE;
                response.seq = msg->seq;

                FaceGroupInfo_t fg;
                memset(&fg, 0, sizeof(FaceGroupInfo_t));

                fg.faceGroupId = 25;
                snprintf(fg.groupName, sizeof(fg.groupName), "人脸库");
                snprintf(fg.createTime, sizeof(fg.createTime), "2019-06-20 20:49");
                fg.enrolment = 60;
                fg.nValidState = 1;

                response.dataLen = sizeof(FaceGroupInfo_t);
                memcpy(response.data, &fg, sizeof(FaceGroupInfo_t));

                zw_usbpipe_uptp_write(&response);

                usleep(40*1000);

                break;
            }
            default: {
                warn("Unknown Cmd[%d]\n", req.cmd);
            }
        }

        info("[%s %s %d]\n", __FILE__, __FUNCTION__, __LINE__);
    }


    return;
}

static void test02()
{
    zw_usbpipe_uptp_init();

    zw_usbpipe_uptp_bind_opt(CB_EventHandler, NULL);

    getchar();

    zw_usbpipe_uptp_uninit();

    return;
}

int main(int argc, char const *argv[])
{
    test02();

    return 0;
}

