#include "user_config.h"
#include "user_event.h"
#include "user_gpio.h"
#include "user_player.h"
#include "user_pwm.h"
#include "user_timer.h"
#include "user_uart.h"

#define TAG "auto_gpio"

#define UART_SEND_MAX      16

typedef enum {
  UART_CMD_NONE = -1,
  UART_CMD_CHECK_WEATHER = 0,
  UART_CMD_CONNECT_SERVER_OK,
  UART_CMD_WIFI_CONNECT_OK,
  UART_CMD_WIFI_CONNECT_NG,
  UART_CMD_BLE_CONNECT_OK,
  UART_CMD_BLE_CONNECT_NG,
  UART_CMD_CONNECT_FILAS,
  UART_CMD_CHECK_ERROR,
  UART_CMD_SEND_ERROR,
  UART_CMD_CONRTOL_ERROR,
  UART_CMD_WIFI_SCAN_DONE,
}uart_cmd_t;

typedef struct {
  char  data[UART_SEND_MAX];
  int   len;
}uart_data_t;

typedef struct {
  uart_data_t uart_data;
  char* pcm;
}user_data_t;

const uart_data_t g_uart_buf[] = {
  {{0x5A, 0x4A, 0x01, 0x00}, 4}, //wakeup_uni
  {{0x5A, 0x4A, 0x02, 0x01}, 4}, //openL
  {{0x5A, 0x4A, 0x02, 0x02}, 4}, //closeL
  {{0x5A, 0x4A, 0x02, 0x03}, 4}, //checkWaether
  {{0x5A, 0x4A, 0x02, 0x04}, 4}, //connectMQTT
  {{0x5A, 0x4A, 0x02, 0x05}, 4}, //connectWiFi
  {{0x5A, 0x4A, 0x02, 0x06}, 4}, //openAPP1
  {{0x5A, 0x4A, 0x02, 0x07}, 4}, //openAPP2
  {{0x5A, 0x4A, 0x02, 0x08}, 4}, //openAPP3
  {{0x5A, 0x4A, 0x02, 0x09}, 4}, //openAPP4
  {{0x5A, 0x4A, 0x02, 0x0A}, 4}, //openAPP5
  {{0x5A, 0x4A, 0x02, 0x0B}, 4}, //openAPP6
  {{0x5A, 0x4A, 0x02, 0x0C}, 4}, //wifiScan
};

user_data_t user_data[] = {
  {{{0x5B, 0x4B, 0x02, 0x03}, 4},"[111]"},
  {{{0x5B, 0x4B, 0x02, 0x04}, 4},"[113]"},
  {{{0x5B, 0x4B, 0x02, 0x05}, 4},"[114]"},
  {{{0x5B, 0x4B, 0x02, 0x06}, 4},"[116]"},
  {{{0x5B, 0x4B, 0x02, 0x07}, 4},"[117]"},
  {{{0x5B, 0x4B, 0x02, 0x08}, 4},"[118]"},
  {{{0x5B, 0x4B, 0x02, 0x09}, 4},"[119]"},
  {{{0x5B, 0x4B, 0x02, 0x0A}, 4},"[120]"},
  {{{0x5B, 0x4B, 0x02, 0x0B}, 4},"[121]"},
  {{{0x5B, 0x4B, 0x02, 0x0C}, 4},"[124]"},
};

