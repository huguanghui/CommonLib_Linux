#include "zw_usbpipe_uptp_api.h"
#include "zw_aistick_sys.h"
#include "zw_aistick_task.h"

struct responseQueue g_queueRequestDone;

AS_SYS_INFO g_asSysInfo= {0};

static void CB_EventHandler(struct zw_uptp_message *msg, void *user_data)
{
	(void)user_data;

	if (MSG_TYPE_RESPONSE == msg->msgType)
	{
		// 处理消息 - 响应
		// info("Seq[%d] MsgType[%s] DataLen[%d] Data[%p]\n", msg->seq, getMsgTypeName((E_MSG_TYPE)msg->msgType), msg->dataLen, msg->data);
		// 放入缓冲队列
		struct responseNode* node = (struct responseNode*)malloc(sizeof(struct responseNode));
		memset(node, 0, sizeof(struct responseNode));
		node->seq = msg->seq;
		node->dataLen = msg->dataLen;
		node->data = malloc(msg->dataLen);
		memcpy(node->data, msg->data, msg->dataLen);
		AS_RESPONSE_QUEUE_INSERT(&g_queueRequestDone, node);
		sem_post(&g_queueRequestDone.sem);
	}
	else
	{
		// 处理消息 - 请求
		AS_TASK_REQUEST req;
		memset(&req, 0, sizeof(req));
		req.cmd = (int)msg->data[0];
		req.paramLen = msg->dataLen - sizeof(int);
		req.param = msg->data + sizeof(int);
		
		// info("cmd[%d] dataLen[%d]\n", req.cmd, req.paramLen);
		switch (req.cmd)
		{
			case AS_TASK_EVENT_REPORT: {
				if (g_asSysInfo.cbEventReport != NULL)
				{
					g_asSysInfo.cbEventReport(req.param, req.paramLen, g_asSysInfo.eventReportUserData);
				}
				break;
			}
			case AS_TASK_SMART_DATA: {
				if (g_asSysInfo.cbSmartFrameReport != NULL)
				{
					g_asSysInfo.cbSmartFrameReport(req.param, req.paramLen, g_asSysInfo.eventReportUserData);
				}
				break;
			}
			default: {
				warn("Unknown Cmd[%d]\n", req.cmd);
			}
		}
	}

	return;
}

static void* fxn_task_asyn_msg(void *arg)
{
	AS_SYS_INFO* info = (AS_SYS_INFO*)arg;
	struct taskQueue* tq = (struct taskQueue*)info->taskQueueAsyncMsg;
	struct taskNode *node = NULL;
	int pos = 0;

	while (!info->bTaskDoneExit)
	{
		if (TAILQ_EMPTY(tq))
		{
			sem_wait(&tq->task_sem);
		}
		else
		{
			node = AS_TASK_QUEUE_GET(tq);
			//info("node[%p]\n", node);
			if (node)
			{
				struct zw_uptp_message msg;
				
				memset(&msg, 0, sizeof(struct zw_uptp_message));
				msg.dataLen = sizeof(int) + node->req.paramLen;
				char *data = msg.data;
				msg.seq = ++g_seq;
				pos = 0;
				memcpy(data, &node->req, sizeof(int));
				pos += sizeof(int);
				memcpy(data+pos, node->req.param, node->req.paramLen);
				
				// 0 - 无返回消息  1 - 有返回消息
				if (1 == node->type)
				{
					// 先插入等待响应队列再发送
					struct waitNode *tn = (struct waitNode*)malloc(sizeof(struct waitNode));
					memset(tn, 0, sizeof(struct waitNode));
					tn->req = msg.seq;
					tn->type = CALL_ASYNC;
					tn->cb = node->cb;
					AS_TASK_WAIT_QUEUE_INSERT(&g_queueWaitMsg, tn);
				}
				zw_usbpipe_uptp_write(&msg);

				if (node->req.param)
				{
					free(node->req.param);
				}
				free(node);
			}
			//info("\n");
		}
	}

	return NULL;
}

