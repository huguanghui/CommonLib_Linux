#ifndef LIST01_H
#define LIST01_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

struct listFile;

struct elFile
{
    char name[64];
    int isDir;
    int size;
    time_t createdTime;
    struct listFile *sub;
    LIST_ENTRY(elFile) elements;
};

struct listFile 
{
    struct elFile *lh_first;
};

extern struct listFile g_listFile;

#define dbg() printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__)

// Function
int list01_init();
int list01_genList(char *dirname, struct listFile *list);

#endif