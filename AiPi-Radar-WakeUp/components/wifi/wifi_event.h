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

extern char ip_addr_str[];
extern char ap_ssid[];
extern char ap_password[];

#define SSID_KEY "SSID"
#define PASS_KEY "PASS"

int wifi_start_firmware_task(void);
void wifi_event_handler(uint32_t code);
uint8_t wifi_connect(char* ssid, char* passwd);
void flash_erase_set(char* key, char* value);
char* flash_get_data(char* key, uint32_t len);
void system_start_auto_connenct(int enable);
void flash_erase_set_numble(char* KEY, uint32_t numble);
uint32_t flash_get_data_numble(char* key);
#endif