#ifndef __USB_PIPE_UTIL_H__
#define __USB_PIPE_UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/queue.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HGH_DBG(fmt, args...) printf("\033[40;31m HGH_DBG(%s %s %d):\t\033[0m"fmt, __FILE__, __func__, __LINE__, ## args)

#define USBPIPE_MAXSIZE (256 * 1024)

void _msg(unsigned level, const char* file, const char* fun, int line, const char *fmt, ...);

void HexOutput(void *buf, size_t len);

#define die(...)  (_msg(2, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__), exit(1))
#define err(...)   _msg(3, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define warn(...)  _msg(4, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define note(...)  _msg(5, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define info(...)  _msg(6, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define debug(...) _msg(7, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

typedef enum {
   MSG_TYPE_REQUEST = 0,
   MSG_TYPE_RESPONSE
}E_MSG_TYPE;

// 定义消息的结构
struct zw_uptp_message {
  char identify[4];
  int msgType;   // 0 - 请求 1 - 响应
  int seq;
  int dateLen;
  char data[USBPIPE_MAXSIZE];
};

const char* getMsgTypeName(E_MSG_TYPE e);

struct zw_uptp_list;

struct elNode
{
    int seq;
    time_t createdTime;
    void* data;
    int dataLen;
    LIST_ENTRY(elNode) elements;
};

struct zw_uptp_list 
{
    pthread_mutex_t mtxLock;
    struct elNode *lh_first;
};

int zw_uptp_list_length(struct zw_uptp_list* list);
int zw_uptp_list_destroy(struct zw_uptp_list* list);
int zw_uptp_list_echo(struct zw_uptp_list* list);
void zw_uptp_list_push(struct zw_uptp_list *list, struct elNode* np);
struct elNode* zw_uptp_list_pop_by_seq(struct zw_uptp_list *list, int seq);
void zw_uptp_list_push(struct zw_uptp_list *list, struct elNode* np);

#ifdef __cplusplus
}
#endif

#endif
