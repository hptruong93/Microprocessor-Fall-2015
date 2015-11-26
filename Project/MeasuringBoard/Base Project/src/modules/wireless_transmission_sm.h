#ifndef WIRELESS_TRANSMISSION_H
#define WIRELESS_TRANSMISSION_H

#include "stm32f4xx.h"

#define WIRELESS_TRANSMISSION_STATE_IDLE       0
#define WIRELESS_TRANSMISSION_STATE_RECEIVE    1
#define WIRELESS_TRANSMISSION_STATE_TRANSMIT   2
#define WIRELESS_TRANSMISSION_STATE_ERROR      3

#define WIRELESS_TRANSMISSION_PACKET_SIZE           10


void wireless_transmission_init(void);

uint8_t wireless_transmission_get_state(void);
void wireless_transmission_periodic(void);

void wireless_transmission_set_send_packet(uint8_t* packet, uint16_t len);
void wireless_transmission_receive_packet(uint8_t* packet);

#endif
