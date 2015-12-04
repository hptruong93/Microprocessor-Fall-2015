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
#include "interfaces/acc.h"
#include "interfaces/gyro.h"

#include "utils/utils.h"
#include "drivers/cc2500.h"
#include "interfaces/led_interface.h"
#include "modules/led_rotation_sm.h"
#include "modules/commands.h"
#include "modules/protocol_go_back_1.h"

/**
 * Flag to indicate when the SysTick timer ticks.
 */
bool has_ticked = false;

#define MAX_COORDINATES_TO_SEND 20

static uint8_t wireless_temp[200];
static int16_t next_coordinates[MAX_COORDINATES_TO_SEND * 2 + 1];
static uint8_t do_wait = 30;

static const uint8_t ready = 1;

void do_send(void) {
	protocol_go_back_1_periodic();

	static uint8_t prev = 0;
	uint8_t state = protocol_go_back_1_get_state();
	if (state == GO_BACK_ONE_SENDER_STATE_IDLE) {
		static uint8_t sending_index = 99;
		if (sending_index == 99) {
			printf("Sending CLEAR\n");
			memcpy(wireless_temp + 1, CLEAR_COMMAND, COMMAND_CLEAR_LEN);
			protocol_go_back_1_send(wireless_temp + 1, COMMAND_CLEAR_LEN);
			sending_index = 0;
		} else if (sending_index < ready) {
            uint16_t length;
            map_terminate_processing();
            int8_t ret = map_get_next_coordinates(next_coordinates + 1, &length, 
                MAX_COORDINATES_TO_SEND * 2);
            if (!ret) {
                sending_index = ready;
            } else {
                printf("Length: %d\n", length);
                printf("Coordinates to send: ");
                for (uint16_t i = 1; i <= length; i++) {
                    printf("%" PRIi16 " - ", next_coordinates[i]);
                }	
                printf("\n");
                protocol_go_back_1_send((uint8_t *) (next_coordinates + 1), 
                    length * sizeof(int16_t));
            }
		} else if (sending_index == ready) {
			printf("Sending PLOT\n");
			memcpy(wireless_temp + 1, PLOT_COMMAND, COMMAND_PLOT_LEN);
			protocol_go_back_1_send(wireless_temp + 1, COMMAND_PLOT_LEN);
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
    acc_init();
	// Initialize gyroscope
    gyro_init();
    // Initialize map
    map_init();

    // Update accelerometer once manually to enable interrupt
    acc_update();
	// Update gyroscope once manually to enable interrupt
	gyro_update();
}

/**
 * Main function of application.
 */
int main() {
    // Initialize all system components
    init();
    
    // Application loop
    while (1) {
        // Interrupts
        if (acc_interrupt) {
            acc_update();
        }
		if (gyro_interrupt) {
            gyro_update();
        }

		if (has_ticked) {
			led_rotation_rotate_leds();

            step_update(acc_get_x());
            turn_update(gyro_get_x());
            
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
