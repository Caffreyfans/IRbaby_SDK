/*
 * @Author: Caffreyfans
 * @Date: 2021-06-06 12:43:13
 * @LastEditTime: 2021-06-06 15:20:57
 * @Description: storage porting layer
 */
#ifndef STORAGE_H_
#define STORAGE_H_
#include "status_code.h"
#include <stdio.h>

size_t irbaby_read(const char *filename, uint8_t *buffer, int len);

size_t irbaby_write(const char *filename, uint8_t *buffer, int len);
#endif
