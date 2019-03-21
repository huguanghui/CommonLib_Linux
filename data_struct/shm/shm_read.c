#include <stdio.h>
#include <stdlib.h>

#include "shareMem.h"

int main(int argc, char const *argv[])
{
	int running = 1;
	void *shm = NULL;
	struct shared_use_st *shared;
	int shmid;

	shmid = CreateShm(sizeof(struct shared_use_st));
	shm = MapShm(shmid);
	shared = (struct shared_use_st*)shm;
	shared->written = 0;
	while(running)
	{
		if(shared->written != 0)
		{
			printf("You wrote: %s", shared->text);
			sleep(rand() % 3);
			shared->written = 0;
			if(strncmp(shared->text, "end", 3) == 0)
				running = 0;
		}
		else
			sleep(1);
	}

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

