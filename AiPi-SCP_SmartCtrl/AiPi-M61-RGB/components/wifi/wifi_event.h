/**
 * @file wifi_event.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-06-29
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef WIFI_EVENT_H
#define WIFI_EVENT_H

int wifi_start_firmware_task(void);
void wifi_event_handler(uint32_t code);
uint8_t wifi_connect(char* ssid, char* passwd);
void wifi_connect_task(void* arg);
#endif