/*
 * @Author: Caffreyfans
 * @Date: 2021-06-16 22:31:40
 * @LastEditTime: 2021-07-15 21:48:48
 * @Description:
 */
#include "peripherals.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_rx.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define IR_SEND_BUFFER_SIZE 512

#define TAG "IRbaby Peripherals"
#define EXAMPLE_IR_RESOLUTION_HZ 1000000
#define NEC_PAYLOAD_DURATION_0 560
#define NEC_PAYLOAD_DURATION_1 1690
#define IR_NEC_DECODE_MARGIN 200 // Tolerance for parsing RMT symbols into bit stream

rmt_encoder_handle_t encoder = NULL;
rmt_channel_handle_t tx_channel = NULL;
rmt_channel_handle_t rx_channel = NULL;
QueueHandle_t receive_queue;
static bool rmt_rx_done_callback(rmt_channel_handle_t channel, const rmt_rx_done_event_data_t *edata, void *user_data)
{
  BaseType_t high_task_wakeup = pdFALSE;
  QueueHandle_t receive_queue = (QueueHandle_t)user_data;
  // send the received RMT symbols to the parser task
  xQueueSendFromISR(receive_queue, edata, &high_task_wakeup);
  return high_task_wakeup == pdTRUE;
}

void ir_init(int tx_pin, int rx_pin)
{
  if (tx_channel != NULL)
  {
    rmt_disable(tx_channel);
    rmt_del_channel(tx_channel);
  }
  if (encoder != NULL)
  {
    rmt_del_encoder(encoder);
  }
  if (rx_channel != NULL)
  {
    rmt_disable(rx_channel);
    rmt_del_channel(rx_channel);
  }
  ESP_LOGI(TAG, "create RMT TX channel pin %d", tx_pin);
  rmt_tx_channel_config_t tx_channel_cfg = {
      .clk_src = RMT_CLK_SRC_DEFAULT,
      .resolution_hz = EXAMPLE_IR_RESOLUTION_HZ,
      .mem_block_symbols = 64, // amount of RMT symbols that the channel can store at a time
      .trans_queue_depth = 1,  // number of transactions that allowed to pending in the background, this example won't queue multiple transactions, so queue depth > 1 is sufficient
      .gpio_num = tx_pin,
      .flags.io_od_mode = 0,
      .flags.with_dma = false,
  };
  ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_channel_cfg, &tx_channel));

  ESP_LOGI(TAG, "modulate carrier to TX channel");
  rmt_carrier_config_t carrier_cfg = {
      .duty_cycle = 0.33,
      .frequency_hz = 38000, // 38KHz
  };
  ESP_ERROR_CHECK(rmt_apply_carrier(tx_channel, &carrier_cfg));

  ESP_ERROR_CHECK(rmt_enable(tx_channel));

  ESP_LOGI(TAG, "install IR encoder");
  rmt_copy_encoder_config_t config = {};
  rmt_new_copy_encoder(&config, &encoder);

  // ESP_LOGI(TAG, "create RMT RX channel");
  // rmt_rx_channel_config_t rx_channel_cfg = {
  //     .clk_src = RMT_CLK_SRC_DEFAULT,
  //     .resolution_hz = EXAMPLE_IR_RESOLUTION_HZ,
  //     .mem_block_symbols = 64, // amount of RMT symbols that the channel can store at a time
  //     .gpio_num = 19,
  // };
  // rmt_channel_handle_t rx_channel = NULL;
  // ESP_ERROR_CHECK(rmt_new_rx_channel(&rx_channel_cfg, &rx_channel));

  // ESP_LOGI(TAG, "register RX done callback");
  // receive_queue = xQueueCreate(1, sizeof(rmt_rx_done_event_data_t));
  // assert(receive_queue);
  // rmt_rx_event_callbacks_t cbs = {
  //     .on_recv_done = rmt_rx_done_callback,
  // };
  // ESP_ERROR_CHECK(rmt_rx_register_event_callbacks(rx_channel, &cbs, receive_queue));
  // ESP_ERROR_CHECK(rmt_enable(rx_channel));
}

rmt_symbol_word_t items[512];
// this example won't send NEC frames in a loop
rmt_transmit_config_t transmit_config = {
    .loop_count = 0, // no loop
};
void ir_send(uint16_t *data, int len)
{
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
  ESP_LOGI(TAG, "rmt start i * sizeof(rmt_symbol_word_t) = %d", i * sizeof(rmt_symbol_word_t));
  rmt_transmit(tx_channel, encoder, items, i * sizeof(rmt_symbol_word_t), &transmit_config);
  rmt_tx_wait_all_done(tx_channel, portMAX_DELAY);
  ESP_LOGI(TAG, "rmt end");
}

void ir_recv(uint16_t *data, int len)
{
}

int ir_receive()
{
  // the following timing requirement is based on NEC protocol
  rmt_receive_config_t receive_config = {
      .signal_range_min_ns = 1250,     // the shortest duration for NEC signal is 560us, 1250ns < 560us, valid signal won't be treated as noise
      .signal_range_max_ns = 12000000, // the longest duration for NEC signal is 9000us, 12000000ns > 9000us, the receive won't stop early
  };
  // save the received RMT symbols
  rmt_symbol_word_t raw_symbols[64]; // 64 symbols should be sufficient for a standard NEC frame
  rmt_rx_done_event_data_t rx_data;
  while (1)
  {
    ESP_ERROR_CHECK(rmt_receive(rx_channel, raw_symbols, sizeof(raw_symbols), &receive_config));

    if (xQueueReceive(receive_queue, &rx_data, pdMS_TO_TICKS(1000)) == pdPASS)
    {
      ESP_LOGI(TAG, "Receive IR:");
      for (int i = 0; i < rx_data.num_symbols; i++)
      {
        printf("%d %d ", rx_data.received_symbols[i].duration0, rx_data.received_symbols[i].duration1);
      }
      printf("\r\n");
      ESP_ERROR_CHECK(rmt_receive(rx_channel, raw_symbols, sizeof(raw_symbols), &receive_config));
    }
  }
}
