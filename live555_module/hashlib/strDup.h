#ifndef _STR_DUP_H
#define _STR_DUP_H

#include <string.h>

char* strDup(char const* str);

char* strDupSize(char const* str);

char* strDupSize(char const* str, size_t& resultBufSize);

#endif