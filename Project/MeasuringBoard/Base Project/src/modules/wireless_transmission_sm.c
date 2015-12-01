#include <string.h>
#include "wireless_transmission_sm.h"
#include "interfaces/cc2500.h"
#include "utils/utils.h"
#include "my_types.h"

static uint8_t send_buffer[WIRELESS_TRANSMISSION_PACKET_SIZE];
static uint8_t receive_buffer[WIRELESS_TRANSMISSION_PACKET_SIZE];

static uint8_t operation_state;
static uint8_t sending_len;
static uint8_t sending_index, receiving_index;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t wireless_transmission_protocol_checksum(uint8_t* raw_packet, uint8_t* check_sum, uint8_t packet_len);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void wireless_transmission_init(void) {
	operation_state = WIRELESS_TRANSMISSION_STATE_IDLE;

	sending_index = 0;
	receiving_index = 0;

	sending_len = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const uint8_t START_PACKET = 255;
static const uint8_t END_PACKET = 254;
static const uint8_t COLLISION_CHECKSUM_REPLACEMENT_1 = END_PACKET - 1;
static const uint8_t COLLISION_CHECKSUM_REPLACEMENT_2 = END_PACKET - 2;
static const uint8_t COLLISION_CHECKSUM_REPLACEMENT_3 = END_PACKET - 3;
static const uint8_t COLLISION_CHECKSUM_REPLACEMENT_4 = END_PACKET - 4;

static const uint8_t START_SIGNAL_LEN = 3;
static const uint8_t LEN_LEN = 1;
static const uint8_t FULL_HEADER_LEN = START_SIGNAL_LEN + LEN_LEN;
static const uint8_t DATA_INDEX = FULL_HEADER_LEN - START_SIGNAL_LEN;
static const uint8_t END_SIGNAL_LEN = 3;
static const uint8_t CHECKSUM_LEN = 3;

static const uint8_t MAX_DATA_SIZE = WIRELESS_TRANSMISSION_PACKET_SIZE - FULL_HEADER_LEN - CHECKSUM_LEN - END_SIGNAL_LEN;

typedef struct {
  uint8_t xor_all;
  uint8_t xor2;
  uint8_t xor3;
} my_check_sum;

uint8_t wireless_transmission_protocol_sanity_check(uint8_t* full_packet, uint8_t full_len) {
	for (uint8_t i = 0; i < START_SIGNAL_LEN; i++) {
		if (full_packet[i] != START_PACKET) {
			return FALSE;
		}
	}

	for (uint8_t i = 0; i < END_SIGNAL_LEN; i++) {
		if (full_packet[full_len - i - 1] != END_PACKET) {
			return FALSE;
		}
	}

	if (full_len <= FULL_HEADER_LEN + CHECKSUM_LEN + END_SIGNAL_LEN) {
		return FALSE;
	}

	uint8_t data_len = full_len - FULL_HEADER_LEN - CHECKSUM_LEN - END_SIGNAL_LEN;
	return wireless_transmission_protocol_checksum(full_packet + FULL_HEADER_LEN, full_packet + FULL_HEADER_LEN + data_len, data_len);
}

static void calculate_check_sum(uint8_t* raw_packet, uint8_t len, my_check_sum* result) {
	uint8_t xor_all = 0, xor2 = 0, xor3 = 0;
	for (uint8_t i = 0; i < len; i++) {
		uint8_t current_byte = raw_packet[i];
		xor_all ^= current_byte;

		if (i % 2 == 0) {
			xor2 ^= current_byte;
		}
		if (i % 3 == 0) {
			xor3 ^= current_byte;
		}
	}

	result->xor_all = xor_all == START_PACKET ? COLLISION_CHECKSUM_REPLACEMENT_2 : (xor_all == END_PACKET ? COLLISION_CHECKSUM_REPLACEMENT_4 : xor_all);
	result->xor2 = (xor2 == START_PACKET) ? COLLISION_CHECKSUM_REPLACEMENT_1 : (xor2 == END_PACKET ? COLLISION_CHECKSUM_REPLACEMENT_2 : xor2);
	result->xor3 = (xor3 == START_PACKET) ? COLLISION_CHECKSUM_REPLACEMENT_3 : (xor3 == END_PACKET ? COLLISION_CHECKSUM_REPLACEMENT_4 : xor3);
}

uint8_t wireless_transmission_protocol_encapsulate(uint8_t* raw_packet, uint8_t len) {
	uint8_t* full_packet_start = raw_packet - FULL_HEADER_LEN;
	memset(full_packet_start, START_PACKET, START_SIGNAL_LEN);

	*(full_packet_start + START_SIGNAL_LEN) = len;

	calculate_check_sum(raw_packet, len, (my_check_sum*) (raw_packet + len));
	memset(raw_packet + len + CHECKSUM_LEN, END_PACKET, END_SIGNAL_LEN);

	return FULL_HEADER_LEN + CHECKSUM_LEN + END_SIGNAL_LEN + len;
}

uint8_t wireless_transmission_protocol_checksum(uint8_t* raw_packet, uint8_t* check_sum, uint8_t packet_len) {
	my_check_sum expecting;
	calculate_check_sum(raw_packet, packet_len, &expecting);
	return memcmp(check_sum, &expecting, CHECKSUM_LEN) == 0;
}

void wireless_transmission_get_received_packet(wireless_received_packet* received_packet) {
	if (operation_state != WIRELESS_TRANSMISSION_STATE_IDLE) {
		return;
	}

	if (receiving_index < DATA_INDEX + CHECKSUM_LEN) {
		received_packet->status = WIRELESS_TRANSMISSION_VERIFY_INCORRECT_LENGTH;
		return;
	}

	//Now verify whatever is in receive_buffer
	uint8_t packet_len = receive_buffer[0];
	if (packet_len == 0) {
		received_packet->status = WIRELESS_TRANSMISSION_VERIFY_INCORRECT_LENGTH;
		return;
	}
	received_packet->len = packet_len;

	uint8_t* check_sum_start_left = receive_buffer + DATA_INDEX + packet_len;
	uint8_t* check_sum_start_right = receive_buffer + receiving_index - CHECKSUM_LEN;
	if (check_sum_start_left != check_sum_start_right) {
		received_packet->status = WIRELESS_TRANSMISSION_VERIFY_INVALID_CHECK_SUM;
		return;
	}

	uint8_t check_sum_verify = wireless_transmission_protocol_checksum(receive_buffer + DATA_INDEX, check_sum_start_left, packet_len);
	if (!check_sum_verify) {
		received_packet->status = WIRELESS_TRANSMISSION_VERIFY_INCORRECT_CHECKSUM;
		return;
	}

	memcpy(received_packet->buffer, receive_buffer + DATA_INDEX, receiving_index - DATA_INDEX - CHECKSUM_LEN);
	received_packet->status = WIRELESS_TRANSMISSION_VERIFY_OK;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t wireless_transmission_transmit(uint8_t* packet, uint8_t len) {
	if (len > WIRELESS_TRANSMISSION_PACKET_SIZE) {
		return FALSE;
	}

	if (len > MAX_DATA_SIZE) {
		return FALSE;
	}

	memcpy(send_buffer + FULL_HEADER_LEN, packet, len);
	
	uint8_t new_len = wireless_transmission_protocol_encapsulate(send_buffer + FULL_HEADER_LEN, len);
	if (new_len == 0) {
		return FALSE;
	}

	sending_index = 0;
	sending_len = new_len;

	operation_state = WIRELESS_TRANSMISSION_STATE_TRANSMIT;
	return TRUE;
}

uint8_t wireless_transmission_retransmit(void) {
	if (sending_len == 0) {
		return FALSE;
	}

	sending_index = 0;
	operation_state = WIRELESS_TRANSMISSION_STATE_TRANSMIT;
	return TRUE;
}

void wireless_transmission_receive_packet(void) {
	receiving_index = 0;
	memset(receive_buffer, 0, WIRELESS_TRANSMISSION_PACKET_SIZE);
	operation_state = WIRELESS_TRANSMISSION_STATE_RECEIVE;
}

void wireless_transmission_periodic(uint8_t* rbyte) {
	if (operation_state == WIRELESS_TRANSMISSION_STATE_TRANSMIT) {
		static uint8_t pending_send = FALSE;

		uint8_t state = CC2500_get_state();
		if (state == CC2500_STATE_TX_UNDERFLOW) {
			CC2500_flush_tx();
			operation_state = WIRELESS_TRANSMISSION_STATE_ERROR;
		} else if (state != CC2500_STATE_TX) {
			CC2500_read_one(CC2500_STX);
			pending_send = FALSE;

			if (sending_index == sending_len) {
				operation_state = WIRELESS_TRANSMISSION_STATE_IDLE;
			}
			return;
		} else {
			if (pending_send == FALSE) {
				// uint8_t byte_left_to_send = sending_len - sending_index;
				// uint8_t byte_available = CC2500_get_txbytes();

				// uint8_t sending = MAX(byte_left_to_send, byte_available);
				// sending = MIN(sending, 3);

				// printf("Going to send\n");
				// print_buffer(send_buffer + sending_index, sending);

				// if (sending > 0) {
				// 	CC2500_write_tx(send_buffer + sending_index, sending);
				// 	sending_index += sending;
				// 	pending_send = TRUE;
				// }

				uint8_t to_send = send_buffer[sending_index];
				CC2500_write_tx_one(to_send);
				sending_index++;
				pending_send = TRUE;
			}
		}
	} else if (operation_state == WIRELESS_TRANSMISSION_STATE_RECEIVE) {
		static uint8_t expecting_end = FALSE;

		uint8_t state = CC2500_get_state();
		if (state == CC2500_STATE_RX_OVERFLOW) {
			CC2500_flush_rx();
			operation_state = WIRELESS_TRANSMISSION_STATE_ERROR;
		} else if (state != CC2500_STATE_RX) {
			do {
				CC2500_read_one(CC2500_SRX);
				state = CC2500_get_state();
			} while (state != CC2500_STATE_RX && state != CC2500_STATE_RX_OVERFLOW);
			wireless_transmission_periodic(rbyte);
		} else {
			uint8_t rxbytes = CC2500_get_rxbytes();

			while (rxbytes > 0) {
				uint8_t received = CC2500_read_rx_one();
				if (expecting_end == FALSE) {
					if (received == START_PACKET) {
						expecting_end = TRUE;
					}
				} else {//expecting_end = TRUE
					if (received == END_PACKET || received == START_PACKET) {
						operation_state = WIRELESS_TRANSMISSION_STATE_IDLE;
						return;
					} else {
						receive_buffer[receiving_index] = received;
						receiving_index++;
					}
				}
				rxbytes--;
			}
		}
	}
}

uint8_t wireless_transmission_get_state(void) {
	return operation_state;
}
