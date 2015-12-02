#include <stdio.h>
#include <string.h>
#include <math.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

#include "system_config.h"

#include "utils/utils.h"
#include "interfaces/cc2500.h"
#include "modules/led_rotation_sm.h"
#include "modules/commands.h"
#include "modules/protocol_go_back_1.h"

static uint8_t system_ticks;
static uint8_t test[200];
static uint8_t do_wait = 30;

/**
 *	Micro P lab has 34 floor tiles
 *	Each floor tile is 12 inches = 1 foot
 *		calcualted by almost 2 US dollar bills
 *		around 1 + 1/12 sheet of US letter paper
 *
 *	bottom of lab is pixel 316
 *	top of lab is pixel 253
 *
 *	lab is 63 pixels long and 34 feet
 *	each pixel is then 34/63 feet
 *
 *	each pixel is 16.4495238 cm (0.164495238 m)
 *	each pixel is approximately 16.45cm
 *
 *	Each meter is 6.0792033384px = approx. 6.08px
 */
#define M2PX(x) ((int16_t)roundf(6.08f * x))

#define LAB_DOOR_PX_X 163
#define LAB_DOOR_PX_Y 266

void transform_array(int16_t* input, uint16_t* output, uint8_t len) {
	uint8_t i;
	for (i = 0; i < len; i++) {

		/**
		 *	Axes are defined as positive x walking straight from the door
		 *	and positive y heading left from the door
		 *	Taking the pixel location of the door, we can transform the scaling system
		 */
		output[2*i] = M2PX(input[2*i]) + LAB_DOOR_PX_X;
		output[2*i + 1] = LAB_DOOR_PX_Y - M2PX(input[2 * i + 1]);
	
		uint8_t* x = ((uint8_t*)(&output[2*i]));

		for (uint8_t j = 0; j < 4; ++j) {
			if (x[j] == START_PACKET || x[j] == END_PACKET) {
				x[j] = END_PACKET - 1;
			} 
		}
	}
}

static const uint8_t row_count = 2;
static const uint8_t coord_count = 4;
static int16_t sending_coordinates[row_count][2 * coord_count + 1] = {
	{0x00, 0, 0, 2, 0, 2, 4, -12, 4},
	{0x00, -12, 10, 0, 10, 2, 12, 2, 16},
};

void do_send(void) {
	protocol_go_back_1_periodic();
	// uint8_t state = wireless_transmission_get_state();
	// if (state == WIRELESS_TRANSMISSION_STATE_IDLE && sent == 0) {
	// 	wireless_transmission_transmit(packet_test, 7);
	// 	sent = 0;
	// }

	static uint8_t prev = 0;
	uint8_t state = protocol_go_back_1_get_state();
	if (state == GO_BACK_ONE_SENDER_STATE_IDLE) {
		static uint8_t sending_index = 99;
		if (sending_index == 99) {
			printf("Sending CLEAR\n");
			memcpy(test + 1, CLEAR_COMMAND, COMMAND_CLEAR_LEN);
			protocol_go_back_1_send(test + 1, COMMAND_CLEAR_LEN);
			sending_index = 0;
		}	else if (sending_index < row_count) {
			transform_array((&(sending_coordinates[sending_index][1])), (uint16_t*) test, 4);
			protocol_go_back_1_send(test, 8 * sizeof(uint16_t));
			printf("Sending index = %d\n", sending_index);
			sending_index++;
		} else if (sending_index == row_count) {
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
