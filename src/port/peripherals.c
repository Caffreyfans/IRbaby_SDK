/*
 * @Author: Caffreyfans
 * @Date: 2021-06-16 22:31:40
 * @LastEditTime: 2021-07-15 21:48:48
 * @Description:
 */
#include "peripherals.h"
#if ESP32 || ESP8266
#include "driver/rmt.h"
#include "esp_log.h"
#include "esp_timer.h"
#endif
#define IR_SEND_BUFFER_SIZE 512

#define TAG "IRbaby Peripherals"

// static void rmt_tx_end_cb(rmt_channel_t channel, void *arg)
// {
//   rmt_remove_channel_from_group(channel);
//   rmt_driver_uninstall(channel);
// }

void ir_send(uint16_t *data, int len, int pin)
{
#if ESP32 || ESP8266
  static rmt_item32_t items[IR_SEND_BUFFER_SIZE];
  rmt_config_t rmtConfig = RMT_DEFAULT_CONFIG_TX(pin, RMT_CHANNEL_0);
  rmtConfig.rmt_mode = RMT_MODE_TX; // transmit mode
  rmtConfig.channel = RMT_CHANNEL_0;  // channel to use 0 - 7
  rmtConfig.clk_div = 80;           // clock divider 1 - 255. source clock is 80MHz -> 80MHz/80 = 1MHz -> 1 tick = 1 us
  rmtConfig.gpio_num = pin;      // pin to use
  rmtConfig.mem_block_num = 1;      // memory block size

  rmtConfig.tx_config.loop_en = 0;                            // no loop
  rmtConfig.tx_config.carrier_freq_hz = 38000;                // 36kHz carrier frequency
  rmtConfig.tx_config.carrier_duty_percent = 33;              // duty
  rmtConfig.tx_config.carrier_level = RMT_CARRIER_LEVEL_HIGH; // carrier level
  rmtConfig.tx_config.carrier_en = 1;         // carrier enable
  rmtConfig.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;        // signal level at idle
  rmtConfig.tx_config.idle_output_en = true;
  rmt_config(&rmtConfig);
  rmt_driver_install(RMT_CHANNEL_0, 0, 0);
  int i = 0, j = 0;
  while (j < len)
  {
    items[i].level0 = 1;
    items[i].duration0 = data[j];
    items[i].level1 = 0;
    items[i].duration1 = data[j + 1];
    i++;
    j += 2;
  }
  int items_len = len / 2;
  rmt_write_items(RMT_CHANNEL_0, items, items_len, true);
  rmt_driver_uninstall(RMT_CHANNEL_0);
#endif
}

void ir_recv(uint16_t *data, int len, int pin)
{
#if defined(ESP32)

#else
#endif // ESP32
}
