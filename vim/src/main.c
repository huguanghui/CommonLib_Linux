#define EXTERN
#include "vim.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "macros.h"

static void parse_command_name(mparm_T *parmp);

void time_msg(char *mesg, void *tv_start);

static mparm_T params;

void main(int argc, char *argv[])
{
    int i;

    vim_memset(&params, 0, sizeof(params));  
    params.argc = argc;
    params.argv = argv;

    for (i = 0; i < argc; ++i) {
       if (STRICMP(argv[i], "--clean") == 0) {
           params.clean = TRUE;
           break;
       } 
    }

    for ( i = 0; i < argc - 1; ++i)
    {
        if (STRICMP(argv[i], "--startuptime") == 0) {
           time_fd = mch_fopen(argv[i + 1], "a");
           TIME_MSG("--- VIM STARTING ---"); 
           break;
        }
    }
    starttime = time(NULL);

}

/** 初始化操作
 @param paramp[in] 需要初始化的结构体
 @return NULL
 @warning 传入参数不能为NULL
 @todo 有待实现
*/
void common_init(mparm_T *paramp)
{

}

static struct timeval pre_timeval;

static void time_diff(struct timeval *then, struct timeval *now)
{
    long usec;
    long msec;

    usec = now->tv_usec - then->tv_usec;
    msec = (now->tv_sec - then->tv_sec) * 1000L + usec / 1000L;
    usec = usec % 1000L;
    fprintf(time_fd, "%03ld.%03ld", msec, usec >= 0 ? usec : usec + 1000L);
    return;
}

void time_msg(char *mesg, void *tv_start)
{
    static struct timeval start;
    struct timeval now;

    if (time_fd != NULL)
    {
        if (strstr(mesg, "STARTING") != NULL) {
            gettimeofday(&start, NULL); 
            pre_timeval = start;
            fprintf(time_fd, "\n\ntime in msec\n");
            fprintf(time_fd, " clock self+sourced self: sourced script\n");

            fprintf(time_fd, " clock elapsed:           other lines\n\n");
        }
        gettimeofday(&now, NULL);
        time_diff(&start, &now);
        if ((struct timeval *)tv_start != NULL)
        {
            fprintf(time_fd, "  ");
            time_diff(((struct timeval *)tv_start), &now);
        }
        fprintf(time_fd, "  ");
        time_diff(&pre_timeval, &now);
        pre_timeval = now;
        fprintf(time_fd, ": %s\n", mesg);
    }
    return;
}

static void parse_command_name(mparm_T *parmp)
{
    char_u *initstr;     
    initstr = gettail((char_u *)parmp->argv[0]);
}
