#include <stdio.h>

#include "bitrate_calc.h"

#define MAX_FRAME_HIST (120)
#define MAX_CH_NUM (2)
#define SEQ_NUM (40)

typedef struct stream_rc_statistics {
    BitrateItem frame_hist[MAX_FRAME_HIST];
    int hist_in_idx;
    int hist_out_idx;
    int total_win_size;
    int frame_cnt;
    int cur_average_rc;
    float cur_average_framerate;
}StreamRcStatistics;

static StreamRcStatistics g_rc[MAX_CH_NUM] = {0};

void rc_check(int ch)
{
    StreamRcStatistics *rc = &g_rc[ch];

    if (rc->frame_cnt >= SEQ_NUM)
    {
        rc->cur_average_rc = rc->total_win_size/rc->frame_cnt;
        int start_idx = (rc->hist_in_idx + MAX_FRAME_HIST - 1)%MAX_FRAME_HIST;
        int tm_delta = rc->frame_hist[start_idx].frame_pts - rc->frame_hist[rc->hist_out_idx].frame_pts;
        rc->cur_average_framerate = rc->frame_cnt*(1000.0)/tm_delta;
        // output
        rc->frame_cnt--;
        rc->total_win_size -= rc->frame_hist[rc->hist_out_idx].frame_sz;
        rc->hist_out_idx++;
        if (rc->hist_out_idx >= MAX_FRAME_HIST) {
            rc->hist_out_idx = rc->hist_out_idx%MAX_FRAME_HIST;
        }
    }

    return;
}

void rc_filled(int ch, const BitrateItem item)
{
    StreamRcStatistics *rc = &g_rc[ch];
    // is_full
    if (rc->frame_cnt < (MAX_FRAME_HIST-1))
    {
        rc->frame_hist[rc->hist_in_idx].frame_sz = item.frame_sz;
        rc->frame_hist[rc->hist_in_idx].frame_pts = item.frame_pts;
        rc->total_win_size += item.frame_sz;
        //printf("frame cnt: %d sz: %d pts: %d\n", rc->frame_cnt, item.frame_sz, item.frame_pts);
        rc->frame_cnt++;
        rc->hist_in_idx++;
        if (rc->hist_in_idx >= MAX_FRAME_HIST) {
            rc->hist_in_idx = rc->hist_in_idx%MAX_FRAME_HIST;
        }
    }
    return;
}

void rc_get_info(int ch, int *bitrate, float *fps)
{
    if ((NULL != bitrate) && (NULL != fps))
    {
        StreamRcStatistics *rc = &g_rc[ch];
        *bitrate = rc->cur_average_rc;
        *fps = rc->cur_average_framerate;
    }

    return;
}
