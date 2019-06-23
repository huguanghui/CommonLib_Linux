#include "zw_usbpipe_base.h"
#include "zw_usbpipe_uptp.h"

int main(int argc, char const *argv[])
{
    int deviceFd = -1;
    int rc = 0;
    char buf[1024];
    int bufLen;
    char *result = NULL;
    int resLen = 0;

    deviceFd = as_usbpipe_open();

    struct zw_uptp_message msg;
    HGH_DBG("\n");
    info("[%s %d]Recv...\n", __FUNCTION__, __LINE__);
    zw_uptp_recive(deviceFd, &msg);

    info("[%s %d]Send...\n", __FUNCTION__, __LINE__);
    
    msg.msgType = MSG_TYPE_RESPONSE;
    msg.dateLen = strlen("hello back!");
    snprintf(msg.data, sizeof(msg.data), "hello back!");

    int dateLen = msg.dateLen + 12;

    zw_uptp_send(deviceFd, &msg);

    as_usbpipe_close(deviceFd);

    return 0;
}

