#include "proc_pid_statm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ProcPidStatm(int pid, struct process_statm *statm)
{
    char filename[64];

    sprintf(filename, "/proc/%d/statm", pid);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("fopen");
        return -1;
    }

    fscanf(fp, "%lu %lu %lu %lu %lu %lu %lu", &statm->size, &statm->resident,
        &statm->share, &statm->text, &statm->lib, &statm->data, &statm->dt);

    fclose(fp);

    return 0;
}
