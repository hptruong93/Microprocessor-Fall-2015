#include <stdio.h>
#include "system_config.h"
#include "interfaces/lsm9ds1.h"

/* This file contains a list of methods that are defined in the main method and SysTick_Handler
*/

static uint8_t system_ticks;

void print_buffer(uint8_t* buffer, uint8_t num) {
	printf("Buffer is: ");
	for (uint8_t i = 0; i < num; i++) {
		printf("%d - ", buffer[i]);
	}
	printf("\n");
}

int main() {

/*	system_init() method will initialize generic_init(), led_init(), seven_segment_init(), seven_segments_sm_init(), keypad_init(), accelerometer_init()
*	SysTick_Config method will set ticks by dividing systemCoreClock with systick frequency (168MHz/100Hz)
*	led_rotation_set_mode method will rotate single/double/triple LEDs or race all the LEDs or simply blink all LEDs 
*	@param		LED_ROTATION_MODE_ROTATE_RACE		defined as 3, which will call method led_rotation_rotate_leds_race() and display race of LED rotations to show initialization
*	print_instruction() method will display instruction in terms of pressing * to start the game
*/

	printf("Begin\n");
	//system_init();
	printf("Done system init\n");
	
	SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY);

	LSM9DS1_InitTypeDef  LSM9DS1_InitStruct;

	/* Set configuration of LIS302DL*/
	LSM9DS1_InitStruct.Power_Mode_Output_DataRate = LSM9DS1_DATARATE_100;
	LSM9DS1_InitStruct.Axes_Enable = LSM9DS1_X_ENABLE | LSM9DS1_Y_ENABLE | LSM9DS1_Z_ENABLE;
	LSM9DS1_InitStruct.Full_Scale = LSM9DS1_FULLSCALE_4;
	LSM9DS1_InitStruct.AA_Filter_BW = LSM9DS1_AA_BW_200;
	LSM9DS1_InitStruct.Continous_Update = LSM9DS1_ContinousUpdate_Enabled;
	LSM9DS1_InitStruct.Self_Test = LSM9DS1_SELFTEST_NORMAL;
	// LSM9DS1_Init(&LSM9DS1_InitStruct);
	LSM9DS1_LowLevel_Init();
	printf("Done driver init\n");
	
	uint8_t buffer[6] = {1,1,1,1,1,1};
	LSM9DS1_Read(buffer, 0xf, 1);
	LSM9DS1_Read(&(buffer[1]), 0x10, 1);
	LSM9DS1_Read(&(buffer[2]), 0x11, 1);
	LSM9DS1_Read(&(buffer[3]), 0x12, 1);
	print_buffer(buffer, 6);

	uint8_t test = 10;
	LSM9DS1_Write(&test, 0x10, 1);
	LSM9DS1_Read(buffer, 0x10, 1);
	print_buffer(buffer, 6);

	// while (1) {
	// 	while (!system_ticks);

		
		
	// 	system_ticks = 0;
	// }
	
	
	return 0;
}


/*interrupt handler
*/

void SysTick_Handler() {
	system_ticks = 1;
}
