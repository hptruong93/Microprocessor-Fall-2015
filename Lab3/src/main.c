#include <stdio.h>
#include "system_config.h"

#include "modules/led_rotation_sm.h"
#include "modules/accelerometer_sm.h"
#include "modules/seven_segments_sm.h"
#include "interfaces/keypad_interface.h"

static uint8_t system_ticks;

int main() {
	printf("Begin\n");
	system_init();
	printf("Done system init\n");

	SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY);

	while (1) {
		while (!system_ticks);

		led_rotation_rotate_leds();
		accelerometer_process();
		keypad_read_state();
		
		
		static uint16_t x = 0, y = 0;
		static uint8_t count = 0;
		
		count++;
		
		if (count == 1) {
			x = (x + 1) % 10000;
			count = 0;
		}

		if (x < 500) {
			seven_segments_set_display_float_smart(7.55);
		} else if (x < 1000) {
			seven_segments_set_display_float_smart(119.0);
		} else if (x < 2000) {
			seven_segments_set_display_float_smart(59.3);
		} else {
			seven_segments_set_display_float_smart(100);
		}
		
		// seven_segment_periodic_display();

		// static uint8_t is_1 = 0;
		// if (is_1 == 0) {
		//     seven_segment_display(SEVEN_SEGMENT_DIGIT_1, x, x % 2);
		// } else if (is_1 == 1) {
		//     seven_segment_display(SEVEN_SEGMENT_DIGIT_2, x, 1 - (x % 2));
		// } else if (is_1 == 2) {
		//     seven_segment_display(SEVEN_SEGMENT_DIGIT_3, x, x % 2);
		// } else {
		//     seven_segment_display(SEVEN_SEGMENT_DIGIT_4, x, 1 - (x % 2));
		// }
		// is_1 = (is_1 + 1) % 4;

		
		//seven_segment_display(SEVEN_SEGMENT_DIGIT_2, (x + 1) % 16);
		//seven_segment_display(SEVEN_SEGMENT_DIGIT_3, (x + 2) % 16);
		//seven_segment_display(SEVEN_SEGMENT_DIGIT_4, (x + 3) % 16);
		
		

		system_ticks = 0;
	}
	
	
	return 0;
}


/*interrupt handler
*/

void SysTick_Handler() {
	system_ticks = 1;
}
