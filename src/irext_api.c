/*
 * @Author: Caffreyfans
 * @Date: 2021-06-06 15:46:30
 * @LastEditTime: 2021-07-14 22:15:44
 * @Description:
 */
#include "irext_api.h"

#include <string.h>

#include "http_client.h"
#include "log.h"
#include "port/storage.h"
#define URL_MAX_LEN 128

static const char *irext_url_prefix = "http://irext.net/irext-server";
static const int irext_url_prefix_size = sizeof(irext_url_prefix);

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
  char url[128];
  char *str = strncpy(url, irext_url_prefix, sizeof(url));
  strncpy(str, suffix, sizeof(url) - irext_url_prefix_size);
}