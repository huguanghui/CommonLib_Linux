#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    unsigned int n = 0;

    n = 10 * 44100 * 16 / 8 * 2;
    n = n / 1024;

    double d = n*1.0 / 1024;

    printf("n = %d\n", n);
    printf("d = %f\n", d);

    return EXIT_SUCCESS;
}
