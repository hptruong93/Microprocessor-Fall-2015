#include <stdio.h>
#include <stdbool.h>

#include "stm32f4xx.h"

#include "drivers/lsm9ds1.h"
#include "modules/map.h"
#include "modules/step.h"
#include "modules/turn.h"
#include "sensors/acc.h"
#include "sensors/gyro.h"

/**
 * Flag to indicate when the SysTick timer ticks.
 */
bool has_ticked = false;

int c = 0;

/**
 * Initialize all system components.
 */
void init() {
    // Set the frequency of the SysTick interrupt to 50 Hz
	SysTick_Config(SystemCoreClock / 50);

    // Initialize accelerometer
    acc_init();
	// Initialize gyroscope
    gyro_init();

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
            step_update(acc_get_x());
            turn_update(gyro_get_x());
            
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
