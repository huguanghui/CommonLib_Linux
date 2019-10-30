#include <stdio.h>

typedef enum errorcode{
    E_NOT_SUPPORT = -1,
    E_OK = 0, // 成功
}ERRCODE_E;

typedef struct CodeStringTable
{
    ERRCODE_E e;
    const char *pStr;
}CodeTable_ST;

static const CodeTable_ST errcode_map[] = 
{
    {E_OK, "Success"},
    {E_NOT_SUPPORT, "Not Support"}
};

const char *get_code_description(ERRCODE_E e)
{
    int i = 0;
    for (i = 0; i < sizeof(errcode_map)/sizeof(errcode_map[0]); i++)
    {
        if (errcode_map[i].e == e)
        {
            return errcode_map[i].pStr;
        }
    }
    return "Error code unknown";
}

int main(int argc, char const *argv[])
{
    printf("Code:%s\n", get_code_description(E_NOT_SUPPORT));
    return 0;
}

