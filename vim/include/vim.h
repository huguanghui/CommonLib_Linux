/* Copyright (C) 
 * 2019 - hgh
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */
/**
 * @file vim.h
 * @brief   测试项
 * @author hgh
 * @version v2.4.2
 * @date 2019-12-05
 */
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
