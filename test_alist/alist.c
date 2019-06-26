#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alist.h"

void test()
{
    printf("test!\n");
    return;
}

int alist_init(ALIST_T* list)
{
    int iRet = 0;

    if (list)
    {
        list->memlist = NULL;
        pthread_mutex_init(&list->mutex, NULL);
    }

    return iRet;
}

void* alist_malloc(ALIST_T* list, size_t n)
{
    char* p = NULL;

    if (SOAP_MAXALLOCSIZE > 0 && n > SOAP_MAXALLOCSIZE)
    {
        return NULL;
    }
    if (n+sizeof(short) < n)
    {
        return NULL;
    }
    n += sizeof(short);
    if (n + ((-(long)n) & (sizeof(void*)-1)) + sizeof(void*) + sizeof(size_t) < n)
    {
        return NULL;
    }
    n += (-(long)n) & (sizeof(void*)-1);
    p = (char*)malloc(n + sizeof(void*) + sizeof(size_t));
    if (NULL == p) {
        return NULL; 
    }
    *(unsigned short*)(p+n-sizeof(unsigned short)) = (unsigned short)SOAP_CANARY;
    
    pthread_mutex_lock(&list->mutex);
    *(void**)(p+n) = list->memlist;
    *(size_t*)(p+n+sizeof(void*)) = n;
    list->memlist = p + n;
    printf("[addr:len][0x%p, %ld]\n", p, n);
    pthread_mutex_unlock(&list->mutex);

    return p;
}

void* alist_calloc(ALIST_T* list, size_t n)
{
    char* p = NULL;

    p = alist_malloc(list, n);
    if (p != NULL)
    {
        memset(p, 0, n); 
    }

    return p;
}

char* alist_strdup(ALIST_T* list, const char* s)
{
    if (NULL == s){
        return NULL; 
    }
    char* t = NULL;

    size_t n = strlen(s) + 1;
    if (n > 0) {
        t = (char*)alist_calloc(list, n);
        if (NULL == t) {
            return NULL;
        }
        memcpy((void*)t, (const void*)s, n);
        t[n-1] = '\0';
    }

    return t;
}

void alist_dealloc(ALIST_T* list, void* p)
{
    if (NULL == list) {
        return; 
    }

    pthread_mutex_lock(&list->mutex);
    if (p != NULL)
    {
        char** q = NULL;
        for ( q = (char**)(void*)&list->memlist; *q; q=*(char***)q)
        {
            if (*(unsigned short*) (char*)(*q - sizeof(unsigned short)) != \
                    (unsigned short)SOAP_CANARY) 
            {
                break;
            }
            if (p == (void*)(*q - *(size_t*)(*q + sizeof(void*))))
            {
                *q = **(char***)q;
                free(p);
                break;
            }
        }
    }
    else
    {
        char* q = NULL;

        while (list->memlist)
        {
             q = (char*)list->memlist;
             if (*(unsigned short*)(q - sizeof(unsigned short)) != (unsigned short)SOAP_CANARY)
             {
                 break;
             }
             list->memlist = *(void**)q;
             q -= *(size_t*)(q + sizeof(void*));
             free(q);
        }
    }
    
    pthread_mutex_unlock(&list->mutex);

    return;
}

void alist_uninit(ALIST_T* list)
{
    if (NULL == list)
    {
        return;
    }

    alist_dealloc(list, NULL);
    pthread_mutex_destroy(&list->mutex);

    return;
}

void echo_alist(ALIST_T* list)
{
    if (NULL == list)
    {
        return;
    }

    char* p = NULL;
    int index = 0;

    pthread_mutex_lock(&list->mutex);
    while (list->memlist)
    {
        // 打印节点的分配的地址
        p = list->memlist;
        if (*(unsigned short*)(p - sizeof(unsigned short)) != (unsigned short)SOAP_CANARY)
        {
            break;
        }
        list->memlist = *(void**)p;
        p -= *(size_t*)(p+sizeof(void*));
        // 打印节点的序号
        printf("[idx: node][%d: 0x%p]\n", index, p);
        index++;
    }
    pthread_mutex_unlock(&list->mutex);
    
    return;
}

