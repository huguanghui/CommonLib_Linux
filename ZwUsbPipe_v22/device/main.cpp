#include <fstream>
#include "zw_usbpipe_uptp_api.h"

void CB_EventHandler(struct zw_uptp_message* msg, void* user_data)
{
	(void)user_data;

	info("msgType-[%d]\n", msg->msgType);
	if (MSG_TYPE_REQUEST == msg->msgType)
	{
		AS_TASK_REQUEST req;

		memset(&req, 0, sizeof(req));
		req.cmd = (int)msg->data[0];
		req.param = msg->data + sizeof(int);
		req.paramLen = msg->dataLen - sizeof(int);
		info("paramLen[%d]\n", req.paramLen);

		info("cmd[%d]\n", req.cmd);
		switch (req.cmd)
		{
			case AS_TASK_HTTP_CONFIG:{
				// // 打印请求字段
				// {
				// 	info("request[%.*s]\n", req.paramLen, req.param);
				// }

				//struct zw_uptp_message response;
				// memset(&response, 0 , sizeof(struct zw_uptp_message));
				// response.msgType = MSG_TYPE_RESPONSE;
				// response.seq = msg->seq;
				// response.dataLen = sizeof(response.data);
				// CGIServer_HttpRequest(-1, (BYTE*)req.param, req.paramLen, (BYTE*)response.data, &response.dataLen);
				// zw_usbpipe_uptp_write(&response);
				// usleep(40*1000);
				break;
			}
			case AS_TASK_STREAM_FRAME:{
				// AiStickFrameHeader_t *frameHead = (AiStickFrameHeader_t*)req.param;
				// void *data = req.param + sizeof(AiStickFrameHeader_t);
				// int len = frameHead->nDataLen;
				// {
				// 	char tName[64] = {0};
				// 	snprintf(tName, sizeof(tName), "/facerecodb/frame_%llu.yuv", frameHead->nFrameNo);
				// 	std::ofstream openfile(tName);
				// 	openfile.write((char *)data, len);
				// 	openfile.close();
				// }
				int return_status = 0;
				struct zw_uptp_message *resp = NULL;
				resp = (struct zw_uptp_message*)malloc(sizeof(struct zw_uptp_message));
				memset(resp, 0, sizeof(struct zw_uptp_message));
				resp->msgType = MSG_TYPE_RESPONSE;
				resp->seq = msg->seq;
				resp->dataLen = sizeof(int);
				memcpy(resp->data, &return_status, sizeof(int));
				zw_usbpipe_uptp_write(resp);
				free(resp);
				break;
			}
			case AS_TASK_CHECK_ONLINE: {
				int return_status = 0;
				struct zw_uptp_message *resp = NULL;
				resp = (struct zw_uptp_message*)malloc(sizeof(struct zw_uptp_message));
				memset(resp, 0, sizeof(struct zw_uptp_message));
				resp->msgType = MSG_TYPE_RESPONSE;
				resp->seq = msg->seq;
				resp->dataLen = sizeof(int);
				memcpy(resp->data, &return_status, sizeof(int));
				zw_usbpipe_uptp_write(resp);
				free(resp);
				break;
			}
			default: {
				warn("Unknown Cmd[%d]\n", req.cmd);
			}
		}
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

