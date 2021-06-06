/*
 * @Author: Caffreyfans
 * @Date: 2021-06-06 15:16:51
 * @LastEditTime: 2021-06-06 15:40:35
 * @Description: 
 */
#include "storage.h"
#include <stdio.h>
#include "log.h"
size_t irbaby_read(const char *filename, uint8_t *buffer, size_t len)
{
    size_t error = 0;
    if (filename == NULL || buffer == NULL)
        return error;
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        printf("open read file fails\n");
        return error;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (buffer_len < size) {
        LOG("buffer size to small\n");
        return error;
    }
    fread(buffer, sizeof(uint8_t), size, f);
    fclose(f);
    return size;
}

size_t irbaby_write(const char *filename, uint8_t *buffer, size_t len)
{
    size_t error = 0;
    if (filename == NULL || buffer == NULL)
        return error;
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        LOG("open write file fails\n");
        return error;
    }
    fwrite(buffer, sizeof(uint8_t), len, f);
    fclose(f);
    return len;
}
