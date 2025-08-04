/*
 * @Author: Caffreyfans
 * @Date: 2021-06-16 22:27:15
 * @LastEditTime: 2021-06-16 23:30:47
 * @Description: IR transport layer
 */
#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_
#include "types.h"
void ir_init(int tx_pin, int rx_pin);
/**
 * @description: IR signal send
 * @param {uint16_t} *data
 * @param {int} len
 * @return {*}
 */
void ir_send(uint16_t *data, int len);

/**
 * @description: IR signal recv 
 * @param {uint16_t} *data
 * @param {int} len
 * @return {*}
 */
void ir_recv(uint16_t *data, int len); 

int ir_receive();
#endif