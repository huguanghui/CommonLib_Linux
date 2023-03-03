#include "proc_pid_stat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ProcPidStat(int pid, struct process_stat *stat)
{
    char path[100];
    sprintf(path, "/proc/%d/stat", pid); // 生成/proc/pid/stat文件的路径

    FILE *fp = fopen(path, "r"); // 打开文件
    if (fp == NULL) {
        perror("Error opening status file");
        return -1;
    }

    long int it_real_value;
    int guest_time, cguest_time;

    fscanf(fp,
        "%d %s %c %d %d %d %d %d %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld "
        "%ld %ld %ld %lu %ld %lu %lu %ld %lu %lu %lu %lu %lu %lu %lu %lu "
        "%lu %lu %d %d",
        &stat->pid, stat->comm, &stat->state, &stat->ppid, &stat->pgrp,
        &stat->session, &stat->tty_nr, &stat->tpgid, &stat->utime, &stat->stime,
        &stat->cutime, &stat->cstime, &stat->priority, &stat->nice,
        &stat->num_threads, &it_real_value, &stat->starttime, &stat->vsize,
        &stat->rss, &stat->rsslim, &stat->startcode, &stat->endcode,
        &stat->startstack, &stat->kstkesp, &stat->kstkeip, &stat->signal,
        &stat->blocked, &stat->sigignore, &stat->sigcatch, &stat->wchan,
        &stat->nswap, &stat->cnswap, &stat->exit_signal, &stat->processor,
        &stat->rt_priority, &stat->delayacct_blkio_ticks, &guest_time,
        &cguest_time);

    fclose(fp);

    return 0;
}
