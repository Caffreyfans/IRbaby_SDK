/*
 * @Author: Caffreyfans
 * @Date: 2021-06-16 22:31:40
 * @LastEditTime: 2021-07-15 21:48:48
 * @Description:
 */
#include "peripherals.h"
#if ESP32 || ESP8266
#include "driver/rmt.h"
#endif
#define IR_SEND_BUFFER_SIZE 512

void ir_send(uint16_t *data, int len, int pin) {
#if ESP32 || ESP8266
  rmt_item32_t items[IR_SEND_BUFFER_SIZE];
  rmt_config_t config = RMT_DEFAULT_CONFIG_TX(pin, RMT_CHANNEL_0);
  config.tx_config.loop_en = false;
  config.tx_config.carrier_duty_percent = 50;
  config.tx_config.carrier_freq_hz = 38000;
  rmt_config(&config);
  rmt_driver_install(RMT_CHANNEL_0, 0, 0);
  int i = 0, j = 0;
  while (j < len) {
    items[i].level0 = 1;
    items[i].duration0 = data[j];
    items[i].level1 = 0;
    items[i].duration1 = data[j + 1];
    i++;
    j += 2;
  }
  int items_len = len / 2;
  rmt_write_items(RMT_CHANNEL_0, items, items_len, false);
  rmt_driver_uninstall(RMT_CHANNEL_0);
#endif
}

void ir_recv(uint16_t *data, int len, int pin) {
#if defined(ESP32)

#else
#endif  // ESP32
}
