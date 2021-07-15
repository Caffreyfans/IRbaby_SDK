/*
 * @Author: Caffreyfans
 * @Date: 2021-06-06 15:11:32
 * @LastEditTime: 2021-07-15 21:14:56
 * @Description: 
 */
#ifndef IRBABY_H_
#define IRBABY_H_
#include "status_code.h"
#include "ir_decode.h"
IRBABY_STATUS irbaby_send(const char *filename, t_remote_ac_status *status);

IRBABY_STATUS irbaby_record(const char *filename);
#endif // IRBABY_H_