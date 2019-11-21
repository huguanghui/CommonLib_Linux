#include <stdio.h>
#include <string.h>
#include "macros.h"
#include "vim.h"

static mparm_T params;

void main(int argc, char *argv[])
{
    int i;

    vim_memset(&params, 0, sizeof(params));  
    params.argc = argc;
    params.argv = argv;

    for (i = 0; i < argc; ++i) {
       if (STRICMP(argv[i]) == 0) {
           params.clean = TRUE;
           break;
       } 
    }
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
        gettimeofday(&now);
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
