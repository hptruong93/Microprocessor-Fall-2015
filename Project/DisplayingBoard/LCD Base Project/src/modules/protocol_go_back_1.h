#ifndef PROTOCOL_GO_BACK_1_H
#define PROTOCOL_GO_BACK_1_H

#include "stm32f4xx.h"
#include "modules/wireless_transmission_sm.h"

/*
* Go back 1 protocol:
* @author: Hoai Phuoc Truong
*
* Usage:
* I) Call protocol_go_back_1_init with appropriate mode after CC2500 initialization
* II) Periodically call protocol_go_back_1_periodic with appropriate frequency (see frequency explanation section below)
* III) To send a packet, follow the sequence below:
*		1) Call protocol_go_back_1_send on sender. Immediately go to 2
*		2) Adjust frequency f[sender] < f[receiver]
*		3) Wait until state becomes GO_BACK_ONE_SENDER_STATE_ACK
*		4) Adjust frequency f[sender] > f[receiver]
*		5) Wait on state change. If state changes to GO_BACK_ONE_SENDER_STATE_SEND, go to step 2
*		6) Wait on state change. If state changes to GO_BACK_ONE_SENDER_STATE_IDLE, transmission is complete. Terminate.
*
* IV) To receive a packet, follow the sequence below:
*		1) Call protocol_go_back_1_receive. Immediately go to 2
*		2) Adjust frequency f[receiver] > f[sender]
*		3) Wait until state becomes GO_BACK_ONE_RECEIVER_STATE_ACK
*		4) Adjust frequency f[receiver] < f[sender]
*		5) Wait on state change. If state changes to GO_BACK_ONE_RECEIVER_STATE_IDLE, packet has been successfully received.
*		6) Call protocol_go_back_1_get_received_data with a buffer pointer to retrieve the packet.
*
* V) Received packet:
*		Received packet will have the following format:
*                  1 byte                   1 byte     Variable length
*			Length (excluding ID field)  |    ID    |       Data
*
* VI) protocol_go_back_1_periodic call frequency:
*		To reliably receive a packet, the packet receiver must have frequency larger than that of the packet transmitter.
*		Therefore, it is neccesary to adjust the frequency of the SENDER and the RECEIVER accordingly so that the packets
*		can be transmitted reliably.
*		For example, when the SENDER is in state GO_BACK_ONE_SENDER_STATE_SEND, it is transmitting
*		packet and therefore SENDER frequency should be lower than RECEIVER frequency. Similarly, when the SENDER is in state 
*		GO_BACK_ONE_SENDER_STATE_ACK, the RECEIVER is transmitting packet and therefore SENDER frequency should be higher than 
*		RECEIVER frequency.
*		In practice, receiver frequency should be twice transmitter frequency for good transmission.
*
* VII) Error handling:
*		When state becomes GO_BACK_ONE_STATE_ERROR, the lower layer encountered an internal error. Simply call protocol_go_back_1_init
*		with the appropriate mode to reset the state machine.
*
*/

#define GO_BACK_ONE_MODE_SENDER    254
#define GO_BACK_ONE_MODE_RECEIVER  253

#define GO_BACK_ONE_STATE_ERROR       255

#define GO_BACK_ONE_SENDER_STATE_IDLE 0
#define GO_BACK_ONE_SENDER_STATE_SEND 1
#define GO_BACK_ONE_SENDER_STATE_ACK 2

#define GO_BACK_ONE_RECEIVER_STATE_IDLE 3
#define GO_BACK_ONE_RECEIVER_STATE_RECEIVE 4
#define GO_BACK_ONE_RECEIVER_STATE_ACK 5

void protocol_go_back_1_init(uint8_t mode);
uint8_t protocol_go_back_1_get_state(void);
void protocol_go_back_1_receive(void);
uint8_t protocol_go_back_1_get_received_data(uint8_t* dest);
void protocol_go_back_1_send(uint8_t* packet, uint8_t len);
void protocol_go_back_1_periodic(uint8_t* debug);

#endif
