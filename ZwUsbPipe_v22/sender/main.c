#include "zw_usbpipe_base.h"
#include "zw_usbpipe_uptp.h"
#include "zw_usbpipe_uptp_api.h"

int main(int argc, char const *argv[])
{
    int deviceFd = -1;

    if (argc != 2)
    {
        err("Usage [%s [msg]]", argv[0]);
        return 0;
    }

    deviceFd = as_usbpipe_open(DEV_HOST);

    info("[%s %d]Send...\n", __FUNCTION__, __LINE__);
    struct zw_uptp_message msg;
    msg.msgType = MSG_TYPE_REQUEST;
    msg.seq = 1;
    msg.dataLen = strlen(argv[1]);
    snprintf(msg.data, sizeof(msg.data), "%s", argv[1]);

    zw_uptp_send(deviceFd, &msg);

    as_usbpipe_close(deviceFd);

    return 0;
}

