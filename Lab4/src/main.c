#include <stdio.h>
#include "system_config.h"


// #include "interfaces/led_interface.h"
// #include "interfaces/seven_segments_interface.h"
// #include "interfaces/temperature_sensor_interface.h"

// #include "modules/led_rotation_sm.h"
// #include "modules/accelerometer_sm.h"
// #include "modules/seven_segments_sm.h"
// #include "modules/keypad_input_sm.h"
// #include "modules/temperature_sensor_sm.h"

#include "cmsis_os.h" // ARM::CMSIS:RTOS:Keil RTX

/* This file contains a list of methods that are defined in the main method and SysTick_Handler
*/

void just_do_it(void) {
	// led_rotation_rotate_leds();
		
//	temperature_sensor_read_temperature_raw();
//	float temp = 1;
//	temperature_sensor_read(&temp);

//	printf("Temp is %f\n", temp);
//	seven_segments_set_display_float_smart(temp);
//	temperature_sensor_alarm(temp);
}


osThreadId thrdID1;
void thread1(void const* arg) {
	while (1) {
		// just_do_it();
		printf("AAA\n");
		printf("BBB\n");
		osDelay(100);
	}
}
osThreadDef(thread1, osPriorityNormal, 1, 0);

int main() {
	printf("Begin\n");
	// system_init();
	printf("Done system init\n");
	// led_rotation_set_mode(LED_ROTATION_MODE_ROTATE_RACE);
	
	
	osKernelInitialize();
	thrdID1 = osThreadCreate(osThread(thread1), NULL);
	osKernelStart();
	while (1);
	
	return 0;
}




///*interrupt handler
//*/

//void SysTick_Handler() {
//	system_ticks = 1;
//}
