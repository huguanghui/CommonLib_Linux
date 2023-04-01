#include <stdio.h>

int main()
{
    union {
        short s;
        char c[sizeof(short)];
    } un;

    un.s = 0x0102;

    if (sizeof(short) == 2) {
        if (un.c[0] == 0x02 && un.c[1] == 0x01)
            printf("Little endian\n");
        else if (un.c[0] == 0x01 && un.c[1] == 0x02)
            printf("Big endian\n");
        else
            printf("Unknown\n");
    } else {
        printf("sizeof(short) = %d\n", sizeof(short));
    }

    return 0;
}
