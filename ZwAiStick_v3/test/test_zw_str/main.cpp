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

int main(int argc, char const *argv[])
{
	return 0;
}
