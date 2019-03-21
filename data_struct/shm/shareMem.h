#ifndef __SHAREMEM_H__
#define __SHAREMEM_H__

#define KEY_NAME   "/tmp"
#define KEY_ID     (0x6666)

int CreateShm(int nSize);

int GetShm(void);

void* MapShm(int nShmID);

int UnMapShm(const void* pShmAddr);

int DestroyShm(int nShmid);

#define TEXT_SZ 2048

struct shared_use_st
{
	int written;//作为一个标志，非0：表示可读，0表示可写
	char text[TEXT_SZ];//记录写入和读取的文本
};

#endif
