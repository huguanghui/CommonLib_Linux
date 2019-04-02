#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

#include "data_struct/ngx_pool/ngx_pool.h"

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

//#define HGH_DBG(fmt, args...) printf("\033[40;33m HGH_DBG(%s %s %d):\t\033[0m"fmt, __FILE__, __func__, __LINE__, ## args)

#define HGH_DBG_CHAR(fmt, args...) do {printf("\033[40;33m HGH_DBG(%s %s %d):\t\033[0m"fmt, __FILE__, __func__, __LINE__, ## args); getchar();}while(0)

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

int main(int argc, char *argv[])
{
    int pagesize = getpagesize();
    printf("PageSize: %d\n", pagesize);

    // DEBUG("abc\n");
    // XFUNC_PRINT("abc\n");
    // TRC_P("abc\n");
    // TRC_PG("abc\n");
    // TRC_PR("abc\n");
    // HGH_DBG("abc\n");
    int i = 0;
    int n = 0;

    for (n = 0; n < 20; n ++)
    {
        if (i++ > 3)
        {
            HGH_DBG("I:%d\n", i);
            break;
        }
    }
    ngx_pool_t *pool = NULL;
    pool = ngx_create_pool(1024);

    ngx_destroy_pool(pool);

    HGH_DBG("I:%d\n", i);

    HGH_DBG_CHAR("abc\n");

    return 0;
}
