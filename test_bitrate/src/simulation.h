#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#ifdef __cplusplus
extern "C" {
#endif

/*! \struct frame_info
 *  \brief 数据帧信息
 *
 *  Detailed description
 */
struct frame_info {
    int frame_len;
};

typedef void (* CB_BITRATE)(struct frame_info *info);

void simulation_set_framehook(CB_BITRATE cb);

void simulation_start();

void simulation_stop();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* end of include guard */
