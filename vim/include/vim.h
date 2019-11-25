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

#define mch_open(n, m, p) oepn((n), (m), (p))
#define mch_fopen(n, p) fopen((n), (p))

typedef struct 
{
    int         argc;
    char        **argv;
    
    char_u      *fname;

    int         evim_mode;
    char_u      *use_vimrc;
    int         clean;
} mparm_T;

#include "globals.h"

#ifndef EXTERN
# define EXTERN extern
# define INIT(x)
#else
# ifndef INIT
#  define INIT(x) x
#  define DO_INIT
#  define COMMA ,
# endif
#endif

typedef time_t time_T;

#endif
