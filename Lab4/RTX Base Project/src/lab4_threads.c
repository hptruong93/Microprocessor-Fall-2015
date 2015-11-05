#include <stdio.h>
#include "cmsis_os.h"
#include "my_types.h"
#include "lab4_threads.h"

#include "interfaces/temperature_sensor_interface.h"
#include "interfaces/accelerometer_interface.h"

#include "modules/led_rotation_sm.h"
#include "modules/accelerometer_sm.h"
#include "modules/seven_segments_sm.h"
#include "modules/keypad_input_sm.h"
#include "modules/temperature_sensor_sm.h"

static const uint8_t KEYPAD_DISPLAY_MODULE_SWITCH = 4;
static const uint8_t KEYPAD_ACCELEROMETER_ROLL = 1;
static const uint8_t KEYPAD_ACCELEROMETER_PITCH = 2;

static const uint8_t DISPLAY_MODULE_TEMPERATURE = 0;
static const uint8_t DISPLAY_MODULE_ACCELEROMETER = 1;
static uint8_t display_module = DISPLAY_MODULE_TEMPERATURE;

static const uint8_t ACCELEROMETER_DISPLAY_ROLL = 0;
static const uint8_t ACCELEROMETER_DISPLAY_PITCH = 1;
static uint8_t accelerometer_display_mode = ACCELEROMETER_DISPLAY_ROLL;

/*******************************************************************************************************/
static osSemaphoreId lock_display_module, lock_accelerator_display_mode;
static osSemaphoreId lock_temperature_data, lock_accelerometer_data;

osSemaphoreDef(lock_display_module);
osSemaphoreDef(lock_accelerator_display_mode);
osSemaphoreDef(lock_temperature_data);
osSemaphoreDef(lock_accelerometer_data);

static uint8_t get_display_module(void) {
	uint8_t result;
	osSemaphoreWait(lock_display_module, osWaitForever);
	result = display_module;
	osSemaphoreRelease(lock_display_module);
	return result;
}

static void set_display_module(uint8_t value) {
	osSemaphoreWait(lock_display_module, osWaitForever);
	display_module = value;
	osSemaphoreRelease(lock_display_module);
}

static uint8_t get_accelerometer_display_mode(void) {
	uint8_t result;
	osSemaphoreWait(lock_accelerator_display_mode, osWaitForever);
	result = accelerometer_display_mode;
	osSemaphoreRelease(lock_accelerator_display_mode);
	return result;
}

static void set_accelerometer_display_mode(uint8_t value) {
	osSemaphoreWait(lock_accelerator_display_mode, osWaitForever);
	accelerometer_display_mode = value;
	osSemaphoreRelease(lock_accelerator_display_mode);
}

/*******************************************************************************************************/

void lab4_os_init(void) {
	lock_display_module = osSemaphoreCreate(osSemaphore(lock_display_module), 1);
	lock_accelerator_display_mode = osSemaphoreCreate(osSemaphore(lock_accelerator_display_mode), 1);

	lock_temperature_data = osSemaphoreCreate(osSemaphore(lock_temperature_data), 1);
	temperature_sensor_set_semaphore(lock_temperature_data);

	lock_accelerometer_data = osSemaphoreCreate(osSemaphore(lock_accelerometer_data), 1);
	accelerometer_set_semaphore(lock_accelerometer_data);
}

void thread_leds(void const* args) {
	led_rotation_rotate_leds();
	osDelay(20);
}

void thread_temperature(void const* args) {
	osSemaphoreWait(lock_temperature_data, osWaitForever);

	temperature_sensor_read_temperature_raw();
	static float temp = 1;
	temperature_sensor_read(&temp);

	if (get_display_module() == DISPLAY_MODULE_TEMPERATURE) {
		seven_segments_set_display_float_smart(temp);
		seven_segment_set_display_degree(TRUE);
	}

	temperature_sensor_alarm(temp);
}

void thread_keypad(void const* args) {
	uint8_t key_pressed = keypad_input_read();
	if (key_pressed == KEYPAD_ACCELEROMETER_ROLL) {
		set_accelerometer_display_mode(ACCELEROMETER_DISPLAY_ROLL);
	} else if (key_pressed == KEYPAD_ACCELEROMETER_PITCH) {
		set_accelerometer_display_mode(ACCELEROMETER_DISPLAY_PITCH);
	} else if (key_pressed == KEYPAD_DISPLAY_MODULE_SWITCH) {
		if (get_display_module() == DISPLAY_MODULE_TEMPERATURE) {
			set_display_module(DISPLAY_MODULE_ACCELEROMETER);
		} else {
			set_display_module(DISPLAY_MODULE_TEMPERATURE);
		}
	}

	osDelay(20);
}

void thread_accelerometer(void const* args) {
	osSemaphoreWait(lock_accelerometer_data, osWaitForever);

	accelerometer_read_raw();
	static accelerometer_info angles;
	accelerometer_calculate_angle(&angles);

	if (get_display_module() == DISPLAY_MODULE_ACCELEROMETER) {
		seven_segment_set_display_degree(TRUE);
		uint8_t display_mode = get_accelerometer_display_mode();
		if (display_mode == ACCELEROMETER_DISPLAY_ROLL) {
			seven_segments_set_display_float_smart(angles.roll < 0 ? -angles.roll : angles.roll);
		} else if (display_mode == ACCELEROMETER_DISPLAY_PITCH) {
			seven_segments_set_display_float_smart(angles.pitch < 0 ? -angles.pitch : angles.pitch);
		}
	}
}
