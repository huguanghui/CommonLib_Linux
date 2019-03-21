#include <stdio.h>
#include <stdlib.h>

#include "shareMem.h"

int main(int argc, char const *argv[])
{
    int td;
    td = CreateShm(1024);

    return 0;
}

