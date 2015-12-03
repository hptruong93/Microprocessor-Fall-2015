#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>

#include "stm32f4xx.h"

#include "drivers/lsm9ds1.h"
#include "modules/map.h"
#include "modules/step.h"
#include "modules/turn.h"
#include "sensors/acc.h"
#include "sensors/gyro.h"

#include "utils/utils.h"
#include "interfaces/cc2500.h"
#include "interfaces/led_interface.h"
#include "modules/led_rotation_sm.h"
#include "modules/commands.h"
#include "modules/protocol_go_back_1.h"

/**
 * Flag to indicate when the SysTick timer ticks.
 */
bool has_ticked = false;


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


static int16_t test_michael[] =  {	0, 0, 0,
								2, 0,
								2, 5,
							  -10, 5,
							  -10, 9,
							    0, 9	};
static const uint8_t ready = 1;

int8_t *next_coordinates;
int length;

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
		} else if (sending_index < ready) {
   //          map_terminate_processing();
			// next_coordinates = map_get_next_coordinates(&length);
			// if (next_coordinates == NULL) {
			// 	sending_index = ready;
			// } else {
			// 	printf("Length: %d\n", length);
			// 	printf("Coordinates to send: ");
			// 	for (uint8_t i = 0; i < length; i++) {
			// 		printf("%" PRIi8 " - ", next_coordinates[i]);
			// 	}	
			// 	printf("\n");
			// 	//transform_array(next_coordinates, (uint16_t*) test, length);
			// 	protocol_go_back_1_send(test, length);
			// }
			protocol_go_back_1_send((uint8_t*) (&test_michael[1]), 12);
			sending_index = ready;

		} else if (sending_index == ready) {
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
			do_wait = 3;
		} else {
			do_wait = 1;
		}
		
		if (state != prev) {
			prev = state;
			printf("State is %d\n", state);
		}
	}
}

/**
 * Initialize all system components.
 */
void init() {
    // Set the frequency of the SysTick interrupt to 50 Hz
	SysTick_Config(SystemCoreClock / 50);

	led_init();
	CC2500_LowLevel_Init();
	CC2500_Reset();
	
	protocol_go_back_1_init(GO_BACK_ONE_MODE_SENDER);
	
    // Initialize accelerometer
//    acc_init();
//	// Initialize gyroscope
//    gyro_init();
//    // Initialize map
//    map_init();

//    // Update accelerometer once manually to enable interrupt
//    acc_update();
//	// Update gyroscope once manually to enable interrupt
//	gyro_update();
}

/**
 * Main function of application.
 */
int main() {
    // Initialize all system components
    init();
    
    // Application loop
    while (1) {
//        // Interrupts
//        if (acc_interrupt) {
//            acc_update();
//        }
//		if (gyro_interrupt) {
//            gyro_update();
//        }

		if (has_ticked) {
			led_rotation_rotate_leds();

//            step_update(acc_get_x());
//            turn_update(gyro_get_x());
            
			static uint8_t count = 0;
			count = (count + 1) % do_wait;
			if (count == 0) {
				do_send();
			}
							
            has_ticked = false;
			
        }
    }
}

/**
 * Handler for SysTick timer.
 */
void SysTick_Handler() {
	has_ticked = true;
}
