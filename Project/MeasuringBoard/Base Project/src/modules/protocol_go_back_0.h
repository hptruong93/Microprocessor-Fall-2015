#ifndef PROTOCOL_GO_BACK_1_H
#define PROTOCOL_GO_BACK_1_H

#include "stm32f4xx.h"
#include "modules/wireless_transmission_sm.h"

#define GO_BACK_ZERO_MODE_SENDER    	254
#define GO_BACK_ZERO_MODE_RECEIVER  	253

#define GO_BACK_ZERO_STATE_ERROR       	255

#define GO_BACK_ZERO_SENDER_STATE_IDLE 	0
#define GO_BACK_ZERO_SENDER_STATE_SEND 	1

#define GO_BACK_ZERO_RECEIVER_STATE_IDLE 3
#define GO_BACK_ZERO_RECEIVER_STATE_RECEIVE 4

void protocol_go_back_0_init(uint8_t mode);
uint8_t protocol_go_back_0_get_state(void);
void protocol_go_back_0_receive(void);
uint8_t protocol_go_back_0_get_received_data(uint8_t* dest);
void protocol_go_back_0_send(uint8_t* packet, uint8_t len);
void protocol_go_back_0_periodic(uint8_t* debug);

#endif
