#include "zw_usbpipe_util.h"
#include "zw_aistick_sys.h"
#include "zw_aistick_task.h"

static void async_call(char *result, int len)
{
    info("[%s:%s:%d] result[%p] len[%d]\n", __FILE__, __FUNCTION__, __LINE__, result, len);

    return;
}

int main(int argc, char const *argv[])
{
    info("[%s:%d]\n", __FUNCTION__, __LINE__);

    int rc;
    AS_SYS_INFO info;

    AS_SYS_Init(&info);

    AS_TASK_REQUEST req;
    AS_TASK_RESULT result;

    memset(&req, 0, sizeof(AS_TASK_REQUEST));
    memset(&result, 0, sizeof(AS_TASK_RESULT));

    {
        req.cmd = AS_TASK_CAPTURE_FACE;
    }

    rc = AS_TASK_Run(&req, &result);
    if (0 == rc)
    {
        if (result.param != NULL)
        {
            info("Param[%p] Len[%d]\n", result.param, result.paramLen);
        }
    }
    info("AS_TASK_Run Call Reuslt - [%d]\n", rc);

    struct taskNode node;
    memset(&node, 0, sizeof(struct taskNode));
    node.req.cmd = AS_TASK_EXTRACT_FEATURE;
    node.cb = async_call;

    info("[%s %d]\n", __FUNCTION__, __LINE__);
    AS_TASK_Add(info.taskQueue, &node);
    info("[%s %d]\n", __FUNCTION__, __LINE__);

    getchar();

    AS_SYS_UnInit();

    return 0;
}
