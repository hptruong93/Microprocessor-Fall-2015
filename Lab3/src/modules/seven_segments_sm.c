#include <math.h>

#include "interfaces/seven_segments_interface.h"
#include "seven_segments_sm.h"


#define SEVEN_SEGMENT_DISPLAY_INTEGER		0
#define SEVEN_SEGMENT_DISPLAY_FLOAT         1

static uint8_t digits[4];
static uint8_t dots[4];

void seven_segment_set_display_int(uint16_t value) {
	for (uint8_t i = 0; i < 4; i++) {
		digits[i] = value % 10;
		value = value / 10;

		dots[i] = 0;
	}
}

void seven_segment_set_display_float(float f, uint8_t decimal_places) {
	for (uint8_t i = 0; i < 4; i++) {
		dots[i] = 0;
	}	

	if (decimal_places == 0) {
		seven_segment_set_display_int((uint16_t) floor(f));
	} else if (decimal_places == 1) {
		seven_segment_set_display_int((uint16_t) floor(10*f));
		dots[1] = 1;
	} else if (decimal_places == 2) {
		seven_segment_set_display_int((uint16_t) floor(100*f));
		dots[2] = 1;
	}
	// else if (decimal_places == 3) {
	// 	seven_segment_set_display_int((uint16_t) floor(1000*f));
	// 	dots[3] = 1;
	// }
}

void seven_segments_set_display_float_smart(float f) {
	if (f < 10) {
		seven_segment_set_display_float(f, 2);
	} else if (f < 100) {
		seven_segment_set_display_float(f, 1);
	} else {
		seven_segment_set_display_float(f, 0);
	}
}

void seven_segment_periodic_display(void) {
	static const uint16_t interface_digits[4] = {SEVEN_SEGMENT_DIGIT_4, SEVEN_SEGMENT_DIGIT_3, SEVEN_SEGMENT_DIGIT_2, SEVEN_SEGMENT_DIGIT_1};
	static uint8_t state = 0;

	state = (state + 1) % 3;
	seven_segment_display(interface_digits[state + 1], digits[state], dots[state]);
}
