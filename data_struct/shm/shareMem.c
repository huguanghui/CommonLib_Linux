#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "shareMem.h"

#define SHM_ERR(format, args...) do{printf("\033[1;31m[SHM][%s-%d](%s) \033[m" format, __FILE__, __LINE__, strerror(errno), ##args);}while(0)

static int CommShm(int nSize, int nFlags)
{
    int nKey = -1;
    int nShmid = -1;
    
    nKey = ftok(KEY_NAME, KEY_ID);
    if (nKey < 0)
    {
        SHM_ERR("Fail to ftok %s\n", KEY_NAME);
        return -1;
    }
    nShmid = shmget(nKey, nSize, nFlags);
    if (nShmid < 0)
    {
        SHM_ERR("Fail to shmget\n");
        return -1;
    }

    return nShmid;
}

static void* CommShmat(int nShmID, const void* pShmAddr, int nFlags)
{
    void* pAddr = NULL;
    pAddr = shmat(nShmID, pShmAddr, nFlags);
    if (-1 == (int)pAddr)
    {
        SHM_ERR("Fail to shmat\n");
        return NULL;
    }

    return pAddr;
}

static int CommShmdt(const void* pShmAddr)
{
    int nRet = -1;
    nRet = shmdt(pShmAddr);
    if (nRet < 0)
    {
        SHM_ERR("Fail to shmdt\n");
        return -1;
    }

    return 0;
}

int CreateShm(int nSize)
{
    return CommShm(nSize, IPC_CREAT|IPC_EXCL|0666);    
}

int GetShm(void)
{
   return CommShm(0, IPC_CREAT); 
}

void* MapShm(int nShmID)
{
    return CommShmat(nShmID, NULL, 0);
}

int UnMapShm(const void* pShmAddr)
{
    return CommShmdt(pShmAddr);
}

int DestroyShm(int nShmid)
{
    if(shmctl(nShmid, IPC_RMID, NULL) < 0)
    { 
        SHM_ERR("Fail to shmctl\n");
        return -1;
    }

    return 0;
}
