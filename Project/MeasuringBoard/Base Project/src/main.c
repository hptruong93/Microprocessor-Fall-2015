#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

#include "modules/led_rotation_sm.h"
#include "system_config.h"
#include "interfaces/cc2500.h"
#include "interfaces/cc2500_settings.h"

static uint8_t system_ticks;
static uint8_t test[1000];

void print_buffer(uint8_t* buffer, uint8_t num) {
	printf("Buffer is: ");
	for (uint8_t i = 0; i < num; i++) {
		printf("%d - ", buffer[i]);
	}
	printf("\n");
}

void write_the_value(uint8_t value, uint8_t addr) {
	uint8_t temp = value;
	CC2500_Write(&temp, addr, 1);
}

void write_config() {
	for (uint8_t i = 0; i < CC2500_CONFIG_COUNT; i++) {
		write_the_value(cc2500_config[i][0], cc2500_config[i][1]);
	}
}

int main() {
	printf("Here\n");
	SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY);
	CC2500_LowLevel_Init();
	system_init();
	
	printf("Done init\n");
	CC2500_Read(test, 0x30, 1);
	CC2500_Read(test + 1, 0x30, 2);
	printf("Read the thing\n");
	print_buffer(test, 3);
	
	write_config();
	
	CC2500_Read(test, 0x34, 1); //SRX
	print_buffer(test, 1);
	
	while(1){
		while (!system_ticks);
		led_rotation_rotate_leds();
		
		CC2500_Read(test, 0x34, 1); //SRX
		CC2500_Read(test + 1, 0x35, 2); //MARCSTATE
		CC2500_Read(test + 3, 0x3b, 2); //Rx bytes
		
		if (*(test + 3) > 0) {
			CC2500_Read(test + 5, 0x3f, 1); //Read rx fifo
			print_buffer(test, 7);
		} else {
			//print_buffer(test, 5);
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
