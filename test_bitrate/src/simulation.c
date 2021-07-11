#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "simulation.h"

struct sim_info {
    pthread_t tid;
    int bexit;
    CB_BITRATE cb;
};

static struct sim_info g_info = {0};

static void *sim_generate(void *args)
{
    struct sim_info *ctx = (struct sim_info *)args;

    int cur_sleepms = 0;
    int cur_framelen = 0;

    if (NULL != ctx) {
         while (!ctx->bexit) {
            if (NULL != ctx->cb) {
                struct frame_info info = {0};
                cur_framelen = (rand()%500 + 501)*1000;
                info.frame_len = cur_framelen;
                ctx->cb(&info);
            }
            // rand生成20-30的数字
            cur_sleepms = rand()%10 + 21;
            usleep(cur_sleepms * 1000);
         }
    }

    return NULL;
}

void simulation_start()
{
    srand((unsigned int)time(NULL));
    g_info.bexit = 0;
    pthread_create(&g_info.tid, NULL, sim_generate, &g_info);
    return;
}

void simulation_stop()
{
    g_info.bexit = 1;
    pthread_join(g_info.tid, NULL);
    return;
}

void simulation_set_framehook(CB_BITRATE cb)
{
    g_info.cb = cb;
    return;
}
