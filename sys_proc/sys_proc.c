#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include "proc_pid_status.h"

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
    pid_t apps_pid = 0;

    getPidByName(&apps_pid, "apps");
    if (apps_pid > 0) {
        ProcIdStatus(apps_pid);
    }

    return EXIT_SUCCESS;
}
