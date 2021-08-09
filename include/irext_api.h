/*
 * @Author: Caffreyfans
 * @Date: 2021-06-06 15:46:25
 * @LastEditTime: 2021-08-09 22:39:49
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

cJSON *irext_login(const char *app_key, const char *app_secret, const char *app_type);

cJSON *irext_list_categories(const int id, const char *token);

cJSON *irext_list_brands(const int category_id, const int id, const char *token);

cJSON *irext_list_indexes(const int category_id, const int brand_id, const int id, const char *token);
#endif