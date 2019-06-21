#include "zw_usbpipe_util.h"
#include "zw_aistick_util.h"

sem_t g_taskQueueSem;

void AS_TASK_QUEUE_INSERT(struct taskQueue* tq, struct taskNode* node)
{
    if (NULL == tq || NULL == node)
    {
        err("param error! tq[%p] node[%p]!\n", tq, node);
        return;
    }

    TAILQ_INSERT_TAIL(tq, node, nodes);

    return;
}

struct taskNode* AS_TASK_QUEUE_GET(struct taskQueue* tq)
{
    if (TAILQ_EMPTY(tq))
    {
        return NULL;
    }

    struct taskNode* e = NULL;
    e = TAILQ_FIRST(tq);
    TAILQ_REMOVE(tq, e, nodes);
    
    return e;
}

void AS_SHOW_TASK_QUEUE(struct taskQueue* tq)
{
    struct taskNode* e = NULL;
    TAILQ_FOREACH(e, tq, nodes)
    {
        // printf("%c ", e->c);
    }
    // printf("\n");
    return;
}

void AS_TASK_QUEUE_RELEASE(struct taskQueue* tq)
{
  struct taskNode* e = NULL;
  while (!TAILQ_EMPTY(tq))
  {
      e = TAILQ_FIRST(tq);
      TAILQ_REMOVE(tq, e, nodes);
      free(e);
      e = NULL;
  }

    return;
}

//

void AS_TASK_TEST_QUEUE_INSERT(struct testQueue* tq, struct testNode* node)
{
    if (NULL == tq || NULL == node)
    {
        err("param error! tq[%p] node[%p]!\n", tq, node);
        return;
    }

    TAILQ_INSERT_TAIL(tq, node, nodes);

    return;
}

struct testNode* AS_TASK_TEST_QUEUE_GET(struct testQueue* tq)
{
    if (TAILQ_EMPTY(tq))
    {
        return NULL;
    }

    struct testNode* e = NULL;
    e = TAILQ_FIRST(tq);
    TAILQ_REMOVE(tq, e, nodes);
    
    return e;
}

struct testNode* AS_TASK_TEST_QUEUE_SEARCH_BY_SEQ(struct testQueue* tq, int seq)
{
    if (TAILQ_EMPTY(tq))
    {
        return NULL;
    }

    struct testNode* e = NULL;
    TAILQ_FOREACH(e, tq, nodes)
    {
        if (e->req == seq)
        {
            TAILQ_REMOVE(tq, e, nodes);
            break;
        }
    }
    
    return e;
}

void AS_SHOW_TASK_TEST_QUEUE(struct testQueue* tq)
{
    struct testNode* e = NULL;
    TAILQ_FOREACH(e, tq, nodes)
    {
        // printf("%c ", e->c);
    }
    // printf("\n");
    return;
}

void AS_TASK_TEST_QUEUE_RELEASE(struct testQueue* tq)
{
  struct testNode* e = NULL;
  while (!TAILQ_EMPTY(tq))
  {
      e = TAILQ_FIRST(tq);
      TAILQ_REMOVE(tq, e, nodes);
      free(e);
      e = NULL;
  }

    return;
}

sem_t g_respQueueSem;

void AS_RESPONSE_QUEUE_INSERT(struct responseQueue* tq, struct responseNode* node)
{
    if (NULL == tq || NULL == node)
    {
        err("param error! tq[%p] node[%p]!\n", tq, node);
        return;
    }

    TAILQ_INSERT_HEAD(tq, node, nodes);

    return;
}

struct responseNode* AS_RESPONSE_QUEUE_GET(struct responseQueue* tq)
{
    if (TAILQ_EMPTY(tq))
    {
        return NULL;
    }

    struct responseNode* e = NULL;
    e = TAILQ_FIRST(tq);
    
    return e;
}

struct responseNode* AS_RESPONSE_QUEUE_SEARCH_BY_SEQ(struct responseQueue* tq, int seq)
{
    if (TAILQ_EMPTY(tq))
    {
        return NULL;
    }

    struct responseNode* e = NULL;
    TAILQ_FOREACH(e, tq, nodes)
    {
        if (e->seq == seq)
        {
            TAILQ_REMOVE(tq, e, nodes);
            break;
        }
    }
    
    return e;
}

void AS_RESPONSE_TEST_QUEUE(struct responseQueue* tq)
{
    struct responseNode* e = NULL;
    TAILQ_FOREACH(e, tq, nodes)
    {
        // printf("%c ", e->c);
    }
    // printf("\n");
    return;
}

void AS_RESPONSE_QUEUE_RELEASE(struct responseQueue* tq)
{
  struct responseNode* e = NULL;
  while (!TAILQ_EMPTY(tq))
  {
      e = TAILQ_FIRST(tq);
      TAILQ_REMOVE(tq, e, nodes);
      free(e);
      e = NULL;
  }

    return;    
}