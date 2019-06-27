#include <stdio.h>
#include <string.h>
#include "mbuf.h"

int main(int argc, char const *argv[])
{
    struct mbuf t;

    mbuf_init(&t, 0);

    const char* str="hello world!";

    mbuf_append(&t, str, strlen(str));

    printf("buf[%.*s]\n", t.len, t.buf);
    
    mbuf_free(&t);

    return 0;
}
