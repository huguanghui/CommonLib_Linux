#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#include "util_net.h"

union SockAddrUnion {
    struct  sockaddr_un	unixVariant;
    struct  sockaddr_in	inetVariant;
};

static int util_net_createlocalipcfd(const char *path, int backlog);

void test_hello()
{
    printf("hello world!\n");
}

static int OS_BuildSockAddrUn(const char *bindPath,
                              struct sockaddr_un *servAddrPtr,
                              int *servAddrLen)
{
    int bindPathLen = strlen(bindPath);

    if(bindPathLen > sizeof(servAddrPtr->sun_path)) {
        return -1;
    }

    memset((char *) servAddrPtr, 0, sizeof(*servAddrPtr));
    servAddrPtr->sun_family = AF_UNIX;
    memcpy(servAddrPtr->sun_path, bindPath, bindPathLen);
    *servAddrLen = sizeof(servAddrPtr->sun_family) + bindPathLen;

    return 0;
}

static int util_net_createlocalipcfd(const char *bindPath, int backlog)
{
    int listenSock, len;
    union   SockAddrUnion sa;  
    unsigned long tcp_ia = 0;
    char    *tp;
    short   port = 0;
    char    host[MAXPATHLEN];

    len = strlen(bindPath);
    if (len >= MAXPATHLEN) {
    	fprintf(stderr, "bind path too long (>=%d): %s\n", 
    			MAXPATHLEN, bindPath);
    	exit(1);
    }
    memcpy(host, bindPath, len + 1);
    
    tp = strchr(host, ':');
    if (tp) {
		*tp = 0;
		port = atoi(++tp);
		if (port == 0) {
	    *--tp = ':';
	 }
    }
    
    // if (port) {
    //   if (!*host || !strcmp(host,"*")) {
	//         tcp_ia = htonl(INADDR_ANY);
	// 	} 
    // 	else {
	//         tcp_ia = inet_addr(host);
	//         if (tcp_ia == INADDR_NONE) {
	// 			struct hostent * hep = gethostbyname(host);
	// 			if ((!hep) || (hep->h_addrtype != AF_INET
	// 					|| !hep->h_addr_list[0])) 
	// 			{
	//                 fprintf(stderr, "Cannot resolve host name %s -- exiting!\n", host);
	//                 exit(1);
	//             }
	//             if (hep->h_addr_list[1]) {
	//                 fprintf(stderr, "Host %s has multiple addresses ---\n", host);
	//                 fprintf(stderr, "you must choose one explicitly!!!\n");
	//                 exit(1);
	//             }
	//             tcp_ia = ((struct in_addr *) (hep->h_addr))->s_addr;
	//         }
    //     }

	//     listenSock = socket(AF_INET, SOCK_STREAM, 0);
    //     if(listenSock >= 0) {
    //         int flag = 1;
    //         if(setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR,
	// 				(char *) &flag, sizeof(flag)) < 0) 
	// 		{
    //             fprintf(stderr, "Can't set SO_REUSEADDR.\n");
	//             exit(1001);
	//         }
	//     }
    // } 
    // else {
    {
	    listenSock = socket(AF_UNIX, SOCK_STREAM, 0);
    }
    
    if(listenSock < 0) {
        return -1;
    }

    /*
     * Bind the listening socket.
     */
    // if (port) {
    //     memset((char *) &sa.inetVariant, 0, sizeof(sa.inetVariant));
    //     sa.inetVariant.sin_family = AF_INET;
    //     sa.inetVariant.sin_addr.s_addr = tcp_ia;
    //     sa.inetVariant.sin_port = htons(port);
	// 	len = sizeof(sa.inetVariant);
    // } 
    // else {
    {
	    unlink(bindPath);
		if (OS_BuildSockAddrUn(bindPath, &sa.unixVariant, &len)) {
	        fprintf(stderr, "Listening socket's path name is too long.\n");
	        exit(1000);
	    }
    }
    
    if (bind(listenSock, (struct sockaddr *) &sa.unixVariant, len) < 0
    		|| listen(listenSock, backlog) < 0) 
    {
	    perror("bind/listen");
        exit(errno);
    }

    return listenSock;
}

int UTIL_NET_OpenSocket(const char *path, int backlog)
{
    int rc = util_net_createlocalipcfd(path, backlog);

    return rc;
}