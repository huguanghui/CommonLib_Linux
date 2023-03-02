#ifndef __proc_pid_maps_h__
#define __proc_pid_maps_h__

#ifdef __cplusplus
extern "C" {
#endif

struct process_maps {
    unsigned long start; // 虚拟内存起始地址
    unsigned long end; // 虚拟内存结束地址
    unsigned long offset; // 文件偏移量
    char perm[5]; // 权限
    unsigned long inode; // inode
    char pathname[256]; // 文件路径
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
