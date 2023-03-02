#include "proc_pid_maps.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 256

int ProcPidMaps(int pid, struct process_maps *maps, int size)
{
    int idx = 0;
    char buf[BUF_SIZE];
    char path[BUF_SIZE];
    FILE *fp;

    snprintf(path, sizeof(path), "/proc/%d/maps", pid);
    fp = fopen(path, "r");
    if (NULL == fp) {
        return 0;
    }

    while (fgets(buf, BUF_SIZE - 1, fp) != NULL) {
        if (idx >= size) {
            break;
        }
        sscanf(buf, "%lx-%lx %4s %lx %*s %*s %*s", &maps[idx].start,
            &maps[idx].end, maps[idx].perm, &maps[idx].offset);
        idx++;
    }
    fclose(fp);

    return idx;
}
