#ifndef __SHAREMEM_H__
#define __SHAREMEM_H__

#define KEY_NAME   "/tmp"
#define KEY_ID     (0x6666)

int CreateShm(int nSize);

int GetShm(void);

void* MapShm(int nShmID);

int UnMapShm(const void* pShmAddr);

int DestroyShm(int nShmid);

#endif
