#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdarg.h>
#include "mg_util.h"

// 获取 header 的长度
static int http_get_request_len(const char *s, int buf_len) {
	const unsigned char *buf = (unsigned char *) s;
	int i;

	for (i = 0; i < buf_len; i++) {
		if (!isprint(buf[i]) && buf[i] != '\r' && buf[i] != '\n' && buf[i] < 128) {
			return -1;
		} else if (buf[i] == '\n' && i + 1 < buf_len && buf[i + 1] == '\n') {
			return i + 2;
		} else if (buf[i] == '\n' && i + 2 < buf_len && buf[i + 1] == '\r' &&
								buf[i + 2] == '\n') {
			return i + 3;
		}
	}

	return 0;
}

int HTTP_ON_USBPIPE_PUT(const char* user, const char* name, const char* url, const char* extra_headers, void* in, int inSize, void** out, int* outSize)
{
	int ret = 0;
	char header[1024];
	struct mbuf auth;
	struct mbuf send_mbuf;
	struct mbuf recv_mbuf;

	mbuf_init(&send_mbuf, 0);
	mbuf_init(&recv_mbuf, 0);

	// 发送内容拼接
	// header
	snprintf(header, sizeof(header), "PUT %s HTTP/1.1\r\nHost: localhost\r\nContent-Length: %u\r\n"
				"%.*s%s\r\n", url, inSize, auth.len, (auth.buf == NULL?"":auth.buf), extra_headers);
	mbuf_append(&send_mbuf, header, strlen(header));
	// body
	if (in != NULL && inSize > 0)
	{
		mbuf_append(&send_mbuf, in, inSize);
	}

    int rc = 0;
//	int rc = AS_TASK_Run(&req, &result);
	if (0 == rc)
	{
		int len = http_get_request_len(recv_mbuf.buf, recv_mbuf.len);
		if (len > 0)
		{
			mbuf_remove(&recv_mbuf, len);
			*out = recv_mbuf.buf;
			*outSize = recv_mbuf.len;
		}
		else
		{
			mbuf_free(&recv_mbuf);
		}
		
	}
	mbuf_free(&send_mbuf);

	return ret;
}

#define SERV_PORT 8000

#define MG_BUFFER_SIZE 100

int mg_avprintf(char **buf, int size, const char *fmt, va_list ap)
{
    va_list ap_copy;
    int len;

    va_copy(ap_copy, ap);
    len = vsnprintf(*buf, size, fmt, ap_copy);
    if (len >= size)
    {
        *buf = (char *)malloc(len+1);
        va_copy(ap_copy, ap);
        len = vsnprintf(*buf, len+1, fmt, ap_copy);
        va_end(ap_copy);
    }
    return len;
}

int mg_vprintf(struct mbuf *m, const char *fmt, va_list ap)
{
    int len;
    char mem[MG_BUFFER_SIZE], *buf = mem;

    if ((len = mg_avprintf(&buf, sizeof(mem), fmt, ap)) > 0)
    {
        mbuf_append(m, buf, len);
    }
    if (buf != mem && buf != NULL)
    {
        free(buf);
    }

    return len;
}

int mg_printf(struct mbuf *m, const char *fmt, ...)
{
    int len;
    va_list ap;
    va_start(ap, fmt);
    len = mg_vprintf(m, fmt, ap);
    va_end(ap);

    return len;
}

void gen_requeset(struct mbuf* m, const char* url)
{
    mg_printf(m, "GET %s HTTP/1.1\r\nHost: localhost:8000\r\nUser-Agent: PostmanRuntime/7.15.0\r\nAccept: */*\r\nCache-Control: no-cache\r\nHost: localhost:8000\r\naccept-encoding: gzip, deflate\r\nConnection: keep-alive\r\ncache-control: no-cache", url);

    return;
}

int main(int argc, char* argv[])
{
    int sockfd;
    int n;
    struct sockaddr_in servaddr;

    // 1. 创建套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.3.145", &servaddr.sin_addr);
    servaddr.sin_port = htons(SERV_PORT);
    // 2. 与服务器建立连接
    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr));
    // 3. 写数据
    struct mbuf send_buf;
    mbuf_init(&send_buf, 0);
    //struct mg_str sd = mg_mk_str("hello");
    gen_requeset(&send_buf, "/api/v1/a");
    printf("send[%.*s]\n", send_buf.len, send_buf.buf);
    int len = send(sockfd, send_buf.buf, send_buf.len, 0);
    printf("writelen[%d]\n", len);
    // 4. 读数据
    char buf[1024];
    n = recv(sockfd, buf, 1024, 0);
    printf("recv[%.*s]\n", n, buf);
    // 5. 关闭套接字
    close(sockfd);
    
    mbuf_free(&send_buf);

    return 0;
}
