#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <curl/curl.h>
 

#define HGH_DBG(fmt, args...) printf("\033[40;33m HGH_DBG(%s %s %d):\t\033[0m" fmt, __FILE__, __func__, __LINE__, ## args)

typedef enum {
    HTTP_GET,
    HTTP_PUT,
    HTTP_POST,
    HTTP_DELETE,
} HTTP_Method;

typedef struct tagUserAuth {
    char username[32];
    char password[32];
    int type;  // 0 - basic
} UserAuth;

typedef struct tagHTTPHeader {
    int status;
    int contentLength;
} HTTPHeader;

typedef struct tagCurlWriteData {
    char *ptr;
    int size;
    int pos;
} CurlWriteData;

typedef struct tagCurlReadData {
    char *ptr;
    int size;
    int pos;
} CurlReadData;

typedef struct tagCurlRestfulData {
    HTTPHeader header;
    CurlWriteData writeData;
    CurlReadData readData;
} CurlRestfulData;

int Restful_get(UserAuth* auth, const char* url, char* in, int inSize, char** out, int *outSize);
int Restful_put(UserAuth* auth, const char* url, char* in, int inSize, char** out, int *outSize);
int Restful_post(UserAuth* auth, const char* url, char* in, int inSize, char** out, int *outSize);
int Restful_delete(UserAuth* auth, const char* url, char* in, int inSize, char** out, int *outSize);

/* parse headers for Content-Length */  
static size_t GetHttpStatusFunc(void *ptr, size_t size, size_t nmemb, void *stream)   
{
	int r;  
	int len = 0;  
    CurlRestfulData* restfulData = (CurlRestfulData*)stream;
    HTTPHeader* header = &restfulData->header;

	r = sscanf((const char*)ptr, "HTTP/1.0  %d\n", &len);
	if (r) /* Microsoft: we don't read the specs */
		header->status = len;

	r = sscanf((const char*)ptr, "HTTP/1.1  %d\n", &len);
	if (r) /* Microsoft: we don't read the specs */
		header->status = len;

	r = sscanf((const char*)ptr, "Content-Length:  %d\n", &len);
	if (r) /* Microsoft: we don't read the specs */
		header->contentLength = len;

    //HGH_DBG("ptr[%s]\n", ptr);
    //HGH_DBG("size[%d] nmemb[%d]\n", size, nmemb);

	return size * nmemb;
}

static size_t WriteFunc(void *ptr, size_t size, size_t nmemb, void *pUsrData)
{
    CurlRestfulData* restfulData = (CurlRestfulData*)pUsrData;
    CurlWriteData* data = &restfulData->writeData;

    if (NULL == data->ptr)
    {
        data->ptr = malloc(restfulData->header.contentLength + 1);
        data->size = restfulData->header.contentLength + 1;
        data->pos = 0;    
        HGH_DBG("data->ptr[%p]\n", data->ptr);
    }

    HGH_DBG("size[%ld] nmemb[%ld]\n", size, nmemb);
    memcpy(data->ptr + data->pos, ptr, size*nmemb);
    data->pos += size*nmemb;
    if (data->pos == restfulData->header.contentLength)
    {
        data->ptr[restfulData->header.contentLength] = '\0';     
    }

    return size*nmemb;
}

/**  HTTP 的 Get 接口
 @param method[in]  输入参数1
 @param auth[in]  验证信息
 @param url[in] 请求
 @param out[out] 响应
 @return        http的返回码
 @warning      curl不能为空
 @note          
 @see           
*/
int HTTPRestful(HTTP_Method method, UserAuth* auth, const char* url, char* in, int inSize, char** out, int *outSize)
{
    int iRet = 0;

    switch (method)
    {
    case HTTP_GET:
        {
            iRet = Restful_get(auth, url, in, inSize, out, outSize);
        }
        break;
    case HTTP_PUT:
        {
            iRet = Restful_put(auth, url, in, inSize, out, outSize);
        }
        break;
    case HTTP_POST:
        {
            iRet = Restful_post(auth, url, in, inSize, out, outSize);
        }
        break;
    case HTTP_DELETE:
        {
            iRet = Restful_delete(auth, url, in, inSize, out, outSize);
        }
        break;
    default:
        break;
    }

    return 0;
}

