#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "zw_aistick_util.h"
#include "zw_str.h"

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

void mg_mbuf_append_base64_putc(char ch, void *user_data) {
	struct mbuf *mbuf = (struct mbuf *) user_data;
	mbuf_append(mbuf, &ch, sizeof(ch));
}

void mg_basic_auth_header(const struct mg_str user, const struct mg_str pass,
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

int main(int argc, char const *argv[])
{
	struct mg_str t = mg_mk_str("hello");

	printf("T[%.*s]\n", t.len, t.p);

	struct mbuf buf;
	mbuf_init(&buf, 0);

	struct mg_str user = mg_mk_str("admin");
	struct mg_str pass = mg_mk_str("");

	mg_basic_auth_header(user, pass, &buf);
	printf("Auth[%.*s]\n", buf.len, buf.buf);

	mbuf_free(&buf);

	return 0;
}
