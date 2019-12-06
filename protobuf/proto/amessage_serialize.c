#include <stdio.h>
#include <stdlib.h>

#include "amessage.pb-c.h"

int main(int argc, char *argv[])
{
    AMessage msg = AMESSAGE__INIT; 
    void *buf;
    unsigned len;

    // 读取输入
    if (argc != 2 && argc != 3)
    {
        fprintf(stderr, "usage: amessage a [b]\n");
        return 1;
    }
    msg.a = atoi(argv[1]);
    if (argc == 3)
    {
        msg.has_b = 1;
        msg.b = atoi(argv[2]);
    }
    len = amessage__get_packed_size(&msg);
    // 序列化
    buf = malloc(len);
    amessage__pack(&msg, buf);
    // 释放资源
    fprintf(stderr, "Writing %d serialized bytes\n", len);
    fwrite(buf, len, 1, stdout);
    free(buf);
    
    return 0;
}
