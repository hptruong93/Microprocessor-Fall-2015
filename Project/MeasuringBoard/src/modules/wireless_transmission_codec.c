#include "wireless_transmission_codec.h"

//Encoding constant declarations
static const uint8_t CODEC_ESCAPE_BYTE = 0x09;

static const uint8_t CODEC_ESCAPED_CODEC_ESCAPE_BYTE = CODEC_ESCAPE_BYTE;
static const uint8_t CODEC_ESCAPED_PAD_BYTE = 0x10;
static const uint8_t CODEC_ESCAPED_START_PACKET = 0x11;
static const uint8_t CODEC_ESCAPED_END_PACKET = 0x12;


/********************************************************************************************************************/
/****************************************** Codec section ***********************************************************/
/********************************************************************************************************************/
/**
 * Pad a packet with null byte.
 * This will append additional bytes at the end of the buffer. Therefore it is the responsibility of the client to ensure
 * that there is enough space in the input buffer
 *
 * @param src	Pointer to the source buffer where input values are read
 * @param len	Length of the source buffer
 *
 * @return number of bytes in the buffer after padding (padded length)
 */
uint16_t wireless_transmission_codec_pad(uint8_t* src, uint16_t len) {
	src[len] = CODEC_ESCAPE_BYTE;
	src[len + 1] = CODEC_ESCAPED_PAD_BYTE;

	return len + 2;
}

/**
 * Encode a packet
 *
 * @param dst	Pointer to the destination buffer where result is written to
 * @param src	Pointer to the source buffer where input values are read
 * @param len	Length of the source buffer
 *
 * @return number of bytes in the destination buffer (encoded length)
 */
uint16_t wireless_transmission_encode(uint8_t* dst, uint8_t* src, uint16_t len) {
	uint16_t index = 0;

	for (uint16_t i = 0; i < len; i++) {
		uint8_t current = src[i];

		if (current == CODEC_ESCAPE_BYTE) {
			dst[index++] = CODEC_ESCAPE_BYTE;
			dst[index++] = CODEC_ESCAPED_CODEC_ESCAPE_BYTE;
		} else if (current == START_PACKET) {
			dst[index++] = CODEC_ESCAPE_BYTE;
			dst[index++] = CODEC_ESCAPED_START_PACKET;
		} else if (current == END_PACKET) {
			dst[index++] = CODEC_ESCAPE_BYTE;
			dst[index++] = CODEC_ESCAPED_END_PACKET;
		} else {
			dst[index++] = current;
		}
	}

	return index;
}

/**
 * Decode a packet from an encoded packet
 *
 * @param dst	Pointer to the destination buffer where result is written to
 * @param src	Pointer to the source buffer where input values are read
 * @param len	Length of the source buffer
 *
 * @return number of bytes in the destination buffer (decoded length)
 */
uint16_t wireless_transmission_decode(uint8_t* dst, uint8_t* src, uint16_t len) {
	uint16_t index = 0, is_escaped = FALSE;

	for (uint16_t i = 0; i < len; i++) {
		uint8_t current = src[i];

		if (is_escaped) {//Escaped mode
			if (current == CODEC_ESCAPED_CODEC_ESCAPE_BYTE) {
				dst[index++] = CODEC_ESCAPE_BYTE;
			} else if (current == 0x10) {
				//Intentionally left blank since padding element should be ignored
			} else if (current == CODEC_ESCAPED_START_PACKET) {
				dst[index++] = START_PACKET;
			} else if (current == CODEC_ESCAPED_END_PACKET) {
				dst[index++] = END_PACKET;
			} else {//Invalid escaped sequence. Unable to decode
				return 0;
			}

			is_escaped = FALSE;
		} else {//Regular mode
			if (current == CODEC_ESCAPE_BYTE) {
				is_escaped = TRUE;
			} else {
				dst[index++] = current;
			}
		}
	}

	return index;
}
