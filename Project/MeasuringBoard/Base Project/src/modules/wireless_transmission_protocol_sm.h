#ifndef WIRELESS_TRANSMISSION_PROTOCOL_H
#define WIRELESS_TRANSMISSION_PROTOCOL_H

#include "stm32f4xx.h"

#define WIRELESS_TRANSMISSION_PROTOCOL_START_PACKET 0x02
#define WIRELESS_TRANSMISSION_PROTOCOL_END_PACKET 0x03

#define WIRELESS_TRANSMISSION_PROTOCOL_CHECK_SUM_COUNT 3

uint8_t wireless_transmission_protocol_sanity_check(uint8_t* full_packet, uint16_t full_len);
uint16_t wireless_transmission_protocol_encapsulate(uint8_t* raw_packet, uint16_t len);
uint8_t wireless_transmission_protocol_checksum(uint8_t* raw_packet, uint8_t* check_sum, uint16_t packet_len);

#endif