int Restful_get(UserAuth* auth, const char* url, char* in, int inSize, char** out, int *outSize)
{
    CURL* curl = NULL;
    struct curl_slist *headers = NULL;
    CURLcode res;
    CurlRestfulData restfulData;

    memset(&restfulData, 0, sizeof(restfulData));
    curl = curl_easy_init();

    // 设置HTTP头
    headers = curl_slist_append(headers, "Accept: ZOWELL");
    if (auth != NULL)
    {
        char authInfo[128] = {0};
        snprintf(authInfo, sizeof(authInfo), "%s:%s", auth->username, auth->password);
        curl_easy_setopt(curl, CURLOPT_USERPWD, authInfo);
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, GetHttpStatusFunc);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &restfulData);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &restfulData);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 3000L);
    //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); 
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", 
                curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if ((CURLE_OK == res) && (200 == restfulData.header.status))
    {
        *out = restfulData.writeData.ptr;
        *outSize = restfulData.writeData.size;
    }

    HGH_DBG("Status[%d] ContentLength[%d]\n", restfulData.header.status, restfulData.header.contentLength);

    return 0;
}

size_t ReadFunc(char *bufptr, size_t size, size_t nitems, void *pUsrData)
{
    CurlRestfulData* restfulData = (CurlRestfulData*)pUsrData;
     CurlReadData* data = &restfulData->readData;

    HGH_DBG("size[%ld] nmemb[%ld]\n", size, nitems);

    int left = data->size - data->pos;
    int copyLen = (left >= size*nitems)?size*nitems:left;
    memcpy(bufptr + data->pos, data->ptr + data->pos, copyLen);
    data->pos += copyLen;

    HGH_DBG("copyLen[%d]\n", copyLen);

    return copyLen;
}

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t retcode;
  curl_off_t nread;
 
  /* in real-world cases, this would probably get this data differently
     as this fread() stuff is exactly what the library already would do
     by default internally */ 
  retcode = fread(ptr, size, nmemb, stream);
 
  nread = (curl_off_t)retcode;
 
  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
          " bytes from file\n", nread);
 
  return retcode;
}

int Restful_put(UserAuth* auth, const char* url, char* in, int inSize, char** out, int *outSize)
{
    CURL* curl = NULL;
    struct curl_slist *headers = NULL;
    CURLcode res;
    CurlRestfulData restfulData;
    // FILE * hd_src;
    // struct stat file_info;
    // const char* file = "abc.txt";

    memset(&restfulData, 0, sizeof(restfulData));
    restfulData.readData.ptr = in;
    restfulData.readData.size = inSize;
    restfulData.readData.pos = 0;
    
    // stat(file, &file_info);
    // hd_src = fopen(file, "rb");
    curl = curl_easy_init();

    // 设置HTTP头
    headers = curl_slist_append(headers, "Accept: ZOWELL");
    // char tmpStr[32];
    // snprintf(tmpStr, sizeof(tmpStr), "Content-Length:  %d", restfulData.readData.size);
    // headers = curl_slist_append(headers, tmpStr);
    if (auth != NULL)
    {
        char authInfo[128] = {0};
        snprintf(authInfo, sizeof(authInfo), "%s:%s", auth->username, auth->password);
        curl_easy_setopt(curl, CURLOPT_USERPWD, authInfo);
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_PUT, 1L);
    //curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadFunc);
    curl_easy_setopt(curl, CURLOPT_READDATA, &restfulData);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)restfulData.readData.size);
    // curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    // curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
    // curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
    //                  (curl_off_t)file_info.st_size);
    
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, GetHttpStatusFunc);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &restfulData);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &restfulData);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 3000L);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", 
                curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if ((CURLE_OK == res) && (200 == restfulData.header.status))
    {
        *out = restfulData.writeData.ptr;
        *outSize = restfulData.writeData.size;
    }
    // fclose(hd_src);
    HGH_DBG("Status[%d] ContentLength[%d]\n", restfulData.header.status, restfulData.header.contentLength);

    return 0;
}

