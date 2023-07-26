/**
 * @file voice_uart.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-07-24
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef VOICE_UART_H
#define VOICE_UART_H

#define UART_SEND_MAX      16

typedef enum {
    UART_CMD_NONE = -1,
    UART_CMD_CHECK_WEATHER = 0,
    UART_CMD_CONNECT_SERVER_OK,
    UART_CMD_WIFI_CONNECT_OK,
    UART_CMD_WIFI_CONNECT_NG,
    UART_CMD_BLE_CONNECT_NG,
    UART_CMD_CONNECT_FILAS,
    UART_CMD_CHECK_ERROR,
    UART_CMD_SEND_ERROR,
    UART_CMD_CONRTOL_ERROR,
    UART_CMD_WIFI_SCAN_DONE,
}uart_cmd_t;

typedef enum {
    UART_RX_CMD_NONE = -1,
    UART_RX_CMD_UNI = 0,
    UART_RX_CMD_OPENL,
    UART_RX_CMD_CLOSEL,
    UART_RX_CMD_CHECK_WEATHER,
    UART_RX_CMD_CONNECT_MQTT,
    UART_RX_CMD_CONNECT_WIFI,
    UART_RX_CMD_OPEN_APP_1,
    UART_RX_CMD_OPEN_APP_2,
    UART_RX_CMD_OPEN_APP_3,
    UART_RX_CMD_OPEN_APP_4,
    UART_RX_CMD_OPEN_APP_5,
    UART_RX_CMD_OPEN_APP_6,
    UART_RX_CMD_WIFI_SCAN,
}uart_rx_cmd_t;


typedef struct {
    char  data[UART_SEND_MAX];
    int   len;
}uart_data_t;

typedef struct {
    uart_data_t uart_data;
    char* pcm;
}user_data_t;

extern user_data_t user_data[10];

void voice_uart_task(void* arg);
#endif