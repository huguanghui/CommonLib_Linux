#include <stdio.h>

#include "bitrate_calc.h"

#define MAX_FRAME_HIST (120)

struct stream_rc_statistics {
    BitrateItem fram_hist[MAX_FRAME_HIST];
    int hist_in_idx;
    int hist_out_idx;
    int total_win_size;
    int framerate;
}StreamRcStatistics;

void rc_check(int ch)
{
    (void)ch;
    return;
}

void rc_filled(int ch, const BitrateItem item)
{
    (void)ch;
    (void)item;
    return;
}

