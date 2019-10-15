#include <stdio.h>

union Endian{
    int a;
    char ch[4];
} ed;
int main()
{
    ed.a = 0x01234567;  
    printf("%p\n%p\t%p\n", &ed.a, &ed.ch[0], &ed.ch[1]);
    printf("%x", ed.ch[0]);
    return 0;
}
