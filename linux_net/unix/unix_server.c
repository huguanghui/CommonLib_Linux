#include <stdlib.h>  
#include <stdio.h>  
#include <stddef.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#include <errno.h>  
#include <string.h>  
#include <unistd.h>  
#include <ctype.h>   
 
#define MAXLINE 80  
 
char *socket_path = "server.socket";  
 
int main(void)  
{  
    struct sockaddr_un serun, cliun;  
    socklen_t cliun_len;  
    int listenfd, connfd, size;  
    char buf[MAXLINE];  
    int i, n;  
 
    if ((listenfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {  
        perror("socket error");  
        exit(1);  
    }  
 
    memset(&serun, 0, sizeof(serun));  
    serun.sun_family = AF_UNIX;  
    strcpy(serun.sun_path, socket_path);  
    size = offsetof(struct sockaddr_un, sun_path) + strlen(serun.sun_path);  
    unlink(socket_path);  
    if (bind(listenfd, (struct sockaddr *)&serun, size) < 0) {  
        perror("bind error");  
        exit(1);  
    }  
    printf("UNIX domain socket bound\n");  
      
    if (listen(listenfd, 20) < 0) {  
        perror("listen error");  
        exit(1);          
    }  
    printf("Accepting connections ...\n");  
 
    while(1) {  
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
              
            printf("received: %s", buf);  
 
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