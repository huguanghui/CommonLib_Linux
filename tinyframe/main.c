#include <stdio.h>

#include "TinyFrame.h"
#define PRO 0

static TinyFrameRx s_rxStr;
static verStatusTypedef s_verStatus;

static void rx_and_parse_data(void);

int main(int argc, char *argv[])
{
    printf("[HGH-TEST][%s %d]\n", __FUNCTION__, __LINE__);
    uint8_t tx_arr[4] = {0x01, 0x01, 0x00, 0x00};
    tinyFramefTx(0xFFFF, (uint8_t*)tx_arr, sizeof(tx_arr)/sizeof(tx_arr[0]));
    rx_and_parse_data();

    return 0;
}

static void rx_and_parse_data(void)
{
    s_rxStr = tinyFramefRx();

    if (s_rxStr.receiveType == 0xFFFF) {
#ifdef GUI
        printf("Successfully received TF frame data and parsed the type to "
               "obtain version information.\n");
#endif
        s_verStatus.project = PRO;
        s_verStatus.version_major = 1;
        s_verStatus.version_minor = 0;
        s_verStatus.version_patch = 0;
#ifdef GUI
        printf("Return data.\n");
#endif
        tinyFramefTx(s_rxStr.receiveType, (uint8_t *)(&s_verStatus),
            sizeof(s_verStatus));
    }

    return;
}
