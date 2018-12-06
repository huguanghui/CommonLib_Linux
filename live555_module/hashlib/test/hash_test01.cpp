#include <iostream>
#include <live555_module/hashlib/strDup.h>
#include <live555_module/hashlib/hashtable.h>

const char* g_str = "aabbccdd";

int main(int argc, char const *argv[])
{
    char* tmp = strDup(g_str);

    std::cout << "Echo " << tmp << std::endl;

    return 0;
}
