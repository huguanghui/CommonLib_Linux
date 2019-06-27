#include "zw_usbpipe_uptp_api.h"
#include "zw_aistick_sys.h"
#include "zw_aistick_task.h"

struct responseQueue g_respQueue;

static void CB_EventHandler(struct zw_uptp_message* msg, void* user_data)
{
	(void)user_data;

	if (MSG_TYPE_RESPONSE == msg->msgType)
	{
		info("Seq[%d] MsgType[%s] DataLen[%d] Data[%p]\n", msg->seq, getMsgTypeName((E_MSG_TYPE)msg->msgType), msg->dataLen, msg->data);
		// 放入缓冲队列
		struct responseNode* node = (struct responseNode*)malloc(sizeof(struct responseNode));
		memset(node, 0, sizeof(struct responseNode));
		node->seq = msg->seq;
		node->dataLen = msg->dataLen;
		node->data = malloc(msg->dataLen);
		memcpy(node->data, msg->data, msg->dataLen);
		AS_RESPONSE_QUEUE_INSERT(&g_respQueue, node);
		sem_post(&g_respQueueSem);
	}
	else
	{
		/* code */
		AS_TASK_REQUEST* req = (AS_TASK_REQUEST*)msg->data;

		info("[%s %s %d]cmd[%d] dataLen[%d]\n", __FILE__, __FUNCTION__, __LINE__, req->cmd, req->paramLen);
	}

	return;
}

static void* fxn_task_done_thread(void *arg)
{
	AS_SYS_INFO* info = (AS_SYS_INFO*)arg;
	struct taskQueue* tq = (struct taskQueue*)info->taskQueue;
	struct taskNode *node = NULL;
	int pos = 0;

	while (!info->bTaskDoneExit)
	{
		if (TAILQ_EMPTY(tq))
		{
			sem_wait(&g_taskQueueSem);
		}
		else
		{
			node = AS_TASK_QUEUE_GET(tq);
			info("node[%p]\n", node);
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
				
				// 先插入队列再发送
				struct testNode* tn = (struct testNode*)malloc(sizeof(struct testNode));
				memset(tn, 0, sizeof(struct testNode));
				tn->req = msg.seq;
				tn->type = CALL_ASYNC;
				tn->cb = node->cb;
				AS_TASK_TEST_QUEUE_INSERT(&g_msgQueue, tn);

				zw_usbpipe_uptp_write(&msg);

				usleep(40*1000);

				if (node->req.param)
				{
					free(node->req.param);
				}
				free(node);
			}
			info("[%s:%d]\n", __FUNCTION__, __LINE__);
		}
	}

	return NULL;
}

static void* fxn_response_done_thread(void *arg)
{
	struct responseNode* node = NULL;
	struct testNode* task = NULL;

	while(1)
	{
		if (TAILQ_EMPTY(&g_respQueue))
		{
			info("[%s:%d]\n", __FUNCTION__, __LINE__);
			sem_wait(&g_respQueueSem);
			info("[%s:%d]\n", __FUNCTION__, __LINE__);
		}
		else
		{
			node = AS_RESPONSE_QUEUE_GET(&g_respQueue);
			if (node != NULL)
			{
				info("Seq[%d]\n", node->seq);
				task = AS_TASK_TEST_QUEUE_SEARCH_BY_SEQ(&g_msgQueue, node->seq);
				info("task[%p]\n", task);
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
					TAILQ_REMOVE(&g_respQueue, node, nodes);
					free(node);
				}
				else
				{
					// 移动到尾部等待处理
					TAILQ_REMOVE(&g_respQueue, node, nodes);
					TAILQ_INSERT_TAIL(&g_respQueue, node, nodes);
				} 
			}
			info("[%s:%d]\n", __FUNCTION__, __LINE__);
		}        
	}

	return NULL;
}

AS_API AS_S32 AS_SYS_Init(AS_SYS_INFO* info)
{
	if (NULL == info)
	{
		err("param error! info[%p]\n", info);
		return -1;
	}
	pthread_t respDoneId;

	info->bTaskDoneExit = 0;
	sem_init(&g_taskQueueSem, 0, 0);
	// 建立连接
	zw_usbpipe_uptp_init();
	// 时间同步
	// 启动心跳
	// 启动同步消息传输
	TAILQ_INIT(&g_msgQueue);
	// 启动异步消息传输
	AS_TASK_CreateQueue(&info->taskQueue);
	//   创建任务队列的处理线程
	pthread_create(&info->taskDoneId, NULL, fxn_task_done_thread, (void *)info);
	// 启动消息监听
	// 创建响应消息的缓存队列, 和缓存队列处理函数
	TAILQ_INIT(&g_respQueue);
	pthread_create(&respDoneId, NULL, fxn_response_done_thread, (void *)info);
	// 绑定接收智能棒处理回调(响应/智能棒的请求)
	zw_usbpipe_uptp_bind_opt(CB_EventHandler, NULL);
	// 获取能力

	return 0;
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
