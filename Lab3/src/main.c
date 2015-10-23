#include <stdio.h>
#include "system_config.h"

#include "modules/led_rotation_sm.h"
#include "modules/accelerometer_sm.h"
#include "interfaces/seven_segments_interface.h"

static uint8_t system_ticks;

int main() {
	printf("Begin\n");
	system_init();
	printf("Done system init\n");

	SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY);
	//running while loop to control the frequency measurement readings of the temperature 
	//waits for interrupt to read the temperature readings
	while (1) {
		while (!system_ticks);

		led_rotation_rotate_leds();
		accelerometer_process();
		seven_segment_display(SEVEN_SEGMENT_DIGIT_1, 1);

		system_ticks = 0;
	}
	
	
	return 0;
}


/*interrupt handler
*/

void SysTick_Handler() {
	system_ticks = 1;
}