static void* fxn_response_done_thread(void *arg)
{
	struct responseNode *node = NULL;
	struct waitNode *task = NULL;

	while(1)
	{
		if (TAILQ_EMPTY(&g_queueRequestDone))
		{
			sem_wait(&g_queueRequestDone.sem);
		}
		else
		{
			node = AS_RESPONSE_QUEUE_GET(&g_queueRequestDone);
			if (node != NULL)
			{
				//info("Seq[%d]\n", node->seq);
				task = AS_TASK_WAIT_QUEUE_SEARCH_BY_SEQ(&g_queueWaitMsg, node->seq);
				//info("task[%p]\n", task);
				if (task != NULL)
				{
					task->response.paramLen = node->dataLen;
					task->response.param = node->data;
					if (CALL_ASYNC == task->type)
					{
						if (task->cb != NULL)
						{
							task->cb((char*)task->response.param, task->response.paramLen);
						}
						if (task->response.param)
						{
							free(task->response.param);
						}
						free(task);
					}
					else if (CALL_SYNC == task->type)
					{
						sem_post(&task->sem);
						// 节点数据由同步接口释放
					}
					else
					{
						warn("Unknow Type!\n");
						if (task->response.param)
						{
							free(task->response.param);
						}
						free(task);
					}
					TAILQ_REMOVE(&g_queueRequestDone, node, nodes);
					free(node);
				}
				else
				{
					// 消息超时 - 移除
					TAILQ_REMOVE(&g_queueRequestDone, node, nodes);
					free(node);
				} 
			}
		}
	}

	return NULL;
}

AS_API AS_S32 AS_SYS_Init()
{
	pthread_t respDoneId;

	memset(&g_asSysInfo, 0, sizeof(g_asSysInfo));
	info("\n");
	g_asSysInfo.bTaskDoneExit = 0;
	sem_init(&g_taskQueueSem, 0, 0);
	// 建立连接
	zw_usbpipe_uptp_init();
	// 时间同步
	// 启动心跳
	// 初始化同步消息队列
	AS_TASK_WAIT_QUEUE_INIT(&g_queueWaitMsg);
	// 启动任务队列 - 异步消息处理
	AS_TASK_CreateQueue(&g_asSysInfo.taskQueueAsyncMsg, "task-queue-01", NULL);
	// 创建任务队列的处理线程
	pthread_create(&g_asSysInfo.taskDoneId, NULL, fxn_task_asyn_msg, (void *)&g_asSysInfo);
	// 创建响应消息的缓存队列
	AS_RESPONSE_QUEUE_INIT(&g_queueRequestDone);
	// 缓存队列处理函数
	pthread_create(&respDoneId, NULL, fxn_response_done_thread, (void *)&g_asSysInfo);
	// 绑定接收智能棒处理回调(响应/智能棒的请求)
	// 启动消息监听
	zw_usbpipe_uptp_bind_opt(CB_EventHandler, NULL);
	// 获取能力
	// 将状态置为初始化完成
	g_asSysInfo.status = 1;
	info("\n");
	return 0;
}

AS_API AS_S32 AS_SYS_SetEventCallback(AS_CALLBACK cb, void *user_data)
{
	int ret = -1;
	if (1 == g_asSysInfo.status)
	{
		if (g_asSysInfo.cbEventReport != NULL)
		{
			warn("AS_EVENT_CALLBACK Has Been Set! This Would Set Aagin!\n");
		}
		g_asSysInfo.cbEventReport = cb;
		g_asSysInfo.eventReportUserData = user_data;
	}
	return ret;
}

AS_API AS_S32 AS_SYS_SetSmartFrameCallback(AS_CALLBACK cb, void *user_data)
{
	int ret = -1;
	if (1 == g_asSysInfo.status)
	{
		if (g_asSysInfo.cbSmartFrameReport != NULL)
		{
			warn("AS_SYS_SetSmartFrameCallback Has Been Set! This Would Set Aagin!\n");
		}
		g_asSysInfo.cbSmartFrameReport = cb;
		g_asSysInfo.smartFrameReportUserData = user_data;
	}
	return ret;
}

AS_API AS_S32 AS_SYS_UnInit()
{
	zw_usbpipe_uptp_uninit();

	return 0;
}

AS_API AS_S32 AS_SYS_GetState()
{
	return 0;
}
