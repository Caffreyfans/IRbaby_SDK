/*
 * @Author: Caffreyfans
 * @Date: 2021-06-06 15:46:25
 * @LastEditTime: 2021-07-15 21:47:41
 * @Description: 
 */
#ifndef DOWNLOAD_H_
#define DOWNLOAD_H_
#include "http_client.h"
#include "status_code.h"
#include "cJSON.h"

#define HTTP_PREFIX "http://irext-debug.oss-cn-hangzhou.aliyuncs.com/irda_"
#define HTTP_PREFIX_LEN (sizeof(HTTP_PREFIX) - 1U) 
#define HTTP_SUFFIX ".bin"
#define HTTP_SUFFIX_LEN (sizeof(HTTP_SUFFIX) - 1U)

IRBABY_STATUS irext_download(const char *filename);

cJSON *irext_login(const char *app_key, const char *app_secret);
// cJSON *irext_list_brands();
#endif