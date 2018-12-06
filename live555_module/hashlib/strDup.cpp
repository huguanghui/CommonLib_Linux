#include "strDup.h"

char* strDup(char const* str)
{
    if (str == NULL)
    {
        return NULL;
    }

    size_t len = strlen(str) + 1;
    char* copy = new char[len];

    if (copy != NULL)
    {
        memcpy(copy, str, len);
    }

    return copy;
}

char* strDupSize(char const* str)
{
    size_t dummy;

    return strDupSize(str, dummy);
}

char* strDupSize(char const* str, size_t& resultBufSize)
{
    if (str == NULL)
    {
        resultBufSize = 0;
        return NULL;
    }

    resultBufSize = strlen(str) + 1;
    char* copy = new char[resultBufSize];

    return copy;
}