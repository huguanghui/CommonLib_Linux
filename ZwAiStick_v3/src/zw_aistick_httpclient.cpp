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

int HTTP_ON_USBPIPE_PUT(userAuth* auth, const char* url, const char* extra_headers, void* in, int inSize, void** out, int* outSize)
{
	int ret = 0;
	char header[1024];
	struct mbuf auth;
	struct mbuf send_mbuf;
	struct mbuf recv_mbuf;
	AS_TASK_REQUEST req;
	AS_TASK_RESULT result;

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

	memset(&req, 0, sizeof(AS_TASK_REQUEST));
	memset(&result, 0, sizeof(AS_TASK_RESULT));
	req.cmd = AS_TASK_HTTP_CONFIG;
	req.paramLen = send_mbuf.len;
	req.param = send_mbuf.buf;

	int rc = AS_TASK_Run(&req, &result);
	if (0 == rc)
	{
		recv_mbuf.buf = result.param;
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

	return ret;
}
