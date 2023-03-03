------
## status

> 进程状态信息

```
Name：进程名称
State：进程状态（例如running、sleeping等）
PPid：父进程的进程ID
Pgrp：进程组ID
Uid：用户ID
Gid：组ID
VmSize：进程虚拟内存的大小
VmRSS：进程物理内存的大小
Threads：线程数
FDSize：进程打开文件的数量
```
------

## maps

> 进程的内存映射信息

```
address           perms offset  dev   inode   pathname
08048000-08056000 r-xp 00000000 08:01 13130   /bin/cat
```

其中，各字段的含义如下：

- address：VMA 的起始地址和终止地址，用短横线（-）隔开。
- perms：VMA 的权限，包括读（r）、写（w）、执行（x）和私有（p）等，用字母表示。
- offset：VMA 相对于映射文件开头的偏移量。
- dev：VMA 所在设备的编号，以及设备类型，用冒号（:）隔开。
- inode：VMA 所映射文件的 inode 编号。
- pathname：如果该 VMA 映射了文件，则为文件的路径名；否则为一个空字符串（" "）。

------

## stat

> 在Linux系统中，/proc/pid/stat是一种特殊的文件，其中pid是进程ID（Process ID）的缩写。此文件包含了有关进程的一些统计信息，例如进程状态、进程的父进程ID、进程的用户ID、进程的系统时间和用户时间等。

```
pid (process id) comm (filename of the executable) state (process state) ppid (parent process id) pgrp (process group id) session (session id) tty_nr (controlling terminal of the process) tpgid (foreground process group id) flags (process flags) minflt (number of minor faults) cminflt (number of minor faults with child's) majflt (number of major faults) cmajflt (number of major faults with child's) utime (user mode jiffies) stime (kernel mode jiffies) cutime (user mode jiffies with child's) cstime (kernel mode jiffies with child's) priority (priority level) nice (nice level) num_threads (number of threads) itrealvalue (time in jiffies before the next SIGALRM is sent to the process) starttime (time in jiffies the process started after system boot) vsize (virtual memory size in bytes) rss (resident set size: number of pages the process has in real memory) rsslim (resident set size limit in bytes) startcode (address above which program text can run) endcode (address below which program text can run) startstack (address of the start of the stack) kstkesp (current value of ESP) kstkeip (current value of EIP) signal (bitmap of pending signals) blocked (bitmap of blocked signals) sigignore (bitmap of ignored signals) sigcatch (bitmap of caught signals) wchan (channel in which the process is waiting) nswap (number of pages swapped) cnswap (cumulative nswap for child processes) exit_signal (signal to be sent to parent when the process dies) processor (CPU number last executed on) rt_priority (real-time scheduling priority) policy (scheduling policy) delayacct_blkio_ticks (I/O delay (in ticks)) guest_time (guest time of the process (time spent running a virtual CPU for a guest operating system)) cguest_time (guest time of the process's children)
```

------

解析每个字段：

- pid：进程ID
- comm：可执行文件名称
- state：进程状态。可能的值包括：
- R：正在运行
- S：睡眠状态
- D：不可中断的睡眠状态
- Z：僵尸进程
- T：跟踪/停止状态
- t：停止状态
- W：换页状态
- X：死亡状态
- x：死亡状态
- K：内核线程
- W：无记账的进程
- ppid：父进程ID
- pgrp：进程组ID
- session：进程所在的会话ID
- tty_nr：进程控制的终端设备号
- tpgid：前台进程组ID
- flags：进程的标志
- minflt：发生的次数页错误，但页不在磁盘上
- cminflt：发生的次数页错误，但页在磁盘上
- majflt：发生的次数页错误，并且需要从磁盘读取
- cmajflt：发生的次数页错误，并且需要从磁盘读取，子进程的总数
- utime：进程在用户模式下运行的时间，以时钟滴答为单位
- stime：进程在核心模式下运行的时间，以时钟滴答为单位
- cutime：所有已终止的子进程在用户模式下运行的时间，以时钟滴答为单位
- cstime：所有已终止的子进程在核心模式下运行的时间，以时钟滴答为单位
- priority：进程优先级
- nice：进程nice值
- num_threads：进程线程数
- starttime：进程启动时间
- vsize：虚拟内存大小
- rss：实际内存使用量（以页面为单位）
- rsslim：进程使用的最大实际内存（以字节为单位）
- startcode：可执行代码段的地址
- endcode：可执行代码段的结束地址
- startstack：栈的开始地址
- kstkesp：当前栈指针（ESP）的值
- kstkeip：当前指令指针（EIP）的值
- signal：当前挂起的信号掩码
- blocked：当前阻塞的信号掩码
- sigignore：当前忽略的信号掩码
- sigcatch：当前捕获的信号掩码
- wchan：进程当前等待的内核函数地址
- nswap：已交换的页面数
- cnswap：所有子进程已交换的页面数的总和
- exit_signal：进程退出时向父进程发送的信号
- processor：最近运行进程的处理器编号
- rt_priority：实时调度优先级
- policy：调度策略
- delayacct_blkio_ticks：I/O延迟（以时钟滴答为单位）
- guest_time：虚拟机内的客户机处理器时间
- cguest_time：所有已终止子进程的虚拟机内客户机处理器时间的总和。

## mem

> 该文件可以用于读取或写入进程的内存
