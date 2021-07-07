#include <stdio.h>

#include "bitrate_calc.h"

int main(int argc, char* argv[])
{
    printf("hello world!\n");
    rc_check(0);
    BitrateItem item ={0};
    rc_filled(0, item);

    return 0;
}
