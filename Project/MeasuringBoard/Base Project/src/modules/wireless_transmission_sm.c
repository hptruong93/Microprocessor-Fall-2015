#include <string.h>
#include "wireless_transmission_sm.h"
#include "interfaces/cc2500.h"
#include "modules/wireless_transmission_protocol_sm.h"
#include "my_types.h"

static uint8_t send_buffer[WIRELESS_TRANSMISSION_PACKET_SIZE];
static uint8_t receive_buffer[WIRELESS_TRANSMISSION_PACKET_SIZE];
static uint8_t receive_overload[WIRELESS_TRANSMISSION_PACKET_SIZE];

static uint8_t operation_state;
static uint16_t sending_len;
static uint16_t sending_index, receiving_index;

void wireless_transmission_init(void) {
	operation_state = WIRELESS_TRANSMISSION_STATE_IDLE;

	sending_index = 0;
	receiving_index = 0;

	sending_len = 0;
}

void wireless_transmission_set_send_packet(uint8_t* packet, uint16_t len) {
	if (len > WIRELESS_TRANSMISSION_PACKET_SIZE) {
		return;
	}

	sending_index = 0;
	sending_len = len;
	memcpy(send_buffer, packet, len);
	operation_state = WIRELESS_TRANSMISSION_STATE_TRANSMIT;
}

void wireless_transmission_receive_packet(uint8_t* packet) {
	receiving_index = 0;
	for (uint16_t i = 0; i < WIRELESS_TRANSMISSION_PACKET_SIZE; i++) {
		receive_buffer[i] = 0;
	}
	operation_state = WIRELESS_TRANSMISSION_STATE_RECEIVE;
}

void wireless_transmission_periodic(void) {
	if (operation_state == WIRELESS_TRANSMISSION_STATE_TRANSMIT) {
		static uint8_t pending_send = FALSE;

		uint8_t state = CC2500_get_state();
		if (state == CC2500_STATE_TX_UNDERFLOW) {
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
				pending_send = TRUE;
				uint8_t to_send = send_buffer[sending_index];
				CC2500_write_tx_one(to_send);
				sending_index++;
			}
		}
	} else if (operation_state == WIRELESS_TRANSMISSION_STATE_RECEIVE) {
		static uint8_t expecting_end = FALSE;

		uint8_t state = CC2500_get_state();
		if (state == CC2500_STATE_RX_OVERFLOW) {
			operation_state = WIRELESS_TRANSMISSION_STATE_ERROR;
		} else if (state != CC2500_STATE_RX) {
			CC2500_read_one(CC2500_SRX);
		} else {
			uint8_t rxbytes = CC2500_get_rxbytes();
			if (rxbytes > 0) {
				uint8_t received = CC2500_read_rx_one();
				if (expecting_end == FALSE) {
					if (received == WIRELESS_TRANSMISSION_PROTOCOL_START_PACKET) {
						expecting_end = TRUE;
					}
				} else {//expecting_end = TRUE
					if (received == WIRELESS_TRANSMISSION_PROTOCOL_END_PACKET || received == WIRELESS_TRANSMISSION_PROTOCOL_START_PACKET) {
						operation_state = WIRELESS_TRANSMISSION_STATE_IDLE;
					} else {
						receive_buffer[receiving_index] = received;
						receiving_index++;
					}
				}
			}
		}
	}
}

uint8_t wireless_transmission_get_state(void) {
	return operation_state;
}
