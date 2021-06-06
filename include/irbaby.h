/*
 * @Author: Caffreyfans
 * @Date: 2021-06-06 15:11:32
 * @LastEditTime: 2021-06-06 15:11:32
 * @Description: 
 */
#ifndef IRBABY_H_
#define IRBABY_H_
#include "status_code.h"

IRBABY_STATUS irbaby_send(const char *filename);

IRBABY_STATUS irbaby_record(const char *filename);
#endif // IRBABY_H_