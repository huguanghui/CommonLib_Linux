```
// 创建
pthread_create(&s_hFileSearchState->tid_openfile, NULL, Fxn_OpenFile, NULL);
// 设置分离式线程
pthread_detach(pthread_self());
```