#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

#include "modules/led_rotation_sm.h"
#include "system_config.h"
#include "interfaces/cc2500.h"

static uint8_t system_ticks;
static uint8_t test[1000];

void print_buffer(uint8_t* buffer, uint8_t num) {
	printf("Buffer is: ");
	for (uint8_t i = 0; i < num; i++) {
		printf("%d - ", buffer[i]);
	}
	printf("\n");
}

int main() {
	printf("Here\n");
	SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY);
	system_init();
	
	CC2500_LowLevel_Init();
	printf("Done init\n");
	
	CC2500_Read(test, 0x30, 2);
	printf("Read the thing\n");
	print_buffer(test, 1);
	
	while(1){
		while (!system_ticks);
		led_rotation_rotate_leds();
		system_ticks = 0;
	}
	
	return 0;
}

/*interrupt handler
*/

void SysTick_Handler() {
	system_ticks = 1;
}
