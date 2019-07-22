#include <string>
#include <fstream>
#include <ctype.h>
#include "zw_usbpipe_util.h"
#include "zw_aistick_task.h"
#include "zw_aistick_httpclient.h"

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

static void mg_mbuf_append_base64_putc(char ch, void *user_data) {
	struct mbuf *mbuf = (struct mbuf *) user_data;
	mbuf_append(mbuf, &ch, sizeof(ch));
}

static void mg_basic_auth_header(const struct mg_str user, const struct mg_str pass,
                          struct mbuf *buf) {
	const char *header_prefix = "Authorization: Basic ";
	const char *header_suffix = "\r\n";

	struct cs_base64_ctx ctx;
	cs_base64_init(&ctx, mg_mbuf_append_base64_putc, buf);

	mbuf_append(buf, header_prefix, strlen(header_prefix));

	cs_base64_update(&ctx, user.p, user.len);
	if (pass.len >= 0) {
	cs_base64_update(&ctx, ":", 1);
	cs_base64_update(&ctx, pass.p, pass.len);
	}
	cs_base64_finish(&ctx);
	mbuf_append(buf, header_suffix, strlen(header_suffix));
}

int HTTP_ON_USBPIPE_PUT(userAuth* auth, const char* url, const char* extra_headers, void* in, int inSize, void** out, int* outSize)
{
	int ret = 0;
	char header[1024];
	struct mbuf auth_mbuf;
	struct mbuf send_mbuf;
	struct mbuf recv_mbuf;
	AS_TASK_REQUEST req;
	AS_TASK_RESULT result;

	mbuf_init(&auth_mbuf, 0);
	mbuf_init(&send_mbuf, 0);
	mbuf_init(&recv_mbuf, 0);

	// Auth 处理
	struct mg_str user = mg_mk_str(auth->username);
	struct mg_str passwd = mg_mk_str(auth->password);
	mg_basic_auth_header(user, passwd, &auth_mbuf);

	// 发送内容拼接
	// header
	snprintf(header, sizeof(header), "PUT %s HTTP/1.1\r\nHost: 192.168.1.11\r\nContent-Type: application/xml\r\n"
			"%.*sUser-Agent: USBPIPE/1.0\r\nContent-Length: %u\r\nConnection: keep-alive\r\n%s\r\n"
			, url, auth_mbuf.len, (auth_mbuf.buf == NULL?"":auth_mbuf.buf), inSize, (extra_headers == NULL?"":extra_headers));
	mbuf_append(&send_mbuf, header, strlen(header));
	// body
	if (in != NULL && inSize > 0)
	{
		mbuf_append(&send_mbuf, in, inSize);
	}
	//info("request[%.*s]\n", send_mbuf.len, send_mbuf.buf);
	memset(&req, 0, sizeof(AS_TASK_REQUEST));
	memset(&result, 0, sizeof(AS_TASK_RESULT));
	req.cmd = AS_TASK_HTTP_CONFIG;
	req.paramLen = send_mbuf.len;
	req.param = send_mbuf.buf;

	int rc = AS_TASK_Run(&req, &result);
	if (0 == rc)
	{
		recv_mbuf.buf = (char *)result.param;
		recv_mbuf.len = result.paramLen;
		recv_mbuf.size = result.paramLen;
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
	mbuf_free(&auth_mbuf);

	return ret;
}

int HTTP_ON_USBPIPE_GET(userAuth *auth, const char *url, const char *extra_headers, void *in, int inSize, void **out, int *outSize)
{
	int ret = 0;
	char header[1024];
	struct mbuf auth_mbuf;
	struct mbuf send_mbuf;
	struct mbuf recv_mbuf;
	AS_TASK_REQUEST req;
	AS_TASK_RESULT result;

	mbuf_init(&auth_mbuf, 0);
	mbuf_init(&send_mbuf, 0);
	mbuf_init(&recv_mbuf, 0);

	// Auth 处理
	struct mg_str user = mg_mk_str(auth->username);
	struct mg_str passwd = mg_mk_str(auth->password);
	mg_basic_auth_header(user, passwd, &auth_mbuf);

	// 发送内容拼接
	// header
	snprintf(header, sizeof(header), "GET %s HTTP/1.1\r\nHost: 192.168.1.11\r\nContent-Type: application/xml\r\n"
			"%.*sUser-Agent: USBPIPE/1.0\r\nContent-Length: %u\r\nConnection: keep-alive\r\n%s\r\n"
			, url, auth_mbuf.len, (auth_mbuf.buf == NULL?"":auth_mbuf.buf), inSize, (extra_headers == NULL?"":extra_headers));
	mbuf_append(&send_mbuf, header, strlen(header));
	// body
	if (in != NULL && inSize > 0)
	{
		mbuf_append(&send_mbuf, in, inSize);
	}
	//info("request[%.*s]\n", send_mbuf.len, send_mbuf.buf);
	memset(&req, 0, sizeof(AS_TASK_REQUEST));
	memset(&result, 0, sizeof(AS_TASK_RESULT));
	req.cmd = AS_TASK_HTTP_CONFIG;
	req.paramLen = send_mbuf.len;
	req.param = send_mbuf.buf;

	int rc = AS_TASK_Run(&req, &result);
	if (0 == rc)
	{
		recv_mbuf.buf = (char *)result.param;
		recv_mbuf.len = result.paramLen;
		recv_mbuf.size = result.paramLen;
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
	mbuf_free(&auth_mbuf);

	return ret;
}

int HTTP_ON_USBPIPE_POST(userAuth *auth, const char *url, const char *content_type, const char *extra_headers, void *in, int inSize, void **out, int *outSize)
{
	if (NULL == auth || NULL == url || NULL == content_type)
	{
		err("param err! auth[%p] url[%p] content_type[%p]\n", auth, url, content_type);
		return -1;
	}

	int ret = 0;
	char header[1024];
	struct mbuf auth_mbuf;
	struct mbuf send_mbuf;
	struct mbuf recv_mbuf;
	AS_TASK_REQUEST req;
	AS_TASK_RESULT result;

	mbuf_init(&auth_mbuf, 0);
	mbuf_init(&send_mbuf, 0);
	mbuf_init(&recv_mbuf, 0);

	// Auth 处理
	struct mg_str user = mg_mk_str(auth->username);
	struct mg_str passwd = mg_mk_str(auth->password);
	mg_basic_auth_header(user, passwd, &auth_mbuf);

	// 发送内容拼接
	// header
	snprintf(header, sizeof(header), "POST %s HTTP/1.1\r\nHost: 192.168.1.11\r\nContent-Type: %s\r\n"
			"%.*sUser-Agent: USBPIPE/1.0\r\nContent-Length: %u\r\nConnection: keep-alive\r\n%s\r\n"
			, url, content_type, auth_mbuf.len, (auth_mbuf.buf == NULL?"":auth_mbuf.buf), inSize, (extra_headers == NULL?"":extra_headers));
	mbuf_append(&send_mbuf, header, strlen(header));
	// body
	if (in != NULL && inSize > 0)
	{
		mbuf_append(&send_mbuf, in, inSize);
	}
	//info("request[%.*s]\n", send_mbuf.len, send_mbuf.buf);
	memset(&req, 0, sizeof(AS_TASK_REQUEST));
	memset(&result, 0, sizeof(AS_TASK_RESULT));
	req.cmd = AS_TASK_HTTP_CONFIG;
	req.paramLen = send_mbuf.len;
	req.param = send_mbuf.buf;

	int rc = 0;
	rc = AS_TASK_Run(&req, &result);
	if (0 == rc)
	{
		recv_mbuf.buf = (char *)result.param;
		recv_mbuf.len = result.paramLen;
		recv_mbuf.size = result.paramLen;
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
	mbuf_free(&auth_mbuf);

	return ret;
}
