#include <string.h>
#include "protocol_go_back_1.h"

#include "modules/wireless_transmission_sm.h"
#include "my_types.h"

static const uint8_t MAX_TIMEOUT = 20;
static const uint8_t MIN_ACK_COUNT = 10;
static const uint8_t BASE_ID = 1; //This has to not cover the START_PACKET and END_PACKET signal
static const uint8_t ID_LEN = 1;

static uint8_t receive_buffer[WIRELESS_TRANSMISSION_PACKET_SIZE];
static wireless_received_packet received_packet;

static uint8_t mode;
static uint8_t state;
static uint8_t current_id;
static uint8_t timeout_count;
static uint8_t ack_left;

/////////////////////////////////////////////////////////////////////////////////////////////////////
static uint8_t next_id() {
	static uint8_t id_count = BASE_ID;
	id_count = (id_count + 1 == 250) ? BASE_ID : id_count + 1;
	current_id = id_count;
	return id_count;
}

static uint8_t consider_retransmit(void) {
	timeout_count--;
	if (timeout_count == 0) {
		wireless_transmission_retransmit();
		state = GO_BACK_ONE_SENDER_STATE_SEND;
		timeout_count = MAX_TIMEOUT;
		return TRUE;
	}
	return FALSE;
}

static void transmit_ack() {
	static uint8_t ack_packet[1];
	ack_packet[0] = current_id;
	wireless_transmission_transmit(ack_packet, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void protocol_go_back_1_init(uint8_t operation_mode) {
	received_packet.buffer = receive_buffer;
	mode = operation_mode;

	if (mode == GO_BACK_ONE_MODE_SENDER) {
		state = GO_BACK_ONE_SENDER_STATE_IDLE;
	} else {
		state = GO_BACK_ONE_RECEIVER_STATE_IDLE;
	}
}

uint8_t protocol_go_back_1_get_state(void) {
	return state;
}

uint8_t protocol_go_back_1_get_received_data(uint8_t* dest) {
	memcpy(dest, receive_buffer, WIRELESS_TRANSMISSION_PACKET_SIZE);
	return received_packet.len;
}

void protocol_go_back_1_receive(void) {
	if (mode != GO_BACK_ONE_MODE_RECEIVER) {
		return;
	}

	state = GO_BACK_ONE_RECEIVER_STATE_RECEIVE;
	ack_left = MIN_ACK_COUNT;
	memset(receive_buffer, 0, WIRELESS_TRANSMISSION_PACKET_SIZE);
	wireless_transmission_receive_packet();
}

void protocol_go_back_1_send(uint8_t* packet, uint8_t len) {
	if (mode != GO_BACK_ONE_MODE_SENDER) {
		return;
	}

	*(packet - ID_LEN) = next_id();
	wireless_transmission_transmit(packet - ID_LEN, len);
	timeout_count = MAX_TIMEOUT;
	state = GO_BACK_ONE_SENDER_STATE_SEND;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
static void protocol_go_back_1_periodic_sender(uint8_t* debug) {
	uint8_t lower_layer_state = wireless_transmission_get_state();
	if (lower_layer_state == WIRELESS_TRANSMISSION_STATE_ERROR) {
		state = GO_BACK_ONE_STATE_ERROR;
		return;
	}

	if (state == GO_BACK_ONE_SENDER_STATE_SEND) {
		if (lower_layer_state == WIRELESS_TRANSMISSION_STATE_IDLE) {
			wireless_transmission_receive_packet();
			state = GO_BACK_ONE_SENDER_STATE_ACK;
		}
	} else if (state == GO_BACK_ONE_SENDER_STATE_ACK) {
		if (lower_layer_state != WIRELESS_TRANSMISSION_STATE_IDLE) {
			consider_retransmit();
			return;
		}

		wireless_transmission_get_received_packet(&received_packet);
		if (received_packet.status != WIRELESS_TRANSMISSION_VERIFY_OK) {
			if (consider_retransmit() == FALSE) {
				wireless_transmission_receive_packet();	
			}
			return;
		}

		uint8_t id = receive_buffer[1];
		if (id == current_id) {
			printf("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK\n");
			state = GO_BACK_ONE_SENDER_STATE_IDLE;
		} else {
			if (consider_retransmit() == FALSE) {
				wireless_transmission_receive_packet();	
			}
		}
	}
}

static void protocol_go_back_1_periodic_receiver(uint8_t* debug) {
	uint8_t lower_layer_state = wireless_transmission_get_state();
	*debug = lower_layer_state;

	if (lower_layer_state == WIRELESS_TRANSMISSION_STATE_ERROR) {
		state = GO_BACK_ONE_STATE_ERROR;
		return;
	}

	if (state == GO_BACK_ONE_RECEIVER_STATE_RECEIVE) {
		if (lower_layer_state != WIRELESS_TRANSMISSION_STATE_IDLE) {
			return;
		}

		wireless_transmission_get_received_packet(&received_packet);
		if (received_packet.status != WIRELESS_TRANSMISSION_VERIFY_OK) {
			wireless_transmission_receive_packet();
			return;
		}

		uint8_t id = receive_buffer[1];
		current_id = id;
		transmit_ack();
		state = GO_BACK_ONE_RECEIVER_STATE_ACK;
	} else if (state == GO_BACK_ONE_RECEIVER_STATE_ACK) {
		if (lower_layer_state == WIRELESS_TRANSMISSION_STATE_IDLE) {
			ack_left--;
			if (ack_left == 0) {
				state = GO_BACK_ONE_RECEIVER_STATE_IDLE;
			} else {
				transmit_ack();
			}
		}
	}
}

void protocol_go_back_1_periodic(uint8_t* debug) {
	if (mode == GO_BACK_ONE_MODE_SENDER) {
		protocol_go_back_1_periodic_sender(debug);
	} else if (mode == GO_BACK_ONE_MODE_RECEIVER) {
		protocol_go_back_1_periodic_receiver(debug);
	}
}
