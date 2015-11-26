#include <stdio.h>

#include "stm32f4xx.h"

#include "sensors/acc/acc.h"
#include "sensors/gyro/gyro.h"
#include "drivers/lsm9ds1/lsm9ds1.h"

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
            float x = gyro_get_y();
            if (c == 0) {
                printf("%f %f %f %f %f %f\n", acc_get_x(), acc_get_y(),
                    acc_get_z(), gyro_get_x(), gyro_get_y(), gyro_get_z());
            }
            //c = (c + 1) % 10;
            
            if (x < 0.1f) {
                gyro_init();
                gyro_update();
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
