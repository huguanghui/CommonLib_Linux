#include "proc_uptime.h"

#include <stdio.h>

int ProcUptime(struct system_uptime *uptime)
{
    FILE *fp;
    char path[100];
    char buffer[256];

    sprintf(path, "/proc/uptime");
    fp = fopen(path, "r");
    if (fp == NULL) {
        perror("fopen");
        return -1;
    }

    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "%lf %lf", &uptime->uptime, &uptime->idletime);
    fclose(fp);

    return 0;
}
