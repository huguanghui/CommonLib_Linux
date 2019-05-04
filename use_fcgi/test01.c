#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "fcgios.h"

#define HGH_DBG(fmt, args...) printf("\033[40;33m HGH_DBG(%s %s %d):\t\033[0m"fmt, __FILE__, __func__, __LINE__, ## args)

const char *unixPath = "/tmp/fcgi.sock";

int main(int argc, char const *argv[])
{
    int listenFd;
    char buf[1024];
    int n;
    int i;

    // 1. create unix fd
    listenFd = OS_CreateLocalIpcFd(unixPath, 100);
    // 2. accept
    while (1)
    {
        int connFd;
        connFd = OS_Accept(listenFd, 1, NULL);
        if (connFd < 0)
        {
            HGH_DBG("Aceep Failed![%d]\n", connFd);
            break;
        }
        while (1)
        {
            memset(buf, 0, sizeof(buf));
            n = OS_Read(connFd, buf, sizeof(buf));  
            if (n < 0) {  
                perror("read error");  
                break;  
            } else if(n == 0) {  
                printf("EOF\n");  
                break;  
            }  
              
            HGH_DBG("Received: %s\n", buf);
 
            // for(i = 0; i < n; i++) {  
            //     buf[i] = toupper(buf[i]);  
            // }
            OS_Write(connFd, buf, n);  
        }
        OS_Close(connFd, 0);
    }
    // 3. close unix fd
    OS_Close(listenFd, 1);

    return 0;
}
