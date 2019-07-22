#ifndef __ZW_AISTICK_HTTP_CLIENT_H__
#define __ZW_AISTICK_HTTP_CLIENT_H__

struct userAuth {
	char username[32];
	char password[32];
	int type;  // 0 - basic
};

#ifdef __cplusplus
extern "C" {
#endif

int HTTP_ON_USBPIPE_PUT(userAuth *auth, const char *url, const char *extra_headers, void *in, int inSize, void **out, int *outSize);

int HTTP_ON_USBPIPE_GET(userAuth *auth, const char *url, const char *extra_headers, void *in, int inSize, void **out, int *outSize);

int HTTP_ON_USBPIPE_POST(userAuth *auth, const char *url, const char *content_type, const char *extra_headers, void *in, int inSize, void **out, int *outSize);

#ifdef __cplusplus
}
#endif

#endif
