#ifndef WIRELESS_TRANSMISSION_H
#define WIRELESS_TRANSMISSION_H

#include "stm32f4xx.h"

#define WIRELESS_TRANSMISSION_STATE_IDLE       0
#define WIRELESS_TRANSMISSION_STATE_RECEIVE    1
#define WIRELESS_TRANSMISSION_STATE_TRANSMIT   2
#define WIRELESS_TRANSMISSION_STATE_ERROR      3

#define WIRELESS_TRANSMISSION_PACKET_SIZE           100

#define WIRELESS_TRANSMISSION_VERIFY_OK   					1
#define WIRELESS_TRANSMISSION_VERIFY_INCORRECT_CHECKSUM		2
#define WIRELESS_TRANSMISSION_VERIFY_INCORRECT_LENGTH		3
#define WIRELESS_TRANSMISSION_VERIFY_INVALID_CHECK_SUM		4

#define START_PACKET 255
#define END_PACKET 254

typedef struct {
	uint8_t status;
	uint8_t len;
	uint8_t* buffer;
} wireless_received_packet;

void wireless_transmission_init(void);

uint8_t wireless_transmission_get_state(void);
void wireless_transmission_periodic(uint8_t* rbyte);

uint8_t wireless_transmission_transmit(uint8_t* packet, uint8_t len);
uint8_t wireless_transmission_retransmit(void);
void wireless_transmission_receive_packet(void);
void wireless_transmission_get_received_packet(wireless_received_packet* received_packet);

//Testing
uint8_t wireless_transmission_protocol_sanity_check(uint8_t* full_packet, uint8_t full_len);
uint8_t wireless_transmission_protocol_encapsulate(uint8_t* raw_packet, uint8_t len);
uint8_t wireless_transmission_protocol_checksum(uint8_t* raw_packet, uint8_t* check_sum, uint8_t packet_len);

#endif
