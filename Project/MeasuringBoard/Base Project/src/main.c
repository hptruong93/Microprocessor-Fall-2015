#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

#include "system_config.h"

#include "utils/utils.h"
#include "interfaces/cc2500.h"
#include "modules/led_rotation_sm.h"
#include "modules/commands.h"
#include "modules/protocol_go_back_1.h"
// #include "modules/wireless_transmission_sm.h"


static uint8_t system_ticks;
static uint8_t test[100];
static uint8_t do_wait = 30;

static uint8_t packet_test[7] = {65, 66, 67, 68, 69, 70, 71};
static uint8_t sending_coordinates[3][11] = {
	{0x00, 10, 10, 20, 20, 30, 20, 30, 30, 40, 30},
	{0x00, 40, 40, 50, 40, 50, 50, 60, 50, 60, 60},
	{0x00, 75, 75, 100, 120, 120, 140, 150, 150, 160, 170},
};

void do_send(void) {
	static uint8_t sent = 0;
	static uint8_t rotate_mode = LED_ROTATION_MODE_BLINK;
	
	uint8_t temp, temp2;
	protocol_go_back_1_periodic(&temp2);
	// uint8_t state = wireless_transmission_get_state();
	// if (state == WIRELESS_TRANSMISSION_STATE_IDLE && sent == 0) {
	// 	wireless_transmission_transmit(packet_test, 7);
	// 	sent = 0;
	// }

	static uint8_t prev = 0;
	uint8_t state = protocol_go_back_1_get_state();
	if (state == GO_BACK_ONE_SENDER_STATE_IDLE) {
		static uint8_t sending_index = 99;
		//protocol_go_back_1_send(packet_test + 1, 7);
		if (sending_index == 99) {
			printf("Sending CLEAR\n");
			memcpy(test + 1, CLEAR_COMMAND, COMMAND_CLEAR_LEN);
			protocol_go_back_1_send(test + 1, COMMAND_CLEAR_LEN);
			sending_index = 0;
		}	else if (sending_index < 3) {
			protocol_go_back_1_send(sending_coordinates[sending_index] + 1, 10);
			printf("Sending index = %d\n", sending_index);
			sending_index++;
		} else if (sending_index == 3) {
			printf("Sending PLOT\n");
			memcpy(test + 1, PLOT_COMMAND, COMMAND_PLOT_LEN);
			protocol_go_back_1_send(test + 1, COMMAND_PLOT_LEN);
			sending_index++;
		} else {
			sending_index = 99;
			printf("Done\n");
		}
	} else {
		if (state == GO_BACK_ONE_SENDER_STATE_SEND) {
			do_wait = 6;
		} else {
			do_wait = 2;
		}
		
		if (state != prev) {
			prev = state;
			printf("State is %d\n", state);
		}
	}
}

int main() {
	SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY);
	CC2500_LowLevel_Init();
	CC2500_Reset();
	
	system_init();
	
	protocol_go_back_1_init(GO_BACK_ONE_MODE_SENDER);

	while(1) {
		while (!system_ticks);
		led_rotation_rotate_leds();

		static uint8_t count = 0;
		count = (count + 1) % do_wait;
		if (count == 0) {
			do_send();
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
