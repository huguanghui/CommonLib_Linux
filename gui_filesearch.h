/*!
 *\file		gui_filesearch.h
 *\brief	
 *\author	Huguanghui
 *\date		2019/03/15
 *\version	v2.1.1 [2019/03/15]
 *\pusrpose	
-#	
 */

#ifndef GUI_FILESEARCH_H
#define GUI_FILESEARCH_H

// ------------------------------------ include standard header and custom head ------------------------------------ //
#include "gui_define.h"
#include <sys/queue.h>

typedef struct tagTimeDuration 
{
    time_t beginTime;
    time_t endTime;
}T_TIMEDURATION, *H_TIMEDURATION;

struct listFile;

struct elFile
{
    char name[64];
    int isDir;
    int size;
    time_t createdTime;
    struct listFile *sub;
    LIST_ENTRY(elFile) elements;
};

struct listFile 
{
    pthread_mutex_t mtxLock;
    struct elFile *lh_first;
};

typedef struct tagFileSearchState
{
    struct listFile show_list;
    struct listFile all_list;
    pthread_t tid_openfile;
    int bState;
}T_FILESEARCHSTATE, *H_FILESEARCHSTATE;

// ------------------------------------ prototype of global function ------------------------------------ //
/*
//!\brief	�����ļ������Ի���
//!\param 	HWND hParentWnd			�����ھ��
//!\return	NULL -- ʧ�ܣ�Else -- �ɹ�
*/ 	
int FileSearchWnd_Create(HWND hParentWnd, H_FILESEARCHSTATE hFileSearchState);


/*
//!\brief	�˳��رռ����Ի���,�ͷ���Դ,���ٴ���
//!\param 	
//!\return	
*/ 	
int FileSearchWnd_Quit();

#endif //GUI_FILESEARCH_H