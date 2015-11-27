#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

#include "system_config.h"

#include "utils/utils.h"
#include "interfaces/cc2500.h"
#include "modules/led_rotation_sm.h"
#include "modules/protocol_go_back_1.h"
// #include "modules/wireless_transmission_sm.h"


static uint8_t system_ticks;
static uint8_t test[1000];

static uint8_t packet_test[7] = {65, 66, 67, 68, 69, 70, 71};
void do_send(void) {
	static uint8_t sent = 0;
	static uint8_t rotate_mode = LED_ROTATION_MODE_BLINK;

	uint8_t temp, temp2;
	wireless_transmission_periodic(&temp);
	uint8_t state = wireless_transmission_get_state();
	if (state == WIRELESS_TRANSMISSION_STATE_IDLE && sent == 0) {
		wireless_transmission_transmit(packet_test, 4);
		sent = 0;
	}
}

int main() {
	SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY);
	CC2500_LowLevel_Init();
	system_init();
	
	CC2500_Reset();
	uint8_t part_num = CC2500_get_part_num();
	wireless_transmission_init();
	protocol_go_back_1_init(GO_BACK_ONE_MODE_SENDER);


	while(1) {
		while (!system_ticks);
		led_rotation_rotate_leds();

		static uint8_t count = 0;
		count = (count + 1) % 30;
		if (count == 2) {
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
