<<<<<<< HEAD
#include <stdio.h>
#include "modules/lab3_game.h"
#include "my_types.h"

#include "interfaces/led_interface.h"
#include "modules/accelerometer_sm.h"
#include "modules/seven_segments_sm.h"
#include "modules/keypad_input_sm.h"

void print_instruction(void) {
	printf("Guess the roll angle. Press * to enter...\n");
}

#define READ_INPUT		1
#define GUESS			2

static uint8_t buffer[100];
static uint16_t buffer_index = 0;

static uint16_t get_user_guess(void) {
	uint16_t value = 0;
	uint16_t index = 0;

	while (TRUE) {
		value += buffer[index];
		if (index == buffer_index) {
			break;
		}
		value *= 10;
		index++;
	}
	return value;
}

static void user_press(uint8_t value) {
	buffer_index++;
	buffer[buffer_index] = value;
}

void gg(void) {
	static uint8_t game_over_correct = FALSE, game_over_incorrect = FALSE;
	static uint8_t state = READ_INPUT;
	static uint16_t try_left = LAB3_GAME_MAX_TRY;

	float angle = accelerometer_calculate_angle();
	angle = angle < 0 ? -angle : angle;

	if (game_over_correct) {
		led_all_off();
		SET_LED(LED_GREEN);
		seven_segments_set_display_float_smart(angle);
	} else if (game_over_incorrect) {
		led_all_off();
		SET_LED(LED_ORANGE);
		seven_segments_set_display_float_smart(angle);
	} else {
		if (state == READ_INPUT) {
			uint8_t pressed = keypad_input_read();
			if (pressed == KEYPAD_INPUT_STAR) {
				state = GUESS;
			} else if (pressed != KEYPAD_INPUT_NO_PRESS) {
				user_press(pressed);
			}
			seven_segment_set_display_int(get_user_guess(), SEVEN_SEGMENT_BASE_10);
		} else if (state == GUESS) {
			float user_guess = (float) get_user_guess();
			buffer_index = 0;

			float diff = user_guess - angle;
			diff = diff < 0 ? -diff : diff;
			printf("Value is %f\n", user_guess);
			if (diff < LAB3_GAME_BOUND) {
				game_over_correct = TRUE;
				printf("Guess correct!!!\n");
			} else {
				if (try_left == 0) {
					game_over_incorrect = TRUE;
				} else {
					state = READ_INPUT;
					try_left--;

					led_all_off();
					if (user_guess < angle) {
						printf("Less than expected\n");
						SET_LED(LED_RED);
					} else {
						printf("More than expected\n");
						SET_LED(LED_BLUE);
					}
				}
			}
		}
		
	}
}
=======
#include <stdio.h>
#include "modules/lab3_game.h"
#include "my_types.h"

#include "interfaces/led_interface.h"
#include "modules/accelerometer_sm.h"
#include "modules/seven_segments_sm.h"
#include "modules/keypad_input_sm.h"

void print_instruction(void) {
	printf("Guess the roll angle. Press * to enter...\n");
}

#define READ_INPUT		1
#define GUESS			2

static uint8_t buffer[100];
static uint16_t buffer_index = 0;

static uint16_t get_user_guess(void) {
	uint16_t value = 0;
	uint16_t index = 0;

	while (TRUE) {
		value += buffer[index];
		if (index == buffer_index) {
			break;
		}
		value *= 10;
		index++;
	}
	return value;
}

static void user_press(uint8_t value) {
	buffer_index++;
	buffer[buffer_index] = value;
}

void gg(void) {
	static uint8_t game_over = FALSE;
	static uint8_t state = READ_INPUT;
	static uint16_t try_left = LAB3_GAME_MAX_TRY;

	float angle = accelerometer_calculate_angle();
	angle = angle < 0 ? -angle : angle;

	if (game_over) {
		led_all_on();
		seven_segments_set_display_float_smart(angle);
	} else {
		if (state == READ_INPUT) {
			uint8_t pressed = keypad_input_read();
			if (pressed == KEYPAD_INPUT_STAR) {
				state = GUESS;
			} else if (pressed != KEYPAD_INPUT_NO_PRESS) {
				user_press(pressed);
			}
			seven_segment_set_display_int(get_user_guess(), SEVEN_SEGMENT_BASE_10);
		} else if (state == GUESS) {
			float user_guess = (float) get_user_guess();
			buffer_index = 0;

			float diff = user_guess - angle;
			diff = diff < 0 ? -diff : diff;
			printf("Value is %f\n", user_guess);
			if (diff < LAB3_GAME_BOUND) {
				game_over = TRUE;
				printf("Guess correct!!!\n");
			} else {
				if (try_left == 0) {
					game_over = TRUE;
				} else {
					state = READ_INPUT;
					try_left--;

					led_all_off();
					if (user_guess < angle) {
						printf("Less than expected\n");
						SET_LED(LED_4);
					} else {
						printf("More than expected\n");
						SET_LED(LED_3);
					}
				}
			}
		}
		
	}
}
>>>>>>> origin/master
