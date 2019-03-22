#include <stdio.h>
#include <stdlib.h>

#include "shareMem.h"

int main(int argc, char const *argv[])
{
    int shmid;
    void *shm;
    struct shared_use_st *shared = NULL;

    shmid = CreateShm(sizeof(struct shared_use_st));
    shm = MapShm(shmid);
    shared = (struct shared_use_st *)shm;

    strncpy(shared->text, "Hello World!", strlen("Hello World!"));

    if (UnMapShm(shm) == -1)
    {
        fprintf(stderr, "UnMapShm Failed!\n");
    }
    sleep(2);

    return 0;
}

