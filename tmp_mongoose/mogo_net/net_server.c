#include <stdio.h>
#include "mongoose.h"
#include "op_db.h"

#define HGH_DBG(fmt, args...) printf("\033[40;33m HGH_DBG(%s %s %d):\t\033[0m"fmt, __FILE__, __func__, __LINE__, ## args)

static const char *s_http_port = "8700";
static int s_sig_num = 0;
static void *s_db_handle = NULL;
static const char *s_db_path = "net.db";
static struct mg_serve_http_opts s_http_server_opts;
static const struct mg_str s_get_method = MG_MK_STR("GET");
static const struct mg_str s_put_method = MG_MK_STR("PUT");
static const struct mg_str s_post_method = MG_MK_STR("POST");
static const struct mg_str s_delete_method = MG_MK_STR("DELETE");


static void single_handler(int sig_num)
{
    signal(sig_num, single_handler);
    s_sig_num = sig_num;
}

static int has_prefix(const struct mg_str *url, const struct mg_str *prefix)
{
    if (NULL == url || NULL == prefix)
    {
        HGH_DBG("Param Error! url[%p] prefix[%p]\n", url, prefix);
        return 0;
    }

    return url->len > prefix->len && memcmp(url->p, prefix->p, prefix->len) == 0;
}

static int is_equal(const struct mg_str *s1, const struct mg_str *s2)
{
    if (NULL == s1 || NULL == s2)
    {
        HGH_DBG("Param Error! s1[%p] s2[%p]\n", s1, s2);
        return 0;
    }
    
    return s1->len == s2->len && memcmp(s1->p, s2->p, s1->len) == 0;
}

static void mg_str_printf(struct mg_str *str)
{
    if (NULL == str) {
        return;
    }

    int len = str->len + 1;
    char *s = malloc(len);

    snprintf(s, len, "%s", str->p);

    HGH_DBG("Str[%s]\n", s);

    free(s);

    return ;
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data)
{
    static const struct mg_str api_prefix = MG_MK_STR("/Net/");
    struct http_message *hm = (struct http_message *) ev_data;
    struct mg_str key;

    switch (ev) {
        case MG_EV_HTTP_REQUEST: {
            if (has_prefix(&hm->uri, &api_prefix)) {
                key.p = hm->uri.p + api_prefix.len;
                key.len = hm->uri.len - api_prefix.len;
                mg_str_printf(&key);
                mg_str_printf(&hm->body);
                mg_str_printf(&hm->method);
                if (is_equal(&hm->method, &s_get_method)) {
                    db_op(nc, hm, &key, s_db_handle, API_OP_GET);
                } else if (is_equal(&hm->method, &s_put_method)) {
                    db_op(nc, hm, &key, s_db_handle, API_OP_PUT);
                }  else if (is_equal(&hm->method, &s_post_method)) {
                    char tmp[128] = "{\"status\":\"ok\"}";
                    mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", strlen(tmp), tmp);  
                }  else if (is_equal(&hm->method, &s_delete_method)) {
                    char tmp[128] = "{\"status\":\"delete\"}";
                    mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", strlen(tmp), tmp);  
                } else {
                    mg_printf(nc, "%s",
                            "HTTP/1.0 501 Not Implemented\r\n"
                            "Content-Length:0\r\n\r\n");
                }
            } else {
                mg_printf(nc, "%s", 
                        "HTTP/1.0 200 OK\r\n"
                        "Content-Length:0\r\n\r\n");
            }
        }
        break;
        default:
        break;
    }
}

static void checkparam()
{
    HGH_DBG("check param!\n");

    return;
}

int main(int argc, char const *argv[])
{
    struct mg_mgr mgr;
    struct mg_connection *nc;

    // 初始化mgr
    mg_mgr_init(&mgr, NULL);

    // 绑定端口和回调接口
    nc = mg_bind(&mgr, s_http_port, ev_handler);
    mg_set_protocol_http_websocket(nc);
    s_http_server_opts.document_root = "web_root ";

    // 注册信号处理函数
    signal(SIGINT, single_handler);
    signal(SIGTERM, single_handler);

    // 打开数据库
    s_db_handle = db_open(s_db_path);

    // 检查配置是否存在
    checkparam();

    // event loop
    HGH_DBG("Start Listening Port %s ...\n", s_http_port);
    while (s_sig_num == 0) {
        mg_mgr_poll(&mgr, 1000);
    }

    // 关闭数据库
    db_close(&s_db_handle);

    // 清理mgr
    mg_mgr_free(&mgr);

    return 0;
}