static void _custom_setting_cb(USER_EVENT_TYPE event,
                               user_event_context_t* context) {
  event_custom_setting_t* setting = NULL;
  if (context) {
    setting = &context->custom_setting;
    LOGT(TAG, "user command: %s", setting->cmd);
    if (0 == uni_strcmp(setting->cmd, "openL")) {
      user_player_reply_list_in_order("[108]");
      // user_player_reply_list_random(setting->reply_files);
      user_uart_send(g_uart_buf[1].data, g_uart_buf[1].len);
    }
    else if (0 == uni_strcmp(setting->cmd, "closeL")) {
      user_player_reply_list_in_order("[109]");
      user_uart_send(g_uart_buf[2].data, g_uart_buf[2].len);
    }
    else if (0 == uni_strcmp(setting->cmd, "checkWaether")) {
      user_player_reply_list_in_order("[110]");
      user_uart_send(g_uart_buf[3].data, g_uart_buf[3].len);
    }
    else if (0 == uni_strcmp(setting->cmd, "connectMQTT")) {
      user_player_reply_list_in_order("[112]");
      user_uart_send(g_uart_buf[4].data, g_uart_buf[4].len);
    }
    else if (0 == uni_strcmp(setting->cmd, "connectWiFi")) {
      user_player_reply_list_in_order("[112]");
      user_uart_send(g_uart_buf[5].data, g_uart_buf[5].len);
    }
    else if (0 == uni_strcmp(setting->cmd, "openAPP1")) {
      user_player_reply_list_in_order("[115]");
      user_uart_send(g_uart_buf[6].data, g_uart_buf[6].len);
    }
    else if (0 == uni_strcmp(setting->cmd, "openAPP2")) {
      user_player_reply_list_in_order("[115]");
      user_uart_send(g_uart_buf[7].data, g_uart_buf[7].len);
    }
    else if (0 == uni_strcmp(setting->cmd, "openAPP3")) {
      user_player_reply_list_in_order("[115]");
      user_uart_send(g_uart_buf[8].data, g_uart_buf[8].len);
    }
    else if (0 == uni_strcmp(setting->cmd, "openAPP4")) {
      user_player_reply_list_in_order("[115]");
      user_uart_send(g_uart_buf[9].data, g_uart_buf[9].len);
    }
    else if (0 == uni_strcmp(setting->cmd, "openAPP5")) {
      user_player_reply_list_in_order("[115]");
      user_uart_send(g_uart_buf[10].data, g_uart_buf[10].len);
    }
    else if (0 == uni_strcmp(setting->cmd, "openAPP6")) {
      user_player_reply_list_in_order("[115]");
      user_uart_send(g_uart_buf[11].data, g_uart_buf[11].len);
    }
    else if (0 == uni_strcmp(setting->cmd, "wifiScan")) {
      user_player_reply_list_in_order("[123]");
      user_uart_send(g_uart_buf[12].data, g_uart_buf[12].len);
    }
    else {
      LOGT(TAG, "Unconcerned command: %s", setting->cmd);
    }

  }
}


static void _goto_awakened_cb(USER_EVENT_TYPE event,
                               user_event_context_t* context) {
  event_goto_awakend_t* awkened = NULL;
  if (context) {
    awkened = &context->goto_awakend;
    user_uart_send(g_uart_buf[0].data, g_uart_buf[0].len);
    user_player_reply_list_random(awkened->reply_files);
  }
}

static void _register_event_callback(void) {
  user_event_subscribe_event(USER_CUSTOM_SETTING, _custom_setting_cb);
  user_event_subscribe_event(USER_GOTO_AWAKENED, _goto_awakened_cb);
}


static void _uart_recv_cb(char* buf, int len)
{
  int i = 0, j = 0;
  int num_cnt = 0;
  while (1) {
    for (i = 0; i<len;i++) {
      if (buf[i]==user_data[j].uart_data.data[i]) {
        num_cnt++;
      }
      else break;
    }

    if (num_cnt==user_data[j].uart_data.len) {
      break;
    }
    j++;
    num_cnt = 0;
  }
  user_player_reply_list_in_order(user_data[j].pcm);

}

int hb_auto_gpio(void) {
  user_gpio_init();
  user_gpio_set_mode(GPIO_NUM_A25, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_A25, 0);
  user_gpio_set_mode(GPIO_NUM_A26, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_A26, 0);
  user_gpio_set_mode(GPIO_NUM_A27, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_A27, 0);
  user_gpio_set_mode(GPIO_NUM_A28, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_A28, 0);
  user_gpio_set_mode(GPIO_NUM_B2, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_B2, 0);
  user_gpio_set_mode(GPIO_NUM_B3, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_B3, 0);
  user_uart_init(_uart_recv_cb);
  _register_event_callback();
  return 0;
}

