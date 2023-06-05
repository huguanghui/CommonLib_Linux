#include <stdio.h>
#include <string.h>

#include "TinyFrame.h"
#include "utils.h"

TinyFrame *demo_tf;

bool do_corrupt = false;

void TF_WriteImpl(TinyFrame *tf, const uint8_t *buff, uint32_t len)
{
    printf("--------------------\n");
    printf("\033[32mTF_WriteImpl - sending frame:\033[0m\n");

    uint8_t *xbuff = (uint8_t *)buff;
    if (do_corrupt) {
        printf("(corrupting to test checksum checking...)\n");
        xbuff[8]++;
    }

    dumpFrame(xbuff, len);

    // Send it back as if we received it
    TF_Accept(tf, xbuff, len);

    return;
}

TF_Result myListener(TinyFrame *tf, TF_Msg *msg)
{
    dumpFrameInfo(msg);
    return TF_STAY;
}

TF_Result testIdListener(TinyFrame *tf, TF_Msg *msg)
{
    printf("OK - ID Listener triggered for msg!\n");
    dumpFrameInfo(msg);
    return TF_CLOSE;
}

int main(int argc, char *argv[])
{
    TF_Msg msg;

    const char *longstr = "Lorem ispum dolor sit emet.";

    demo_tf = TF_Init(TF_MASTER);
    TF_AddGenericListener(demo_tf, myListener);

    TF_ClearMsg(&msg);
    msg.type = 0x22;
    msg.data = (uint8_t *)"Hello TinyFrame";
    msg.len = 16;
    TF_Send(demo_tf, &msg);

    msg.type = 0x33;
    msg.data = (uint8_t *)longstr;
    msg.len = (TF_LEN)(strlen(longstr) + 1);
    TF_Send(demo_tf, &msg);

    msg.len = 0;
    msg.type = 0x77;
    TF_Query(demo_tf, &msg, testIdListener, NULL, 0);

    do_corrupt = true;
    msg.type = 0x44;
    msg.data = (uint8_t *)"Hello2";
    msg.len = 7;
    TF_Send(demo_tf, &msg);

    return 0;
}
