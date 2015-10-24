#ifndef SEVEN_SEGMENT_SM_H
#define SEVEN_SEGMENT_SM_H

#include "stm32f4xx.h"

void seven_segment_set_display_int(uint16_t value);
void seven_segment_set_display_float(float f, uint8_t decimal_places);
void seven_segments_set_display_float_smart(float f);
void seven_segment_periodic_display(void);

#endif
