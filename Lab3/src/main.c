<<<<<<< HEAD
#include <stdio.h>
#include "system_config.h"
#include "modules/led_rotation_sm.h"
#include "modules/lab3_game.h"

static uint8_t system_ticks;

int main() {
	printf("Begin\n");
	system_init();
	printf("Done system init\n");
	
	SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY);
	led_rotation_set_mode(LED_ROTATION_MODE_ROTATE_RACE);

	print_instruction();
	while (1) {
		while (!system_ticks);

		// led_rotation_rotate_leds();
		gg();
		
		system_ticks = 0;
	}
	
	
	return 0;
}


/*interrupt handler
*/

void SysTick_Handler() {
	system_ticks = 1;
}
=======
#include <stdio.h>
#include "system_config.h"
#include "modules/led_rotation_sm.h"
#include "modules/lab3_game.h"

static uint8_t system_ticks;

int main() {
	printf("Begin\n");
	system_init();
	printf("Done system init\n");
	
	SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY);
	led_rotation_set_mode(LED_ROTATION_MODE_ROTATE_RACE);

	print_instruction();
	while (1) {
		while (!system_ticks);

		// led_rotation_rotate_leds();
		gg();
		
		system_ticks = 0;
	}
	
	
	return 0;
}


/*interrupt handler
*/

void SysTick_Handler() {
	system_ticks = 1;
}
>>>>>>> origin/master
