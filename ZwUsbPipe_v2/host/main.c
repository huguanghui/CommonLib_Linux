#include <stdlib.h>
#include <fcntl.h>
#include "zw_usbpipe_uptp_api.h"

// int test_fifo()
// {
//     const char *fifo_name = "/tmp/my_fifo";
//     int pipe_fd = -1;
//     int data_fd = -1;
//     int res = 0;
//     const int open_mode = O_WRONLY;
//     char buffer[PIPE_BUF+1];
//     if(access(fifo_name,F_OK)==-1)
//     {
//         res = mkfifo(fifo_name,0777);
//         if(res!=0)
//         {
//             fprintf(stderr,"could not create fifo\n");
//             exit(EXIT_FAILURE);
//         }
//     }
//     printf("process %d opening fifo O_WRONLY\n",getpid());
//     pipe_fd = open(fifo_name,open_mode);
//     data_fd = open("data.txt",O_RDONLY);
//     printf("process %d result %d\n",getpid(),pipe_fd);
//     if(pipe_fd!=-1)
//     {
//         int bytes_read = 0;
//         bytes_read = read(data_fd,buffer,PIPE_BUF);
//         while(bytes_read>0)
//         {
//             res = write(pipe_fd,buffer,bytes_read);
//             if(res==-1)
//             {
//                 fprintf(stderr,"write error\n");
//                 exit(EXIT_FAILURE);
//             }
//             bytes_read = read(data_fd,buffer,PIPE_BUF);
//             buffer[bytes_read]='\0';
//         }
//         close(pipe_fd);
//         close(data_fd);
//     }
//     else{
//         exit(EXIT_FAILURE);
//     }
//     printf("process %d finished.\n",getpid());
//     exit(EXIT_SUCCESS);
// }

static void abc()
{
    // int hostFd = -1;
    // int rc = 0;
    // char buf[1024];
    // int bufLen;
    // char *result = NULL;
    // int resLen = 0;

    // hostFd = as_usbpipe_open();
    
    // info("[%s %d]Send...\n", __FUNCTION__, __LINE__);
    // snprintf(buf, sizeof(buf), "hello world!");
    // as_usbpipe_send(hostFd, buf, strlen(buf));

    // info("[%s %d]Recv...\n", __FUNCTION__, __LINE__);
    // as_usbpipe_recv(hostFd, &result, &resLen);
    // memcpy(buf, result, resLen);
    // free(result);
    // buf[resLen] = '\0';
    // printf("Result:%s\n", buf);

    // as_usbpipe_close(hostFd);
    
    // return;
}

void CB_EventHandler(struct zw_uptp_message* msg, void* user_data)
{
    (void)user_data;

    char buf[1024];
    memcpy(buf, msg->data, msg->dataLen);
    buf[msg->dataLen] = '\0';

    info("[%s %s %d]Data[%s]\n", __FILE__, __FUNCTION__, __LINE__, buf);

    return;
}

static void test()
{
    // zw_usbpipe_uptp_init();

    // zw_usbpipe_uptp_bind_opt(CB_EventHandler, NULL);

    // char* result = NULL;
    // int resultLen = 0;
    // char buf[1024];
    // snprintf(buf, sizeof(buf), "hello huguanghui");
    // zw_usbpipe_uptp_call_sync(buf, strlen(buf), (void**)&result, &resultLen);

    // memcpy(buf, result, resultLen);
    // buf[resultLen] = '\0';
    // info("Result[%s]\n", buf);
    // free(result);
    // getchar();

    // zw_usbpipe_uptp_uninit();

    return;
}

int main(int argc, char const *argv[])
{
    //test();   

    return 0;
}
