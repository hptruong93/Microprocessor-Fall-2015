#include <stdio.h>
#include "system_config.h"

#include "modules/led_rotation_sm.h"
#include "modules/accelerometer_sm.h"
#include "modules/seven_segments_sm.h"
#include "modules/keypad_input_sm.h"

static uint8_t system_ticks;

int main() {
	printf("Begin\n");
	system_init();
	printf("Done system init\n");
	
	SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY);
	led_rotation_set_mode(LED_ROTATION_MODE_ROTATE_RACE);

	while (1) {
		while (!system_ticks);

		led_rotation_rotate_leds();
		accelerometer_process();
		keypad_read_state();
		
		
		uint8_t pressed = keypad_input_read();
		if (pressed != 255) {
			seven_segment_set_display_int(pressed, SEVEN_SEGMENT_BASE_10);
		}

		system_ticks = 0;
	}
	
	
	return 0;
}


/*interrupt handler
*/

void SysTick_Handler() {
	system_ticks = 1;
}
