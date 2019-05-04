/** @brief   基于unix域套接字的封装
 *  @file    util_net.h
 *  @author  huguanghui
 *  @version v0.0.1
 *  @date    2019-05-04
 *  @note    用于本地的微服务通信
 *  @since   
 */

#ifndef __UTIL_NET_H__
#define __UTIL_NET_H__

#ifndef MAXPATHLEN
#define MAXPATHLEN 1024
#endif

void test_hello();

int UTIL_NET_OpenSocket(const char *path, int backlog);

#endif  /* utils_net.h */