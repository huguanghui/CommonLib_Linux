#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <semaphore.h>

#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <string.h>

//#include "data_struct/ngx_pool/ngx_pool.h"

//仅仅是打印函数名字替换 DEBUG <--> printf
#define DEBUG(format, ...) printf(format, ##__VA_ARGS__)

//替换打印函数，在打印出来的内容加上前缀
#define XFUNC_PRINT(format, arg...) printf("XFUNC: " format "", ##arg)

//名字替换，并在打印出来的内容加上前缀,同时加入定位的功能。
#define TRC_P(fmt, args...) fprintf(stderr,"  TRC_P(%s:%d):\t" fmt, __func__, __LINE__, ##args)

//名字替换，并在打印出来的内容加上前缀,同时加入定位的功能，并让打印的前缀具备特殊颜色(\033[1;32m  \033[0m这些表示颜色，\t一定程度上使屏幕输出对齐)
#define TRC_PG(fmt, args...) fprintf(stderr, "\033[1;32m  TRC_PG(%s:%d):\033[0m" fmt, __func__, __LINE__, ## args)

//名字替换，并在打印出来的内容加上前缀,同时加入定位的功能，并让打印的前缀具备特殊颜色
#define TRC_PR(fmt, args...) fprintf(stderr, "\033[1;31m  TRC_PR(%s:%d):\t\033[0m" fmt, __func__, __LINE__, ## args)

#define HGH_DBG(fmt, args...) printf("\033[40;33m HGH_DBG(%s %s %d):\t\033[0m" fmt, __FILE__, __func__, __LINE__, ## args)

#define HGH_DBG_CHAR(fmt, args...) do {printf("\033[40;33m HGH_DBG(%s %s %d):\t\033[0m" fmt, __FILE__, __func__, __LINE__, ## args); getchar();}while(0)

// 部分颜色代码:
// 字背景颜色: 40--49                字颜色: 30--39
// 40: 黑                           30: 黑
// 41: 红                           31: 红
// 42: 绿                           32: 绿
// 43: 黄                           33: 黄
// 44: 蓝                           34: 蓝
// 45: 紫                           35: 紫
// 46: 深绿                         36: 深绿
// 47：白色                         37：白色

typedef struct
{
    char addr[256];
}stTest;


void HexOutput(void *buf, size_t len)
{
    char *b = (char*)buf;
    int numRow = 0;
    int numCol = 0;

    printf("Hen[%p]Len[%d]:\n", buf, len);
    size_t i = 0;
    for(i=0; i<len; i=i+16)
    {
        printf("%p|", b+i);
        numCol = (len-i>=16) ? 16 : (len-i);    
        int j = 0;
        for (j = 0; j < numCol; j++)
        {
            unsigned char c = b[i+j]; // must use unsigned char to print >128 value
            if( c < 16)
            {
                printf(" 0%x", c);
            }
            else
            {
                printf(" %x", c);
            }
        }
        printf("\n");
    }
    printf("\n");
}


static char *util_getlocaltime()
{
    char localtime[32] = {0};
    char *strRc = (char *)localtime;

    struct timeval tv;
    struct timezone tz;
    gettimeofday (&tv, &tz);

    return strRc;
}
static char const* echoTime()
{
    static char buf[200];

    time_t tt = time(NULL);
    strftime(buf, sizeof(buf), "Date: %a, %b %d %Y %H:%M:%S GMT", gmtime(&tt));

    return buf;
}

static char const* getTime()
{
    static char buf[200];
    struct timeval tv;

    gettimeofday(&tv, NULL);
    snprintf(buf, sizeof(buf), "[%ld]s [%ld]us", tv.tv_sec, tv.tv_usec);

    return buf;
}

#define HGH_TIME_DBG(fmt, args...) printf("\033[40;33m HGH_DBG[%s](%s %s %d):\t\033[0m"fmt, getTime(), __FILE__, __func__, __LINE__, ## args)
// int main(int argc, char *argv[])
// {
//     int pagesize = getpagesize();
//     printf("PageSize: %d\n", pagesize);

//     // DEBUG("abc\n");
//     // XFUNC_PRINT("abc\n");
//     // TRC_P("abc\n");
//     // TRC_PG("abc\n");
//     // TRC_PR("abc\n");
//     // HGH_DBG("abc\n");
//     // int i = 0;
//     // int n = 0;

//     // for (n = 0; n < 20; n ++)
//     // {
//     //     if (i++ > 3)
//     //     {
//     //         HGH_DBG("I:%d\n", i);
//     //         break;
//     //     }
//     // }
//     // ngx_pool_t *pool = NULL;
//     // pool = ngx_create_pool(1024);
//     // ngx_destroy_pool(pool);
//     // HGH_DBG("I:%d\n", i);
//     // HGH_DBG_CHAR("abc\n");
//     struct timeval start_time, over_time, consume_time;
//     gettimeofday(&start_time, NULL);
//     {
//         char b[21] = {0};
//         HexOutput(&b, sizeof(b));
//     }
//     gettimeofday(&over_time, NULL);
    // consume_time.tv_usec = over_time.tv_usec - start_time.tv_usec;
    // consume_time.tv_sec = over_time.tv_sec - start_time.tv_sec;
    // if (consume_time.tv_usec < 0)
    // {
    //     consume_time.tv_sec --;
    //     consume_time.tv_usec += 1000000;
    // }
//     HGH_DBG("Hex Cost Time[%ld\t%ld]\n", consume_time.tv_sec, consume_time.tv_usec);

//     return 0;
// }

sem_t g_sem;
//sem_init(&g_sem, 0, 0);
//sem_post(&g_sem);
//sem_wait(&g_sem);

void file_write(const char *s)
{
    std::ofstream openfile("frame_translate.txt", std::ios::app);
    openfile.write(s, strlen(s));
    openfile << "\n";
    openfile.close();
}

int main(int argc, char const *argv[])
{
    /* 轮询当前进程的环境变量 */
    // extern char **environ;
    // int i;

    // for (i=0; environ[i] != NULL; i++)
    // {
    //     HGH_DBG("%s\n", environ[i]);
    // }

    /* 获取和设置指定环境环境变量 */
    char *s_path = getenv("PATH");
    char *s_user = getenv("USER");

    HGH_DBG("PATH[%s] USER[%s]\n", s_path?s_path:"", s_user?s_user:"");

    char* tmp = "abcddddddddddddd";
    HGH_DBG("%.*s test\n", 7, tmp);
    	
    // file_write("abcdef");
    // file_write("111111");
    // file_write("1ccccdd");

    stTest tmpT;
    memset(&tmpT, 0, sizeof(stTest));
    HGH_DBG("tmpT[%p]\n", tmpT.addr);
    HGH_DBG("tmpT2[%p]\n", tmpT.addr+4);

    return 0;
}
