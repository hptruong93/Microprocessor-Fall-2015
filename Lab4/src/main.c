#include <stdio.h>
#include "system_config.h"
#include "modules/led_rotation_sm.h"

#include "interfaces/led_interface.h"
#include "interfaces/seven_segments_interface.h"
#include "modules/accelerometer_sm.h"
#include "modules/seven_segments_sm.h"
#include "modules/keypad_input_sm.h"

/* This file contains a list of methods that are defined in the main method and SysTick_Handler
*/

static uint8_t system_ticks;

int main() {

/*	system_init() method will initialize generic_init(), led_init(), seven_segment_init(), seven_segments_sm_init(), keypad_init(), accelerometer_init()
*	SysTick_Config method will set ticks by dividing systemCoreClock with systick frequency (168MHz/100Hz)
*	led_rotation_set_mode method will rotate single/double/triple LEDs or race all the LEDs or simply blink all LEDs 
*	@param		LED_ROTATION_MODE_ROTATE_RACE		defined as 3, which will call method led_rotation_rotate_leds_race() and display race of LED rotations to show initialization
*	print_instruction() method will display instruction in terms of pressing * to start the game
*/

	printf("Begin\n");
	system_init();
	printf("Done system init\n");
	
	SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY);
	led_rotation_set_mode(LED_ROTATION_MODE_ROTATE_RACE);

/*	running while loop to control the frequency measurement
*	waits for interrupt handler 
*/	
	seven_segment_set_display_mode(SEVEN_SEGMENT_DISPLAY_MODE_BLINK);
	while (1) {
		while (!system_ticks);

		led_rotation_rotate_leds();
		seven_segment_set_display_int(8, SEVEN_SEGMENT_BASE_10);
		
		system_ticks = 0;
	}
	
	
	return 0;
}


/*interrupt handler
*/

void SysTick_Handler() {
	system_ticks = 1;
}
