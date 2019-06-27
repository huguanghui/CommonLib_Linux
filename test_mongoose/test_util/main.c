#include <stdio.h>
#include "mg_util.h"

int main(int argc, char* argv[])
{
    struct mbuf m;

    mbuf_init(&m, 0);

    const char* str = "hello world!";

    mbuf_append(&m, str, strlen(str));
    printf("M[%.*s]\n", (int)m.len, m.buf);

    struct mg_str s = mg_mk_str(str);
    printf("str[%.*s]\n", (int)s.len, s.p);

    mbuf_free(&m);

    return 0;
}
