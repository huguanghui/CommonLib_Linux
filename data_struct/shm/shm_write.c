#include <stdio.h>
#include <stdlib.h>

#include "shareMem.h"

int main(int argc, char const *argv[])
{
    int running = 1;
    int shmid;
    void *shm;
    struct shared_use_st *shared = NULL;
    char buffer[BUFSIZ + 1];

    shmid = CreateShm(sizeof(struct shared_use_st));
    shm = MapShm(shmid);
    shared = (struct shared_use_st *)shm;
    while (running)
    {
        while (shared->written == 1)
        {
            sleep(1);
            printf("Enter some text: ");
        }
        fgets(buffer, BUFSIZ, stdin);
        strncpy(shared->text, buffer, TEXT_SZ);
        shared->written = 1;
        if (strncmp(buffer, "end", 3) == 0)
            running = 0;
    }

    if (UnMapShm(shm) == -1)
    {
        fprintf(stderr, "UnMapShm Failed!\n");
    }
    sleep(2);

    return 0;
}

