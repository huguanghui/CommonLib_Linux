/******************************************************************************
* File:             vim.h
*
* Author:           huguanghui  
* Created:          11/20/19 
* Description:      整理 vim 中常用的使用
*****************************************************************************/

#ifndef __VIM_H__
#define __VIM_H__

typedef unsigned char char_u;

#ifndef TRUE
#define FALSE 0
#define TRUE 1
#endif

#define vim_memset(ptr, c, size) memset((ptr), (c), (size))

#define STRICMP(d, s) strcasecmp((char*)(d), (char*)(s))

typedef struct 
{
    int         argc;
    char        **argv;
    
    char_u      *fname;

    int         evim_mode;
    char_u      *use_vimrc;
    int         clean;
} mparm_T;

#endif
