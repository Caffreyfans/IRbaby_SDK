/*
 * @Author: Caffreyfans
 * @Date: 2021-06-04 23:29:17
 * @LastEditTime: 2021-08-09 22:57:45
 * @Description: http request test
 */
#include "irext_api.h"

#include <malloc.h>
#include <stdio.h>

#include "http_client.h"
int main(int argc, char *argv[]) {
  cJSON *token_obj = irext_login("cdf33048c9dbef2962b0f915bc7e420c",
                                 "f00f57af376c66ca1355cfe109400dd2", "2");
  char *token_str = cJSON_GetObjectItem(token_obj, "token")->valuestring;
  int id_num = cJSON_GetObjectItem(token_obj, "id")->valueint;
  cJSON *categories_obj = irext_list_categories(id_num, token_str);
  // cJSON *brands_obj = irext_list_brands(1, id_num, token_str);
  irext_list_indexes(1, 1, id_num, token_str);
  cJSON_Delete(categories_obj);
  cJSON_Delete(token_obj);
  token_obj = NULL;
  return 0;
}
