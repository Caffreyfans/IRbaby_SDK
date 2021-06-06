/*
 * @Author: Caffreyfans
 * @Date: 2021-06-06 15:15:05
 * @LastEditTime: 2021-06-06 15:41:07
 * @Description: 
 */
#include "irbaby.h"
#include "port/storage.h"

#define IR_RX_BUFFER_LEN 512
#define IR_TX_BUFFER_LEN 512

IRBABY_STATUS irbaby_send(const char *filename)
{
    uint8_t *tx_buffer = (uint8_t *)malloc(IR_TX_BUFFER_LEN);
    if (buffer == NULL)
        return FAIL;
    int read_len = irbaby_read(filename, tx_buffer, IR_TX_BUFFER_LEN);
    if (read_len == 0) {
        
    }
}