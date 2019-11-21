#include <stdio.h>
#include "macros.h"
#include "vim.h"

static mparm_T params;

void main(int argc, char *argv[])
{
    int i;

    vim_memset(&params, 0, sizeof(params));  
    params.argc = argc;
    params.argv = argv;

    for (i = 0; i < argc; ++i) {
       if (STRICMP(argv[i]) == 0) {
           params.clean = TRUE;
           break;
       } 
    }
}
