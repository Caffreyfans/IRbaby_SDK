/*
 * @Author: Caffreyfans
 * @Date: 2021-06-06 12:43:13
 * @LastEditTime: 2021-06-15 23:37:49
 * @Description: storage porting layer
 */
#ifndef STORAGE_H_
#define STORAGE_H_
#include "status_code.h"
#include <stdio.h>
#include "types.h"
size_t irbaby_read(const char *filename, uint8_t *buffer, size_t len);

size_t irbaby_write(const char *filename, const uint8_t *buffer, size_t len);
#endif
