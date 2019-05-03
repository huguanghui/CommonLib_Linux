#include <stdio.h>
#include "fcgiapp.h"

#define HGH_DBG(fmt, args...) printf("\033[40;33m HGH_DBG(%s %s %d):\t\033[0m"fmt, __FILE__, __func__, __LINE__, ## args)

int main(int argc, char const *argv[])
{

    FCGX_Request request;  
    FCGX_Init();  
    int sock_fd = FCGX_OpenSocket("/dev/shm/demo.sock",100);  
    FCGX_InitRequest(&request, sock_fd, 0);

    HGH_DBG("Start Accept:\n"); 
    while (FCGX_Accept_r(&request) >= 0) 
    {  
        //get param 1  
        for(int i = 0; request.envp[i]; ++i) {  
            char *s = request.envp[i];  
            HGH_DBG("Str[%d:%s]\n", i, s);
        }  
    }

    return 0;
}
