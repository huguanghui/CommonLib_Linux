#include <stdio.h>
#include <stdlib.h>
#include <linux/types.h>

#include "ngx_queue.h"

typedef struct{
    char *str;
    ngx_queue_t qEle;
    int num;
}TestNode ;

static void queue_echo(ngx_queue_t *list);
static int testnode_destroy(ngx_queue_t *list);
int testnode_cmp(const ngx_queue_t *a, const ngx_queue_t *b);

/*
 * nginx中的双向链表的使用样例
 * 1.创建一个链表进行从小到大进行排序输出打印
 * 
 */
int main(int argc, char const *argv[])
{
    int i;
    ngx_queue_t testList;
    TestNode *tmpNode = NULL;

    // 1.创建链表,初始化
    ngx_queue_init(&testList);

    // 2.链表插入数据
    for (i = 0; i < 10; i++)
    {
        tmpNode = (TestNode *)malloc(sizeof(TestNode));
        printf("malloc(0x%p)\n", (void *)tmpNode);
        tmpNode->num = 10 - i;
        ngx_queue_insert_tail(&testList, &tmpNode->qEle);
    }
    queue_echo(&testList);
    // 3.排序
    ngx_queue_sort(&testList, testnode_cmp);
    queue_echo(&testList);

    // 4.删除链表
    testnode_destroy(&testList);

    return 0;
}

static void queue_echo(ngx_queue_t *list)
{
    int idx = 0;
    ngx_queue_t *q = NULL;
    TestNode *tmpNode = NULL;

    for (q = ngx_queue_head(list); q != ngx_queue_sentinel(list); q = ngx_queue_next(q))
    {
        tmpNode = ngx_queue_data(q, TestNode, qEle);
        printf("Node[%d]: %d\n", idx++, tmpNode->num);
    }

    return ;
}

static int testnode_destroy(ngx_queue_t *list)
{
    if (NULL == list)
    {
        printf("[ERR] Pararm Err!\n");
        return -1;
    }

    ngx_queue_t *q = NULL;
    TestNode *tmpNode = NULL;

    for (q = ngx_queue_head(list); q != ngx_queue_sentinel(list); q = ngx_queue_next(q))
    {
        tmpNode = ngx_queue_data(q, TestNode, qEle);
        ngx_queue_remove(&tmpNode->qEle);
        printf("free[0x%p]\n", (void *)tmpNode);
        free(tmpNode);
    }    

    return 0;
}

int testnode_cmp(const ngx_queue_t *a, const ngx_queue_t *b)
{
    TestNode *m = ngx_queue_data(a, TestNode, qEle);
    TestNode *n = ngx_queue_data(b, TestNode, qEle);

    return m->num > n->num;
}
