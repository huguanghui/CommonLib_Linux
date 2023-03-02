#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include "proc_pid_status.h"
#include "proc_uptime.h"

#define BUF_SIZE 1024

static void getPidByName(pid_t *pid, char *task_name)
{
    DIR *dir;
    struct dirent *ptr;
    FILE *fp;
    char filepath[50];
    char cur_task_name[50];
    char buf[BUF_SIZE];

    dir = opendir("/proc");
    if (NULL != dir) {
        while ((ptr = readdir(dir)) != NULL) {
            if ((strcmp(ptr->d_name, ".") == 0)
                || (strcmp(ptr->d_name, "..") == 0))
                continue;
            if (DT_DIR != ptr->d_type)
                continue;

            sprintf(filepath, "/proc/%s/status", ptr->d_name);
            fp = fopen(filepath, "r");
            if (NULL != fp) {
                if (fgets(buf, BUF_SIZE - 1, fp) == NULL) {
                    fclose(fp);
                    continue;
                }
                sscanf(buf, "%*s %s", cur_task_name);
                if (!strcmp(task_name, cur_task_name)) {
                    sscanf(ptr->d_name, "%d", pid);
                }
                fclose(fp);
            }
        }
        closedir(dir);
    }
    return;
}

int main(int argc, char *argv[])
{
    int rc;
    pid_t apps_pid = 0;
    struct process_status apps_proc_status;
    struct system_uptime up_ts, up_ts2;

    ProcUptime(&up_ts);
    getPidByName(&apps_pid, "ovf_srv");
    if (apps_pid > 0) {
        rc = ProcPidStatus(apps_pid, &apps_proc_status);
        if (!rc) {
            printf("[HGH-TEST][%s %d] fdsize: %d\n", __FUNCTION__, __LINE__,
                apps_proc_status.fdsize);
            printf("[HGH-TEST][%s %d] threads: %d\n", __FUNCTION__, __LINE__,
                apps_proc_status.threads);
            printf("[HGH-TEST][%s %d] vmsize: %ld kb\n", __FUNCTION__, __LINE__,
                apps_proc_status.vmsize);
            printf("[HGH-TEST][%s %d] vmrss: %ld kb\n", __FUNCTION__, __LINE__,
                apps_proc_status.vmrss);
            printf("[HGH-TEST][%s %d] vmstk: %ld kb\n", __FUNCTION__, __LINE__,
                apps_proc_status.vmstk);
        }
    }
    ProcUptime(&up_ts2);
    printf("[HGH-TEST][%s %d] cost: %.4f\n", __FUNCTION__, __LINE__,
        up_ts2.uptime - up_ts.uptime);

    return EXIT_SUCCESS;
}
