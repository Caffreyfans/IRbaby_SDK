/*
 * @Author: Caffreyfans
 * @Date: 2021-06-06 15:46:30
 * @LastEditTime: 2021-07-15 23:56:05
 * @Description:
 */
#include "irext_api.h"

#include <malloc.h>
#include <string.h>

#include "http_client.h"
#include "log.h"
#include "port/storage.h"
#define URL_MAX_LEN 128

#define IREXT_URL_PREFIX "http://irext.net/irext-server"
#define IREXT_URL_PREFIX_SIZE (sizeof(IREXT_URL_PREFIX) - 1U)

IRBABY_STATUS irext_download(const char *filename) {
  IRBABY_STATUS status = IRBABY_FAIL;
  char url[URL_MAX_LEN] = {0};
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
  HTTPClient client = {.url = url, .method = HTTP_METHOD_GET};
  IRBABY_LOG("url = %s\n", url);
  http_client_init(&client);
  if (http_client_send(&client)) {
    IRBABY_LOG("try to save file\n");
    if (irbaby_write(filename, client.response.pBody,
                     client.response.bodyLen)) {
      status = IRBABY_OK;
    }
  }
  http_client_close(&client);
  return status;
}

cJSON *irext_login(const char *app_key, const char *app_secret) {
  char *suffix = "/app/app_login";
  char url[256] = {0};
  strncpy(url, IREXT_URL_PREFIX, sizeof(url));
  strncpy(url + IREXT_URL_PREFIX_SIZE, suffix,
          sizeof(url) - IREXT_URL_PREFIX_SIZE);
  cJSON *root = cJSON_CreateObject();
  cJSON_AddStringToObject(root, "appKey", app_key);
  cJSON_AddStringToObject(root, "appSecret", app_secret);
  cJSON_AddStringToObject(root, "appType", "2");
  char *root_decode = cJSON_PrintUnformatted(root);
  HTTPClient client = {.url = url,
                       .method = HTTP_METHOD_POST,
                       .payload = root_decode,
                       .payload_len = strlen(root_decode)};
  http_client_init(&client);
  char *type_filed = "Content-Type";
  char *type_value = "application/json;charset=utf-8";
  HTTPClient_AddHeader(&client.header, type_filed, strlen(type_filed),
                       type_value, strlen(type_value));

  char *length_filed = "Content-Length";
  int len = strlen(root_decode);
  char buffer[10] = {0};
  snprintf(buffer, sizeof(buffer), "%d", len);
  HTTPClient_AddHeader(&client.header, length_filed, strlen(length_filed),
                       buffer, strlen(buffer));

  printf("header = \n%s\n", (char *)client.header.pBuffer);
  if (http_client_send(&client)) {
    printf("Response: %s\n", client.response.pBody);
  }
  printf("Header: \n%s\n", client.response.pBuffer);
  free(root_decode);
  cJSON_Delete(root);
  return NULL;
}