#ifndef __BITRATE_CALC_H__
#define __BITRATE_CALC_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bitrate_item {
    int frame_sz;
    int frame_pts; // ms
}BitrateItem;

void rc_check(int ch);

void rc_filled(int ch, const BitrateItem item);


#ifdef __cplusplus
};
#endif

#endif /** bitrate_calc.h **/

