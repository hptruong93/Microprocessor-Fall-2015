#include <stdio.h>
#include <math.h>
#include "system_config.h"
#include "utils/utils.h"
#include "interfaces/lsm9ds1.h"
#include "interfaces/accelerometer_interface.h"

/* This file contains a list of methods that are defined in the main method and SysTick_Handler
*/

static uint8_t system_ticks;
static uint8_t test_flag;

void print_buffer(uint8_t* buffer, uint8_t num) {
	printf("Buffer is: ");
	for (uint8_t i = 0; i < num; i++) {
		printf("%d - ", buffer[i]);
	}
	printf("\n");
}

void accelerometer_normalize(float* x, float* y, float* z) {

static const float normalizing_matrix[3][3] = {
	{1.01025618,  0.01006384,  0.02401493},
	{-0.00904249,  1.00515987, -0.01296093},
	{0.03992654,  0.00910658,  1.0367582}
};

static const float cx = -28.44255082;
static const float cy = -217.47119435;
static const float cz = -1548.73592871;


float normalized_values[3];
for (uint8_t i = 0; i < 3; i++) {
	normalized_values[i] = normalizing_matrix[i][0] * (*x) + normalizing_matrix[i][1] * (*y) + normalizing_matrix[i][2] * (*z);
}

*x = (normalized_values[0]) + cx;
*y = (normalized_values[1]) + cy;
*z = (normalized_values[2]) + cz;
}

int main() {

/*	system_init() method will initialize generic_init(), led_init(), seven_segment_init(), seven_segments_sm_init(), keypad_init(), accelerometer_init()
*	SysTick_Config method will set ticks by dividing systemCoreClock with systick frequency (168MHz/100Hz)
*	led_rotation_set_mode method will rotate single/double/triple LEDs or race all the LEDs or simply blink all LEDs 
*	@param		LED_ROTATION_MODE_ROTATE_RACE		defined as 3, which will call method led_rotation_rotate_leds_race() and display race of LED rotations to show initialization
*	print_instruction() method will display instruction in terms of pressing * to start the game
*/

	printf("Begin\n");
	system_init();
	
	SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY);

	LSM9DS1_InitTypeDef  LSM9DS1_InitStruct;
	
	/* Set configuration of LIS302DL*/
	LSM9DS1_InitStruct.Output_DataRate = LSM9DS1_DATARATE_100;
	LSM9DS1_InitStruct.Axes_Enable = LSM9DS1_X_ENABLE | LSM9DS1_Y_ENABLE | LSM9DS1_Z_ENABLE;
	LSM9DS1_InitStruct.Full_Scale = LSM9DS1_FULLSCALE_2G;
	LSM9DS1_InitStruct.Bandwidth = LSM9DS1_BANDWIDTH_ODR_BASED;
	LSM9DS1_InitStruct.AntiAliasingBandwidth = LSM9DS1_ANTI_ALIASING_BANDWIDTH_408_HZ;
	LSM9DS1_Init(&LSM9DS1_InitStruct);
	
	/* Enable interrupt when data is ready */	
	LSM9DS1_InterruptConfigTypeDef DRYInterruptConfig;
	DRYInterruptConfig.DataReadyInterrupt = LSM9DS1_INTERRUPT_DATA_READY_ON;
	LSM9DS1_InterruptConfig(&DRYInterruptConfig);
	
	printf("Done driver init\n");
	
		/* Set pin as input */
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/* Tell system that you will use PE0 for EXTI_Line0 */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);
	
	/* PE0 is connected to EXTI_Line0 */
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStruct);

	/* Add IRQ vector to NVIC */
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	printf("Done more init\n");
	
	
	
	uint8_t buffer[6] = {1,1,1,1,1,1};
	LSM9DS1_Read(buffer, 0xf, 1);
	print_buffer(buffer, 6);

	LSM9DS1_Read(&(buffer[1]), LSM9DS1_CTRL_REG6_XL_ADDR, 1);
	LSM9DS1_Read(&(buffer[2]), LSM9DS1_CTRL_REG5_XL_ADDR, 1);
	print_buffer(buffer, 6);
	
	test_flag = 0;
	printf("Ready\n");
	
	while (1) {
	 	while (!system_ticks);
		

		if (test_flag == 1) {
			for (uint8_t i = 0; i < 6; i++) {
				LSM9DS1_Read(buffer + i, LSM9DS1_OUT_X_ADDR + i, 1);
			}
			
			int16_t x_raw = buffer[0] + (int16_t)(buffer[1] << 8);
			int16_t y_raw = buffer[2] + (int16_t)(buffer[3] << 8);
			int16_t z_raw = buffer[4] + (int16_t)(buffer[5] << 8);
			
			float x = (float) x_raw;
			float y = (float) y_raw;
			float z = (float) z_raw;
			printf("%d, %d, %d;\n", x_raw, y_raw, z_raw);
			accelerometer_normalize(&x, &y, &z);
//			printf("--> %f, %f, %f;\n", x, y, z);
			
			float pitch = radian_to_degree(atan(x / sqrt(y*y + z*z)));
			float roll = radian_to_degree(atan(y / sqrt(x*x + z*z)));
			printf("Angle = (%f, %f)\n", pitch, roll);
			
			test_flag = 0;
		}
		
	 	system_ticks = 0;
	}
	
	
	return 0;
}


void EXTI0_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {	
		EXTI_ClearITPendingBit(EXTI_Line0);
		test_flag = 1;
	}
}


/*interrupt handler
*/

void SysTick_Handler() {
	system_ticks = 1;
}
