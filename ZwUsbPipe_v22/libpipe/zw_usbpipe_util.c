#include <stdlib.h>
#include <ctype.h>
#include "zw_usbpipe_util.h"

static const char* g_arrMsgType[2] = {
	"Msg-Request",
	"Msg-Response"
};

const char* getMsgTypeName(E_MSG_TYPE e)
{
  return g_arrMsgType[e];
}

static const char g_module_name[] = "usbpipe";
static unsigned verbosity = 7;

static char* getFileName(const char *p)
{
	char ch = '/';
	char *q = strrchr(p, ch) + 1;

	return q;
}

void _msg(unsigned level, const char* file, const char* fun, int line,
        const char *fmt, ...)
{
	if (level < 2)
		level = 2;
	else if (level > 7)
		level = 7;

	if (level <= verbosity) {
		static const char levels[8][6] = {
			[2] = "crit",
			[3] = "err",
			[4] = "warn",
			[5] = "note",
			[6] = "info",
			[7] = "dbg"
		};
		static const char color[8][6] = {
			[2] = "32",
			[3] = "31",
			[4] = "35",
			[5] = "34",
			[6] = "30",
			[7] = "32"
		};


		int _errno = errno;
		va_list ap;

		fprintf(stderr, "\033[%sm[%s: %s]-[%s-%s:%d] ", color[level], g_module_name, levels[level], getFileName(file), fun, line);

		va_start(ap, fmt);
		vfprintf(stderr, fmt, ap);
		va_end(ap);

		if (fmt[strlen(fmt) - 1] != '\n') {
			char buffer[128];
			strerror_r(_errno, buffer, sizeof buffer);
			fprintf(stderr, ": (-%d) %s\n", _errno, buffer);
		}
		fprintf(stderr, "\033[0m");

		fflush(stderr);
	}
}

void HexOutput(void *buf, size_t len)
{
	char *b = (char*)buf;
	int numCol = 0;

	printf("Hen[%p]Len[%d]:\n", buf, len);
	size_t i = 0;
	for(i=0; i<len; i=i+16)
	{
		printf("%p|", b+i);
		numCol = (len-i>=16) ? 16 : (len-i);    
		int j = 0;
		for (j = 0; j < numCol; j++)
		{
			unsigned char c = b[i+j]; // must use unsigned char to print >128 value
			if( c < 16)
			{
				printf(" 0%x", c);
			}
			else
			{
				printf(" %x", c);
			}
		}
		printf("\n");
	}
	printf("\n");
}
