#include "system_config.h"
#include "stm32f4xx_conf.h"

#include "interfaces/accelerometer_interface.h"
#include "interfaces/led_interface.h"
#include "interfaces/seven_segments_interface.h"
#include "interfaces/keypad_interface.h"

#include "modules/seven_segments_sm.h"

static void generic_init(void) {
	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE); // Enables the AHB1 peripheral clock, providing power to GPIOC branch
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB, ENABLE); // Enables the AHB1 peripheral clock, providing power to GPIOB branch
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOD, ENABLE); // Enables the AHB1 peripheral clock, providing power to GPIOD branch
	
	/* Enable clock for GPIOE */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	/* Enable clock for SYSCFG */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
}

void system_init(void) {
	generic_init();
	led_init();
	seven_segment_init();
	seven_segments_sm_init();
	// keypad_init();
	accelerometer_init();
}

