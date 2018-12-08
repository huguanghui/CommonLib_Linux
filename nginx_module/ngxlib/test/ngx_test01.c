#include <nginx_module/ngxlib/ngx_config.h>
#include <nginx_module/ngxlib/ngx_core.h>

int main(int argc, char const *argv[])
{
    ngx_str_t tmpStr;

    if (ngx_strerror_init() != NGX_OK)
    {
        return 1;
    }
    ngx_str_set(&tmpStr, "abcdefg");
    printf("%s\n", tmpStr.data);

    return 0;
}
