#include "proc_pid_status.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_status_file(int pid, struct process_status *status)
{
    char filename[256];
    sprintf(filename, "/proc/%d/status", pid);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        char *name = strtok(line, ":");
        char *value = strtok(NULL, ":");
        if (strcmp(name, "Name") == 0) {
            // 进程名称
            strncpy(status->name, value, sizeof(status->name));
            status->name[sizeof(status->name) - 1] = '\0';
        } else if (strcmp(name, "State") == 0) {
            // 进程状态
            strncpy(status->state, value, sizeof(status->state));
            status->state[sizeof(status->state) - 1] = '\0';
        } else if (strcmp(name, "Tgid") == 0) {
            // 进程组ID
            status->tgid = atoi(value);
        } else if (strcmp(name, "Pid") == 0) {
            // 进程ID
            status->pid = atoi(value);
        } else if (strcmp(name, "PPid") == 0) {
            // 父进程ID
            status->ppid = atoi(value);
        } else if (strcmp(name, "TracerPid") == 0) {
            // 跟踪器进程ID
            status->tracer_pid = atoi(value);
        } else if (strcmp(name, "Uid") == 0) {
            // 用户ID
            char *uid_real = strtok(value, "\t");
            char *uid_effective = strtok(NULL, "\t");
            char *uid_saved = strtok(NULL, "\t");
            status->uid_real = atoi(uid_real);
            status->uid_effective = atoi(uid_effective);
            status->uid_saved = atoi(uid_saved);
        }
    }
}

int ProcPidStatus(int pid, struct process_status *status)
{
    FILE *fp;
    char path[100];
    char buffer[256];

    sprintf(path, "/proc/%d/status", pid);
    fp = fopen(path, "r");
    if (fp == NULL) {
        perror("Error opening status file");
        return -1;
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        char *name = strtok(buffer, ":");
        char *value = strtok(NULL, ":");
        if (strcmp(name, "Name") == 0) {
            // 进程名称
            strncpy(status->name, value, sizeof(status->name));
            status->name[sizeof(status->name) - 1] = '\0';
        } else if (strcmp(name, "State") == 0) {
            // 进程状态
            strncpy(status->state, value, sizeof(status->state));
            status->state[sizeof(status->state) - 1] = '\0';
        } else if (strcmp(name, "Tgid") == 0) {
            // 进程组ID
            status->tgid = atoi(value);
        } else if (strcmp(name, "Pid") == 0) {
            // 进程ID
            status->pid = atoi(value);
        } else if (strcmp(name, "PPid") == 0) {
            // 父进程ID
            status->ppid = atoi(value);
        } else if (strcmp(name, "TracerPid") == 0) {
            // 跟踪器进程ID
            status->tracer_pid = atoi(value);
        } else if (strcmp(name, "Uid") == 0) {
            // 用户ID
            char *uid_real = strtok(value, "\t");
            char *uid_effective = strtok(NULL, "\t");
            char *uid_saved = strtok(NULL, "\t");
            status->uid_real = atoi(uid_real);
            status->uid_effective = atoi(uid_effective);
            status->uid_saved = atoi(uid_saved);
        } else if (strcmp(name, "Gid") == 0) {
            // 组ID
            char *gid_real = strtok(value, "\t");
            char *gid_effective = strtok(NULL, "\t");
            char *gid_saved = strtok(NULL, "\t");
            status->gid_real = atoi(gid_real);
            status->gid_effective = atoi(gid_effective);
            status->gid_saved = atoi(gid_saved);
        } else if (strcmp(name, "FDSize") == 0) {
            status->fdsize = atoi(value);
        } else if (strcmp(name, "Groups") == 0) {
            status->groups = atoi(value);
        } else if (strcmp(name, "VmPeak") == 0) {
            status->vmpeak = atoi(value);
        } else if (strcmp(name, "VmSize") == 0) {
            status->vmsize = atoi(value);
        } else if (strcmp(name, "VmLck") == 0) {
            status->vmlck = atoi(value);
        } else if (strcmp(name, "VmPin") == 0) {
            status->vmpin = atoi(value);
        } else if (strcmp(name, "VmHWM") == 0) {
            status->vmhwm = atoi(value);
        } else if (strcmp(name, "VmRSS") == 0) {
            status->vmrss = atoi(value);
        } else if (strcmp(name, "VmStk") == 0) {
            status->vmstk = atoi(value);
        } else if (strcmp(name, "VmExe") == 0) {
            status->vmexe = atoi(value);
        } else if (strcmp(name, "VmLib") == 0) {
            status->vmlib = atoi(value);
        } else if (strcmp(name, "Threads") == 0) {
            status->threads = atoi(value);
        }
    }
    fclose(fp);

    return 0;
}
