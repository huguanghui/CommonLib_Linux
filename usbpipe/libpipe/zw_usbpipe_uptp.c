#include "zw_usbpipe_base.h"
#include "zw_usbpipe_uptp.h"

int zw_uptp_send(int fd, struct zw_uptp_message* msg)
{
    info("[%s %d]\n", __FUNCTION__, __LINE__);
    
    int dataLen = msg->dateLen + 16;
    msg->identify[0] = 'P';
    msg->identify[1] = 'I';
    msg->identify[2] = 'P';
    msg->identify[3] = 'E';

    as_usbpipe_send(fd, (char*)msg, dataLen);

    return 0;
}

int zw_uptp_recive(int fd, struct zw_uptp_message* msg)
{
    info("[%s %d]\n", __FUNCTION__, __LINE__);
    char* result = NULL;
    int resultLen = 0;

    as_usbpipe_recv(fd, &result, &resultLen);
    memcpy(msg, result, resultLen);
    free(result);

    return 0;
}