int Restful_post(UserAuth* auth, const char* url, char* in, int inSize, char** out, int *outSize)
{
    CURL* curl = NULL;
    struct curl_slist *headers = NULL;
    CURLcode res;
    CurlRestfulData restfulData;

    memset(&restfulData, 0, sizeof(restfulData));
    restfulData.readData.ptr = in;
    restfulData.readData.size = inSize;
    restfulData.readData.pos = 0;
    

    curl = curl_easy_init();

    // 设置HTTP头
    headers = curl_slist_append(headers, "Accept: ZOWELL");
    if (auth != NULL)
    {
        char authInfo[128] = {0};
        snprintf(authInfo, sizeof(authInfo), "%s:%s", auth->username, auth->password);
        curl_easy_setopt(curl, CURLOPT_USERPWD, authInfo);
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadFunc);
    curl_easy_setopt(curl, CURLOPT_READDATA, &restfulData);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)restfulData.readData.size);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, GetHttpStatusFunc);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &restfulData);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &restfulData);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 3000L);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", 
                curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if ((CURLE_OK == res) && (200 == restfulData.header.status))
    {
        *out = restfulData.writeData.ptr;
        *outSize = restfulData.writeData.size;
    }
    HGH_DBG("Status[%d] ContentLength[%d]\n", restfulData.header.status, restfulData.header.contentLength);

    return 0;
}

int Restful_delete(UserAuth* auth, const char* url, char* in, int inSize, char** out, int *outSize)
{
    CURL* curl = NULL;
    struct curl_slist *headers = NULL;
    CURLcode res;
    CurlRestfulData restfulData;

    memset(&restfulData, 0, sizeof(restfulData));
    restfulData.readData.ptr = in;
    restfulData.readData.size = inSize;
    restfulData.readData.pos = 0;
    

    curl = curl_easy_init();

    // 设置HTTP头
    headers = curl_slist_append(headers, "Accept: ZOWELL");
    if (auth != NULL)
    {
        char authInfo[128] = {0};
        snprintf(authInfo, sizeof(authInfo), "%s:%s", auth->username, auth->password);
        curl_easy_setopt(curl, CURLOPT_USERPWD, authInfo);
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadFunc);
    curl_easy_setopt(curl, CURLOPT_READDATA, &restfulData);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)restfulData.readData.size);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, GetHttpStatusFunc);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &restfulData);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &restfulData);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 3000L);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", 
                curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if ((CURLE_OK == res) && (200 == restfulData.header.status))
    {
        *out = restfulData.writeData.ptr;
        *outSize = restfulData.writeData.size;
    }
    HGH_DBG("Status[%d] ContentLength[%d]\n", restfulData.header.status, restfulData.header.contentLength);

    return 0;
}

int main(int argc, char const *argv[])
{
    curl_global_init(CURL_GLOBAL_ALL);

    HGH_DBG("\n");
    UserAuth auth;
    snprintf(auth.username, sizeof(auth.username), "admin");
    snprintf(auth.password, sizeof(auth.password), "");
    char request[1024];
    char *response = NULL;
    int lens = 0;
    snprintf(request, sizeof(request), "<?xml version=\"1.0\" encoding=\"UTF-8\" ?> \
<StreamConfig Version=\"1.0\"> \
    <ID>2</ID> \
    <StreamType>VideoAndAudio</StreamType> \
    <VideoCodecType>H.265</VideoCodecType> \
    <ResolutionWidth>704</ResolutionWidth> \
    <ResolutionHeigth>576</ResolutionHeigth> \
    <FrameRate>1500</FrameRate> \
    <BitrateType>VBR</BitrateType> \
    <Bitrate>512</Bitrate> \
    <FixedQuality>2</FixedQuality> \
    <keyFrameInterval>200</keyFrameInterval> \
    <Transport Version=\"1.0\"> \
        <RTSPURI>rtsp://192.168.6.98:554/live/0/SUB</RTSPURI> \
    </Transport> \
</StreamConfig>");
    //HTTPRestful(HTTP_GET, &auth, "http://192.168.6.98/Streams/1/CapabilityV2", request, sizeof(request), &response, &lens);
    //HTTPRestful(HTTP_GET, &auth, "http://localhost:8600/Net/NetworkInterface", request, strlen(request), &response, &lens);
    //HTTPRestful(HTTP_GET, &auth, "http://192.168.3.125:3000/network", request, strlen(request), &response, &lens);
    //HTTPRestful(HTTP_PUT, &auth, "http://localhost:8700/Net/NetworkInterface", request, strlen(request), &response, &lens);
    //HTTPRestful(HTTP_POST, &auth, "http://localhost:8700/Net/NetworkInterface", request, strlen(request), &response, &lens);
    HTTPRestful(HTTP_DELETE, &auth, "http://localhost:8700/Net/NetworkInterface", request, strlen(request), &response, &lens);
    if (response)
    {
        HGH_DBG("response[%p]\n", response);
        HGH_DBG("response[%s] lens[%d]\n", response, lens);
        free(response);
    }
    
    curl_global_cleanup();

    return 0;
}
