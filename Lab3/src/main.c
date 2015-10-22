#include <stdio.h>
#include "system_config.h"

#include "interfaces/accelerometer_interface.h"


#include "modules/led_rotation_sm.h"


static uint8_t system_ticks;
extern accelerometer_info accelerometer_angles;

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

		static float x, y, z;
		accelerometer_read(&x, &y, &z);
		accelerometer_calculate_rotation(x, y, z);
		printf("PRY = (%.2f, %.2f, %.2f)\n", accelerometer_angles.pitch, accelerometer_angles.roll, accelerometer_angles.yaw);

		system_ticks = 0;
	}
	
	
	return 0;
}


/*interrupt handler
*/

void SysTick_Handler(){
	system_ticks = 1;
}
