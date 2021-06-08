/*
 * @Author: Caffreyfans
 * @Date: 2021-06-06 15:15:05
 * @LastEditTime: 2021-06-07 20:46:41
 * @Description: 
 */
#include "irbaby.h"
#include "port/storage.h"
#include "download.h"
#include <stdlib.h>
#include "log.h"

#define IR_RX_BUFFER_LEN 512
#define IR_TX_BUFFER_LEN 512

IRBABY_STATUS irbaby_send(const char *filename)
{
    IRBABY_STATUS status = IRBABY_FAIL;
    uint8_t *tx_buffer = (uint8_t *)malloc(IR_TX_BUFFER_LEN);
    if (tx_buffer == NULL)
        goto exit; 
    int read_len = irbaby_read(filename, tx_buffer, IR_TX_BUFFER_LEN);
    if (read_len == 0)
    {
        if (irbaby_download(filename) != IRBABY_OK)
        {
            goto exit;
        }

        status = IRBABY_OK;
        IRBABY_LOG("IR send success\n");
    }
    return IRBABY_OK;
exit:
    if (tx_buffer != NULL)
        free(tx_buffer);
    return status;
}