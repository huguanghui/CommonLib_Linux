#include <stdio.h>
#include <curl/curl.h>

#define HGH_DBG(fmt, args...) printf("\033[40;33m HGH_DBG(%s %s %d):\t\033[0m"fmt, __FILE__, __func__, __LINE__, ## args)

struct FtpFile {
    const char *filename;
    FILE *stream;
};

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
    struct FtpFile *out = (struct FtpFile *)stream;
    if (NULL == out->stream) {
        out->stream = fopen(out->filename, "wb");
        if (NULL == out->stream)
            return -1;
    }
    return fwrite(buffer, size, nmemb, out->stream);
}

int main(int argc, char const *argv[])
{
    CURL *curl=NULL;
    CURLcode res;

    struct FtpFile ftpfile = {
        "pcre.tar.gz",
        NULL
    };

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl)
    {
        /**< 设置URL的地址 */
        curl_easy_setopt(curl, CURLOPT_URL, 
                        "ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/pcre2-10.32.tar.gz");
        /**< 设置写回调函数 */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
        /**< 设置写的用户数据 */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);
        /**< 开启全协议支持 */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (CURLE_OK != res)
        {
            fprintf(stderr, "curl result %d\n", res);
        }
    }
    
    if (ftpfile.stream)
    {
        fclose(ftpfile.stream);
    }

    curl_global_cleanup();

    return 0;
}
