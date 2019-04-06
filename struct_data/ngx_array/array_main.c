#include "ngx_array.h"

//#define HGH_DBG(fmt, args...) printf("\033[40;33m HGH_DBG(%s %s %d):\t\033[0m"fmt, __FILE__, __func__, __LINE__, ## args)

struct tagAbc {
    int id;
    char key[32];
    char value[32];
};

void array_abc_echo(ngx_array_t *a)
{
    int i;
    struct tagAbc *array = (struct tagAbc *)a->elts;
    struct tagAbc *tmp = NULL;

    if (NULL == a)
    {
        return ;
    }
    for (i = 0; i < a->nelts; i++)
    {
        tmp = (struct tagAbc*)array + i;
        HGH_DBG("##################\n");
        HGH_DBG("# ID: %d\n", tmp->id);
        HGH_DBG("# KEY: %s\n", tmp->key);
        HGH_DBG("# VALUE: %s\n", tmp->value);
        HGH_DBG("##################\n");
    }
}

int main(int argc, char const *argv[])
{
    ngx_pool_t *pool = NULL;
    ngx_array_t *array = NULL;
    struct tagAbc *tmp = NULL;
    int i;

    // 创建内存池
    pool = ngx_create_pool(1024);
    // 创建动态数组
    array = ngx_array_create(pool, 10, sizeof(struct tagAbc));
    for (i = 0; i < 5; i++)
    {
        tmp = (struct tagAbc*)ngx_array_push(array);
        tmp->id = i;
        snprintf(tmp->key, sizeof(tmp->key), "key-%d", i);
        snprintf(tmp->value, sizeof(tmp->value), "value-%d", i);
        printf("nelt: %d\n", array->nelts);
    }
    array_abc_echo(array);

    // 销毁动态数组
    ngx_array_destroy(array);
    // 销毁内存池
    ngx_destroy_pool(pool);

    return 0;
}
