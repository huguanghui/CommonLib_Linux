#ifndef __proc_pid_status_h__
#define __proc_pid_status_h__

#ifdef __cplusplus
extern "C" {
#endif

struct process_status {
    char name[256];
    char state[256];
    int tgid;
    int pid;
    int ppid;
    int tracer_pid;
    int uid_real;
    int uid_effective;
    int uid_saved;
    int gid_real;
    int gid_effective;
    int gid_saved;
    int fdsize;
    int groups;
    unsigned long vmpeak;
    unsigned long vmsize; // 进程虚拟内存大小
    unsigned long vmlck; // 进程锁定的内存大小
    unsigned long vmpin; // 进程使用的固定内存大小
    unsigned long vmhwm; // 进程使用的最大物理内存大小
    unsigned long vmrss; // 进程实际使用的物理内存大小
    unsigned long rssanon; // 匿名内存大小
    unsigned long rssfile; // 进程使用的文件大小
    unsigned long rssshmem; // 共享内存大小
    unsigned long vmdat; // 数据段大小
    unsigned long vmstk; // 进程栈大小
    unsigned long vmexe; // 可执行文件大小
    unsigned long vmlib; // 共享库大小
    unsigned long vmpte; // 页表大小
    int threads;
};

int ProcPidStatus(int pid, struct process_status *status);

#ifdef __cplusplus
}
#endif

#endif // !__proc_pid_status_h__
