/**
 * @file relays_dev.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-23
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef RELAYS_DEV_H
#define RELAYS_DEV_H

#define RELAYS_SW1 20
#define RELAYS_SW2 17
#define RELAYS_SW3 15

typedef struct {
    int gpio_state;
    int gpio_pin;
    char* relays_name;
}relays_dev_t;

extern relays_dev_t relay_sw1;
extern relays_dev_t relay_sw2;
extern relays_dev_t relay_sw3;
void relays_device_init(void);
void relays_on(relays_dev_t* relays);
void relays_off(relays_dev_t* relays);
#endif