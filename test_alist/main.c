#include <stdio.h>
#include "alist.h"


int main(int argc, char* argv[])
{
    ALIST_T alist;

    alist_init(&alist);
    
    int i = 0;

    for (i = 0; i < 10; i++)
    {
        alist_malloc(&alist, 1024); 
    }

    echo_alist(&alist);

    alist_uninit(&alist);

    return 0;
}
