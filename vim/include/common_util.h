/******************************************************************************
* File:             common_util.h
*
* Author:           huguanghui  
* Created:          11/20/19 
* Description:      封装公共接口
*****************************************************************************/

#ifndef __COMMON_UTIL_H__
#define __COMMON_UTIL_H__

/*
 * 简化unsigned变量, 方便兼容多系统
 *
 */
typedef unsigned char char_u;
typedef unsigned short short_u;
typedef unsigned int int_u;

#if HAVE_NO_LONG_LONG
    typedef long long_long_T;
    typedef long unsigned long_long_u_T;
#else
    typedef  long long long_long_T;
    typedef  long long unsigned long_long_u_T;
#endif

typedef struct ABC
{
    char *test;
    int len;
} ABC_T;

typedef struct AAA
{
    char *test;
    int len;
} AAA_T;

#endif
