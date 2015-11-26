#include <string.h>
#include "wireless_transmission_protocol_sm.h"
#include "my_types.h"


static const uint8_t HEADER_COUNT = 5;
static const uint8_t END_COUNT = 5;

typedef struct {
  uint8_t xor_all;
  uint8_t xor_even;
  uint8_t xor_odd;
} my_check_sum;

uint8_t wireless_transmission_protocol_sanity_check(uint8_t* full_packet, uint16_t full_len) {
	for (uint8_t i = 0; i < HEADER_COUNT; i++) {
		if (full_packet[i] != WIRELESS_TRANSMISSION_PROTOCOL_START_PACKET) {
			return FALSE;
		}
	}

	for (uint8_t i = 0; i < END_COUNT; i++) {
		if (full_packet[i] != WIRELESS_TRANSMISSION_PROTOCOL_END_PACKET) {
			return FALSE;
		}
	}
	
	if (full_len <= HEADER_COUNT + WIRELESS_TRANSMISSION_PROTOCOL_CHECK_SUM_COUNT + END_COUNT) {
		return FALSE;
	}

	uint16_t data_len = full_len - HEADER_COUNT - WIRELESS_TRANSMISSION_PROTOCOL_CHECK_SUM_COUNT - END_COUNT;
	return wireless_transmission_protocol_checksum(full_packet + HEADER_COUNT, full_packet + HEADER_COUNT + data_len, data_len);
}

static void calculate_check_sum(uint8_t* raw_packet, uint16_t len, my_check_sum* result) {
	uint8_t xor_all = 0, xor_even = 0, xor_odd = 0;
	for (uint16_t i = 0; i < len; i++) {
		uint8_t current_byte = raw_packet[i];
		xor_all ^= current_byte;

		if (i % 2 == 0) {
			xor_even ^= current_byte;
		} else {
			xor_odd ^= current_byte;
		}
	}

	result->xor_all = xor_all;
	result->xor_even = xor_even;
	result->xor_odd = xor_odd;
}

uint16_t wireless_transmission_protocol_encapsulate(uint8_t* raw_packet, uint16_t len) {
	memset(raw_packet - HEADER_COUNT, WIRELESS_TRANSMISSION_PROTOCOL_START_PACKET, HEADER_COUNT);
	calculate_check_sum(raw_packet, len, (my_check_sum*) (raw_packet + len));
	memset(raw_packet + len + WIRELESS_TRANSMISSION_PROTOCOL_CHECK_SUM_COUNT, WIRELESS_TRANSMISSION_PROTOCOL_START_PACKET, END_COUNT);
	
	return HEADER_COUNT + WIRELESS_TRANSMISSION_PROTOCOL_CHECK_SUM_COUNT + END_COUNT + len;
}

uint8_t wireless_transmission_protocol_checksum(uint8_t* raw_packet, uint8_t* check_sum, uint16_t packet_len) {
	my_check_sum expecting;
	calculate_check_sum(raw_packet, packet_len, &expecting);
	return memcmp(check_sum, &expecting, WIRELESS_TRANSMISSION_PROTOCOL_CHECK_SUM_COUNT) == 0;
}

