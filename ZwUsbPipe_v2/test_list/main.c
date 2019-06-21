#include "zw_usbpipe_util.h"

struct zw_uptp_list g_List;

int main(int argc, char const *argv[])
{
    // list 初始化
    char buf[128];
    LIST_INIT(&g_List);

    pthread_mutex_init(&g_List.mtxLock, NULL);

    struct elNode *node;
    for (int i=0; i < 15; i++)
    {
        node = (struct elNode *)malloc(sizeof(struct elNode));
        memset(node, 0, sizeof(struct elNode));
        node->seq = i;
        snprintf(buf, sizeof(buf), "test_%02d", i);
        node->data = strdup(buf);
        node->dataLen = strlen(buf);
        zw_uptp_list_push(&g_List, node);
    }

    int len = zw_uptp_list_length(&g_List);
    info("Len[%d]\n", len);

    zw_uptp_list_echo(&g_List);

    struct elNode *test;
    test = zw_uptp_list_pop_by_seq(&g_List, 10);
    memcpy(buf, test->data, test->dataLen);
    buf[test->dataLen] = '\0';
    info("Seq[%d] Data[%s]\n" ,test->seq, buf);

    len = zw_uptp_list_length(&g_List);
    info("Len[%d]\n", len);
    zw_uptp_list_echo(&g_List);

    zw_uptp_list_destroy(&g_List);

    return 0;
}

