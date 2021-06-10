/*
 * @Author: Caffreyfans
 * @Date: 2021-06-06 15:46:30
 * @LastEditTime: 2021-06-10 23:41:22
 * @Description: 
 */
#include "download.h"
#include "http_client.h"
#include "port/storage.h"
#include <string.h>
#include "log.h"
#define URL_MAX_LEN     128
IRBABY_STATUS irbaby_download(const char *filename)
{
    IRBABY_STATUS status = IRBABY_FAIL;
    char url[URL_MAX_LEN] = { 0 };
    IRBABY_LOG("url = %s\n", url);
    int offset = 0;
    strcpy(url + offset, HTTP_PREFIX);
    IRBABY_LOG("HTTP_PREFIX = %s\n", HTTP_PREFIX);
    IRBABY_LOG("url = %s\n", url);
    offset += HTTP_PREFIX_LEN;
    strcpy(url + offset, filename);
    IRBABY_LOG("url = %s\n", url);
    offset += strlen(filename);
    strcpy(url + offset, HTTP_SUFFIX);
    IRBABY_LOG("url = %s\n", url);
    offset += HTTP_SUFFIX_LEN;
    HTTPClient client = {
        .url = url,
        .method = HTTP_METHOD_GET
    };
    IRBABY_LOG("url = %s\n", url);
    http_client_init(&client);
    if (http_client_send(&client)) {
        IRBABY_LOG("try to save file\n");
        if (irbaby_write(filename, client.response.pBody, client.response.bodyLen)) {
           status = IRBABY_OK; 
        }
    }
    http_client_close(&client);
    return status;
}