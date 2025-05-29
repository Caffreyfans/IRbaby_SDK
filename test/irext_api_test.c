/*
 * @Author: Caffreyfans
 * @Date: 2021-06-04 23:29:17
 * @LastEditTime: 2021-08-17 21:21:45
 * @Description: http request test
 */
#include "irext_api.h"

#include <malloc.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  cJSON *token_obj = irext_login("00567968d9316665fe5180fa2b1a24aa",
                                  "6a111f71549eb485e616252f208017f4", "2");
  char *token_str = cJSON_GetObjectItem(token_obj, "token")->valuestring;
  int id_num = cJSON_GetObjectItem(token_obj, "id")->valueint;
  cJSON *categories_obj = irext_list_categories(id_num, token_str);
  cJSON *brands_obj = irext_list_brands(1, id_num, token_str);
  cJSON *indexes_obj = irext_list_indexes(1, 1, id_num, token_str);
  
  char *str = cJSON_PrintUnformatted(categories_obj);
  printf("%s\n", str);
  free(str);
  str = cJSON_PrintUnformatted(brands_obj);
  printf("%s\n", str);
  free(str);
  str = cJSON_PrintUnformatted(indexes_obj);
  printf("%s\n", str);
  free(str);

  cJSON_Delete(categories_obj);
  cJSON_Delete(token_obj);
  cJSON_Delete(indexes_obj);
  return 0;
}
