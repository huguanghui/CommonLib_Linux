#ifndef __proc_pid_maps_h__
#define __proc_pid_maps_h__

#ifdef __cplusplus
extern "C" {
#endif

struct process_maps {
    unsigned long start_addr; // 起始地址
    unsigned long end_addr; // 结束地址
    unsigned long size; // 区域大小
    char perms[5]; // 权限标志
    unsigned long offset; // 偏移量
    char dev[12]; // 设备名称
    unsigned long inode; // 文件的inode节点号
    char pathname[256]; // 区域所映射文件的路径名
};

/**
 * @brief 获取进程虚拟内存映射信息
 *
 * @param pid 进程ID
 * @param maps 进程虚拟内存映射信息
 * @param size 进程虚拟内存映射信息数组大小
 * @return int 成功返回填充大小，失败返回-1
 */
int ProcPidMaps(int pid, struct process_maps *maps, int size);

#ifdef __cplusplus
}
#endif

#endif // !__proc_pid_maps_h__
