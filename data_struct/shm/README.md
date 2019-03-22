[TOC]

# 共享内存
> 允许不相关的进程访问同一个逻辑内存.

## 查看系统的共享内存

### ipcs

#### 选项

```shell
-m  显示有关共享内存的信息
-q  显示有关信息队列的信息
-s  显示有关semaphore的信息
```

#### 样例

```shell
$ ipcs -m
$ ipcrm -m shm_id 删除ipc
```

```cpp
#include <sys/types.h>
#include <sys/ipc.h>

key_t ftok(const char *pathname, int proj_id);
```

#include <sys/shm.h>

## 创建共享内存

```cpp
int shmget(key_t key, size_t size, int shmflg);
```

## 启动共享内存的访问

```cpp
void *shmat(int shm_id, const void *shm_addr, int shmflg);
```

## 将共享内存从当前进程分离出来

```cpp
int shmdt(const void *shmaddr);
```

## 控制共享内存

```cpp
int shmctl(int shm_id, int command, struct shmid_ds *buf);
```