/*
 * @Author: Caffreyfans
 * @Date: 2021-06-06 15:46:30
 * @LastEditTime: 2021-06-07 21:13:29
 * @Description: 
 */
#include "download.h"
#include "http_client.h"
#include "port/storage.h"
#include <string.h>
#define URL_MAX_LEN     128
IRBABY_STATUS irbaby_download(const char *filename)
{
    char url[URL_MAX_LEN];
    int offset = 0;
    strcpy(url + offset, HTTP_PREFIX);
    offset += HTTP_PREFIX_LEN;
    strcpy(url + offset, filename);
    offset += strlen(filename);
    strcpy(url + offset, HTTP_SUFFIX);
    offset += HTTP_SUFFIX_LEN;
    HTTPClient client = {
        .url = url,
        .method = HTTP_METHOD_GET
    };
    http_client_init(&client);
    if (http_client_send(&client)) {
        irbaby_write(filename, client.response.pBody, client.reponse.bodyLen);
    }
    http_client_close(&client);
}