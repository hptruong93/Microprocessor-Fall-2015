#include <string.h>
#include "protocol_go_back_0.h"

#include "modules/wireless_transmission_sm.h"
#include "my_types.h"

static const uint8_t MAX_REPEAT = 5;
static const uint8_t BASE_ID = 4; //This has to not cover the START_PACKET and END_PACKET signal
static const uint8_t ID_LEN = 1;

static uint8_t receive_buffer[WIRELESS_TRANSMISSION_PACKET_SIZE];
static wireless_received_packet received_packet;

static uint8_t mode;
static uint8_t state;
static uint8_t current_id;
static uint8_t repeat_count;

/////////////////////////////////////////////////////////////////////////////////////////////////////
static uint8_t next_id() {
	static uint8_t id_count = BASE_ID;
	id_count = (id_count + 1 == 0) ? BASE_ID : id_count + 1;
	return id_count;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void protocol_go_back_0_init(uint8_t operation_mode) {
	received_packet.buffer = receive_buffer;
	mode = operation_mode;

	if (mode == GO_BACK_ZERO_MODE_SENDER) {
		state = GO_BACK_ZERO_SENDER_STATE_IDLE;
	} else if (mode == GO_BACK_ZERO_MODE_RECEIVER) {
		state = GO_BACK_ZERO_RECEIVER_STATE_IDLE;
	}
}

uint8_t protocol_go_back_0_get_state(void) {
	return state;
}

uint8_t protocol_go_back_0_get_received_data(uint8_t* dest) {
	memcpy(dest, receive_buffer + 2, received_packet.len - 1);
	return receive_buffer[1];
}

void protocol_go_back_0_receive(void) {
	if (mode != GO_BACK_ZERO_MODE_RECEIVER) {
		return;
	}

	state = GO_BACK_ZERO_RECEIVER_STATE_RECEIVE;
	memset(receive_buffer, 0, WIRELESS_TRANSMISSION_PACKET_SIZE);
	wireless_transmission_receive_packet();
}

void protocol_go_back_0_send(uint8_t* packet, uint8_t len) {
	if (mode != GO_BACK_ZERO_MODE_SENDER) {
		return;
	}

	*(packet - ID_LEN) = next_id();
	wireless_transmission_transmit(packet - ID_LEN, len);
	repeat_count = 0;
	state = GO_BACK_ZERO_SENDER_STATE_SEND;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
static void protocol_go_back_0_periodic_sender(uint8_t* debug) {
	uint8_t lower_layer_state = wireless_transmission_get_state();
	// printf("State is %d. Lower state is %d\n", state, lower_layer_state);
	if (lower_layer_state == WIRELESS_TRANSMISSION_STATE_ERROR) {
		state = GO_BACK_ZERO_STATE_ERROR;
		return;
	}

	if (state == GO_BACK_ZERO_SENDER_STATE_SEND) {
		if (lower_layer_state == WIRELESS_TRANSMISSION_STATE_IDLE) {
			repeat_count++;
			if (repeat_count == MAX_REPEAT) {
				state = GO_BACK_ZERO_SENDER_STATE_IDLE;
			} else {
				wireless_transmission_retransmit();
			}
		}
	}
}

static void protocol_go_back_0_periodic_receiver(uint8_t* debug) {
	uint8_t lower_layer_state = wireless_transmission_get_state();
	*debug = lower_layer_state;

	if (lower_layer_state == WIRELESS_TRANSMISSION_STATE_ERROR) {
		state = GO_BACK_ZERO_STATE_ERROR;
		return;
	}

	if (state == GO_BACK_ZERO_RECEIVER_STATE_RECEIVE) {
		if (lower_layer_state != WIRELESS_TRANSMISSION_STATE_IDLE) {
			return;
		}

		wireless_transmission_get_received_packet(&received_packet);
		if (received_packet.status != WIRELESS_TRANSMISSION_VERIFY_OK) {
			wireless_transmission_receive_packet();
			return;
		}

		state = GO_BACK_ZERO_RECEIVER_STATE_IDLE;
	}
}

void protocol_go_back_0_periodic(uint8_t* debug) {
	if (mode == GO_BACK_ZERO_MODE_SENDER) {
		protocol_go_back_0_periodic_sender(debug);
	} else if (mode == GO_BACK_ZERO_MODE_RECEIVER) {
		protocol_go_back_0_periodic_receiver(debug);
	}
}
