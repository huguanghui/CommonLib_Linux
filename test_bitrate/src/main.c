#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>

#include "simulation.h"
#include "bitrate_calc.h"

#ifndef MS
#define MS(x) ((x).tv_sec*1000 + (x).tv_nsec/1000000)
#endif

void cb_frame(struct frame_info *info)
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    int cur_pts = MS(tp);

    rc_check(0);
    BitrateItem item ={0};
    item.frame_sz = info->frame_len;
    item.frame_pts = cur_pts;

    rc_filled(0, item);
    //printf("[%s %d] len: %d\n", __FUNCTION__, __LINE__, info->frame_len);
    return;
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    int framerate = 0;
    float fps = 0;
    simulation_set_framehook(cb_frame);
    simulation_start();
    while(1)
    {
        rc_get_info(0, &framerate, &fps);
        printf("framerate: %d fps: %.2f\n", framerate, fps);
        sleep(1);
    }
    simulation_stop();

    return 0;
}
