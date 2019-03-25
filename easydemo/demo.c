#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int pagesize = getpagesize();
    printf("PageSize: %d\n", pagesize);

    return 0;
}
