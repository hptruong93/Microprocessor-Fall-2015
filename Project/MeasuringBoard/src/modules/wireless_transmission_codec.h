#ifndef WIRELESS_TRANSMISSION_CODEC_H
#define WIRELESS_TRANSMISSION_CODEC_H

#include "stm32f4xx.h"
#include "my_types.h"


/*
* Wireless transmission codec
* @author: Hoai Phuoc Truong
*/

/*
 * Pad a packet with null bytes
 */
uint16_t wireless_transmission_codec_pad(uint8_t* src, uint16_t len);

/*
 * Encode a packet
 */
uint16_t wireless_transmission_codec_encode(uint8_t* dst, uint8_t* src, uint16_t len);

/*
 * Decode a packet
 */
uint16_t wireless_transmission_codec_decode(uint8_t* dst, uint8_t* src, uint16_t len);


#endif
