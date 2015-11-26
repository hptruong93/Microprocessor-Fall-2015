#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

#include "modules/led_rotation_sm.h"
#include "system_config.h"
#include "interfaces/cc2500.h"
#include "modules/wireless_transmission_sm.h"
#include "modules/wireless_transmission_protocol_sm.h"

static uint8_t system_ticks;
static uint8_t test[1000];

void print_buffer(uint8_t* buffer, uint8_t num) {
	printf("Buffer is: ");
	for (uint8_t i = 0; i < num; i++) {
		printf("%d - ", buffer[i]);
	}
	printf("\n");
}

static uint8_t packet_test[7] = {65, 66, 67, 68, 69, 70, 71};
void do_send(void) {
	wireless_transmission_periodic();
	uint8_t state = wireless_transmission_get_state();
	if (state == WIRELESS_TRANSMISSION_STATE_IDLE) {
		wireless_transmission_set_send_packet(packet_test, 7);
	} else {
		printf("State is %d\n", state);
	}
}

void do_send2(void) {
	printf("Sending\n");
	
	static uint8_t sent = 0;
	test[0] = CC2500_get_state();

	if (test[0] == CC2500_STATE_TX_UNDERFLOW) {
		printf("Underflow...\n");
		return;
	} else if (test[0] != CC2500_STATE_TX) {
		sent = 0;
		CC2500_read_one(CC2500_STX);
	} else {
		static uint8_t raw_send = 0;
		if (sent == 0) {
			raw_send = (raw_send + 1) % 20;
			uint8_t to_send = raw_send + 'A';
			//CC2500_write_one(&to_send, CC2500_TX_FIFO);
			CC2500_write_tx_one(to_send);
			sent = 1;
		}
 	}
}

int main() {
	test[5] = 1;
	test[6] = 1;
	test[7] = 1;
	
	print_buffer(test, 15);
	wireless_transmission_protocol_encapsulate(test + 5, 3);
	print_buffer(test, 15);
	uint8_t result = wireless_transmission_protocol_checksum(test + 5, test + 5 + 3, 3);
	printf("Done %d\n", result);
	return 0;
	
	SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY);
	CC2500_LowLevel_Init();
	system_init();
	
	printf("Done init\n");
	CC2500_Reset();
	uint8_t part_num = CC2500_get_part_num();
	wireless_transmission_init();

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
