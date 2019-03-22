#include <stdio.h>
#include <stdlib.h>

#include "shareMem.h"

int main(int argc, char const *argv[])
{
	void *shm = NULL;
	struct shared_use_st *shared;
	int shmid;

	shmid = CreateShm(sizeof(struct shared_use_st));
	shm = MapShm(shmid);
	shared = (struct shared_use_st*)shm;

    printf("%s\n", shared->text);

	if(UnMapShm(shm) == -1)
	{
		fprintf(stderr, "shmdt failed\n");
	}

	if(DestroyShm(shmid) == -1)
	{
		fprintf(stderr, "shmctl(IPC_RMID) failed\n");
	}

	return 0;
}

