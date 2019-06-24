#ifndef __ALIST_H__
#define __ALIST_H__

void test();

typedef struct ALIST_S{
 void* memlist;
 pthread_mutex_t mutex;
} ALIST_T;

#define SOAP_MAXALLOCSIZE (0)
#define SOAP_CANARY (0xC0DE)

int alist_init(ALIST_T* list);
void* soap_malloc(ALIST_T* list, size_t n);
void* soap_calloc(ALIST_T* list, size_t n);
char* alist_strdup(ALIST_T* list, const char* s);
void alist_dealloc(ALIST_T* list, void* p);
void alist_uninit(ALIST_T* list);
void echo_alist(ALIST_T* list);

#endif
