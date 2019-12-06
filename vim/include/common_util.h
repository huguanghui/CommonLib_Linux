/** @brief 公共基础结构和接口
 *  @file common_util.h
 *  @author huguanghui
 *  @version v2.4.2
 *  @date 2019-12-05 14:03
 *  @note  暂无
 */

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

/**
 * @brief 测试结构体ABC_T
 */
typedef struct ABC
{
    char *test;
    int len;
} ABC_T;

/**
 * @brief 测试结构体ABC_T
 */
typedef struct AAA
{
    char *test;
    int len;
} AAA_T;

#endif
