#include "zw_usbpipe_util.h"
#include "zw_aistick_sys.h"
#include "zw_aistick_task.h"

int main(int argc, char const *argv[])
{
    info("[%s:%d]\n", __FUNCTION__, __LINE__);

    struct taskQueue tq;
    TAILQ_INIT(&tq);

    struct taskNode* e = NULL;
    int c = 0;
    for (c = 0; c < 10; ++c)
    {
        e = (struct taskNode*)malloc(sizeof(struct taskNode));
        e->req.cmd = c;
        TAILQ_INSERT_TAIL(&tq, e, nodes);
    }

    AS_SHOW_TASK_QUEUE(&tq);

    e = AS_TASK_QUEUE_GET(&tq);
    info("get [%d]\n", e->req.cmd);
    free(e);

    AS_SHOW_TASK_QUEUE(&tq);

    AS_TASK_QUEUE_RELEASE(&tq);

    return 0;
}
