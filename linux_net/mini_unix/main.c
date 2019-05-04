#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#include <sys/socket.h>
#include <sys/un.h>

#include "util_net.h"

#define HGH_DBG(fmt, args...) printf("\033[40;33m HGH_DBG(%s %s %d):\t\033[0m"fmt, __FILE__, __func__, __LINE__, ## args)
#define MAXLINE 80 

const char *unix_path = "/tmp/demo.sock";

int main(int argc, char const *argv[])
{
    int listenfd = UTIL_NET_OpenSocket(unix_path, 100);

    while(1)
    {
        struct sockaddr_un cliun;  
        socklen_t cliun_len;  
        int connfd;  
        char buf[MAXLINE];  
        int i, n;  
        
        cliun_len = sizeof(cliun);         
        if ((connfd = accept(listenfd, (struct sockaddr *)&cliun, &cliun_len)) < 0){  
            perror("accept error");  
            continue;  
        }  
          
        while(1) {  
            n = read(connfd, buf, sizeof(buf));  
            if (n < 0) {  
                perror("read error");  
                break;  
            } else if(n == 0) {  
                printf("EOF\n");  
                break;  
            }  
              
            HGH_DBG("received: %s", buf);  
 
            for(i = 0; i < n; i++) {  
                buf[i] = toupper(buf[i]);  
            }  
            write(connfd, buf, n);  
        }  
        close(connfd);      
    }
    close(listenfd);

    return 0;
}
