/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2018, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/ 
/* <DESC>
 * Download a given URL into a local file named page.out.
 * </DESC>
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
#include <curl/curl.h>
 
int main(int argc, char *argv[])
{
    curl_global_init(CURL_GLOBAL_ALL);

    CURL *hnd = curl_easy_init();

    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(hnd, CURLOPT_URL, "http://192.168.6.99/FaceGroup/AddPersonInfoAndFaceImage");

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "cache-control: no-cache");
    headers = curl_slist_append(headers, "Connection: keep-alive");
    headers = curl_slist_append(headers, "content-length: 18708");
    headers = curl_slist_append(headers, "content-type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW");
    headers = curl_slist_append(headers, "accept-encoding: gzip, deflate");
    headers = curl_slist_append(headers, "Host: 192.168.6.99");
    headers = curl_slist_append(headers, "Postman-Token: dcf4ffaa-721d-4991-a641-da081a9929eb,30b2c6e9-c6df-4065-9f5f-3cf9cb3a9f9f");
    headers = curl_slist_append(headers, "Cache-Control: no-cache");
    headers = curl_slist_append(headers, "Accept: */*");
    headers = curl_slist_append(headers, "User-Agent: PostmanRuntime/7.15.0");
    headers = curl_slist_append(headers, "Authorization: Basic YWRtaW46");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
            "Content-Disposition: form-data; name=\"IgnoreError\"\r\n\r\nfalse\r\n"
            "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
            "Content-Disposition: form-data; name=\"Name\"\r\n\r\n王金岩\r\n"
            "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
            "Content-Disposition: form-data; name=\"Sex\"\r\n\r\nmale\r\n"
            "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
            "Content-Disposition: form-data; name=\"CertificateType\"\r\n\r\nIC\r\n"
            "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
            "Content-Disposition: form-data; name=\"CertificateID\"\r\n\r\n420041198101013347\r\n"
            "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
            "Content-Disposition: form-data; name=\"GroupID\"\r\n\r\n3\r\n"
            "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
            "Content-Disposition: form-data; name=\"Country\"\r\n\r\n中国\r\n"
            "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
            "Content-Disposition: form-data; name=\"Address\"\r\n\r\n湖北省#武汉\r\n"
            "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
            "Content-Disposition: form-data; name=\"Birthday\"\r\n\r\n1981-01-01\r\n"
            "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
            "Content-Disposition: form-data; name=\"file\"; filename=\"王金岩#S2#B1981-01-01#C中国#P湖北省#武汉市#T2#M420041198101013347.jpg\"\r\n"
            "Content-Type: image/jpeg\r\n\r\n\r\n"
            "------WebKitFormBoundary7MA4YWxkTrZu0gW--");

    CURLcode ret = curl_easy_perform(hnd);

    curl_global_cleanup();
fa
    return 0;
}
