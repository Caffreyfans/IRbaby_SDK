/*
 * @Author: Caffreyfans
 * @Date: 2021-06-04 22:22:02
 * @LastEditTime: 2021-06-04 23:58:10
 * @Description: 
 */
#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

#ifndef IRBABY_LOG_ON
#define IRBABY_LOG_ON  1
#endif // IRBABY_LOG_ON

#define IRBABY_LOG(fmt, ...) do { \
    if (IRBABY_LOG_ON)  \
        printf(fmt, ##__VA_ARGS__); \
    } while (0)

#endif // LOG_H_