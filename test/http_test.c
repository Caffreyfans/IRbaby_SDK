/*
 * @Author: Caffreyfans
 * @Date: 2021-06-04 23:29:17
 * @LastEditTime: 2021-06-06 16:48:11
 * @Description: http request test
 */
#include "http_client.h"
#include <stdio.h>
#include "irbaby.h"
int main(int argc, char *argv[])
{
    HTTPClient client = {
        .url = "http://irext-debug.oss-cn-hangzhou.aliyuncs.com/irda_new_ac_2582.bin",
        .method = HTTP_METHOD_GET};
    http_client_init(&client);
    if (http_client_send(&client) == IRBABY_OK)
    {
        for (int i = 0; i < client.response.bufferLen; i++)
        {
            printf("%c", client.response.pBuffer[i]);
        }
        printf("\n");
    }
    printf("\n");
    http_client_close(&client);

    irbaby_send("new_ac_2582");
}
