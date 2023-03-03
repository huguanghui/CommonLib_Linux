#ifndef __proc_pid_stat_h__
#define __proc_pid_stat_h__

#ifdef __cplusplus
extern "C" {
#endif

struct process_stat {
    int pid; // 进程ID
    char comm[256]; // 进程命令名
    char state; // 进程状态
    int ppid; // 父进程ID
    int pgrp; // 进程组ID
    int session; // 会话ID
    int tty_nr; // 终端控制进程的tty编号
    int tpgid; // 前台进程组ID
    unsigned int flags; // 进程标志
    unsigned long minflt; // 未被换出的页面访问但无需物理读取的次数
    unsigned long cminflt; // 子进程的同样信息
    unsigned long majflt; // 未被换出的页面访问且需要物理读取的次数
    unsigned long cmajflt; // 子进程的同样信息
    unsigned long utime; // 用户态运行的时间
    unsigned long stime; // 内核态运行的时间
    long cutime; // 所有已死子进程的用户态运行时间之和
    long cstime; // 所有已死子进程的内核态运行时间之和
    long priority; // 进程优先级
    long nice; // 进程nice值
    long num_threads; // 进程线程数
    long starttime; // 进程开始时间
    unsigned long vsize; // 进程虚拟地址空间大小
    long rss; // 常驻集大小，以页面计算
    unsigned long rsslim; // 常驻集大小限制，以字节计算
    unsigned long startcode; // 进程代码段的地址
    unsigned long endcode; // 进程代码段的地址+长度
    unsigned long startstack; // 进程栈的地址
    unsigned long kstkesp; // 栈指针
    unsigned long kstkeip; // 栈内指令指针
    long signal; // 挂起的信号位图
    long blocked; // 阻塞的信号位图
    long sigignore; // 忽略的信号位图
    long sigcatch; // 捕获的信号位图
    unsigned long wchan; // 进程等待的内核函数地址
    long nswap; // 进程交换出的页面数
    long cnswap; // 子进程的同样信息
    long exit_signal; // 终止进程的信号
    long processor; // 执行进程的CPU编号
    unsigned long rt_priority; // 实时进程优先级
    unsigned int policy; // 进程调度策略
    unsigned long delayacct_blkio_ticks; // 累计的块I/O延迟时钟数
};

int ProcPidStat(int pid, struct process_stat *stat);

#ifdef __cplusplus
}
#endif

#endif // !__proc_pid_stat_h__
