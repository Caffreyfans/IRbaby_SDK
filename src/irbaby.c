/*
 * @Author: Caffreyfans
 * @Date: 2021-06-06 15:15:05
 * @LastEditTime: 2021-07-28 21:44:15
 * @Description:
 */
#include "irbaby.h"
#include "port/storage.h"
#include "port/peripherals.h"
#include <stdlib.h>
#include "irext_api.h"
#include "log.h"

#define IR_RX_BUFFER_LEN 1024
#define IR_TX_BUFFER_LEN 1024
#define data_buffer_BUFFER_LEN 1024

IRBABY_STATUS irbaby_send(const char *filename, t_remote_ac_status *ac_status, int pin)
{
    IRBABY_STATUS status = IRBABY_FAIL;
    uint16_t *tx_buffer = NULL;
    uint8_t *data_buffer = NULL;
    int ret = (((data_buffer = (uint8_t *)calloc(1, IR_TX_BUFFER_LEN)) != NULL) &&
               ((tx_buffer = (uint16_t *)calloc(2, data_buffer_BUFFER_LEN)) != NULL));
    if (!ret)
    {
        IRBABY_LOG("calloc failed\n");
        goto exit;
    }
    int data_len = irbaby_read(filename, data_buffer, IR_TX_BUFFER_LEN);
    if (data_len == 0)
    {
        if (irext_download(filename) != IRBABY_OK)
        {
            goto exit;
        }
        IRBABY_LOG("download IR library\n");
        data_len = irbaby_read(filename, data_buffer, IR_TX_BUFFER_LEN);
    }
    if (ac_status != NULL)
    {
        uint8_t ret = ir_binary_open(REMOTE_CATEGORY_AC, 1, data_buffer, data_len);
        uint16_t tx_len = ir_decode(0, tx_buffer, ac_status, 0);
        IRBABY_LOG("decode %s data len = %d\n", ret == IR_DECODE_SUCCEEDED ? "SUCCESS" : "FAILED", tx_len);
        for (int i = 0; i < tx_len; i++)
        {
            IRBABY_LOG("%d ", tx_buffer[i]);
        }
        IRBABY_LOG("\n");
        ir_send(tx_buffer, tx_len);
    }
    status = IRBABY_OK;
exit:
    if (tx_buffer != NULL)
        free(tx_buffer);
    if (data_buffer != NULL)
        free(data_buffer);
    return status;
}