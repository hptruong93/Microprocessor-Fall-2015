#include "stm32f4xx_gpio.h"
#include "keypad_interface.h"

static const uint16_t keypad_rows[KEYPAD_NUM_PINS_ROW] = {KEYPAD_ROW_1, KEYPAD_ROW_2, KEYPAD_ROW_3, KEYPAD_ROW_4};
static const uint16_t keypad_cols[KEYPAD_NUM_PINS_COL] = {KEYPAD_COL_1, KEYPAD_COL_2, KEYPAD_COL_3, KEYPAD_COL_4};

static const uint16_t all_pins[KEYPAD_NUM_PINS_ROW * KEYPAD_NUM_PINS_COL] = {KEYPAD_PIN_1, KEYPAD_PIN_2, KEYPAD_PIN_3, KEYPAD_PIN_4, KEYPAD_PIN_5, KEYPAD_PIN_6, KEYPAD_PIN_7, KEYPAD_PIN_8};

static uint8_t row = 0, col = 0;
static uint8_t is_reading_col = 0;
static uint8_t expected_pressed_value = 1;

#define KEYPAD_READ_GPIO(keypad_pin) GPIO_ReadInputDataBit(KEYPAD_GPIO, keypad_pin)

static void configure_gpio_input(GPIO_InitTypeDef* gpio_init_s, const uint16_t* pins, uint8_t number_of_pins) {
	GPIO_StructInit(gpio_init_s);
	gpio_init_s->GPIO_Pin = 0;
	for (uint8_t i = 0; i < number_of_pins; i++) {
		gpio_init_s->GPIO_Pin |= pins[i];
	}
	// printf("Calculated %d\n", gpio_init_s->GPIO_Pin);
	gpio_init_s->GPIO_Mode = GPIO_Mode_IN;
	gpio_init_s->GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_s->GPIO_OType = GPIO_OType_PP;
	gpio_init_s->GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(KEYPAD_GPIO, gpio_init_s);
}

static void configure_gpio_output(GPIO_InitTypeDef* gpio_init_s, const uint16_t* pins, uint8_t number_of_pins) {
	GPIO_StructInit(gpio_init_s);
	gpio_init_s->GPIO_Pin = 0;
	for (uint8_t i = 0; i < number_of_pins; i++) {
		gpio_init_s->GPIO_Pin |= pins[i];
	}
	// printf("Calculated %d\n", gpio_init_s->GPIO_Pin);
	gpio_init_s->GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_s->GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_s->GPIO_OType = GPIO_OType_PP;
	gpio_init_s->GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEYPAD_GPIO, gpio_init_s);
}

void keypad_init(void) {
	GPIO_InitTypeDef gpio_init_s;
	if (is_reading_col) {
		printf("Setting columns\n");
		configure_gpio_input(&gpio_init_s, keypad_rows, KEYPAD_NUM_PINS_ROW);
		printf("Expected %d\n", KEYPAD_ROW_1 | KEYPAD_ROW_2 | KEYPAD_ROW_3 | KEYPAD_ROW_4);
		configure_gpio_output(&gpio_init_s, keypad_cols, KEYPAD_NUM_PINS_COL);
		printf("Expected %d\n", KEYPAD_COL_1 | KEYPAD_COL_2 | KEYPAD_COL_3 | KEYPAD_COL_4);
		GPIO_SetBits(KEYPAD_GPIO, KEYPAD_COL_1);
		GPIO_SetBits(KEYPAD_GPIO, KEYPAD_COL_2);
		GPIO_SetBits(KEYPAD_GPIO, KEYPAD_COL_3);
		GPIO_SetBits(KEYPAD_GPIO, KEYPAD_COL_4);
	} else {
		printf("Setting rows\n");
		configure_gpio_output(&gpio_init_s, keypad_rows, KEYPAD_NUM_PINS_ROW);
		printf("Expected %d\n", KEYPAD_ROW_1 | KEYPAD_ROW_2 | KEYPAD_ROW_3 | KEYPAD_ROW_4);
		configure_gpio_input(&gpio_init_s, keypad_cols, KEYPAD_NUM_PINS_COL);
		printf("Expected %d\n", KEYPAD_COL_1 | KEYPAD_COL_2 | KEYPAD_COL_3 | KEYPAD_COL_4);
		// GPIO_SetBits(KEYPAD_GPIO, KEYPAD_ROW_1);
		// GPIO_SetBits(KEYPAD_GPIO, KEYPAD_ROW_2);
		// GPIO_SetBits(KEYPAD_GPIO, KEYPAD_ROW_3);
		// GPIO_SetBits(KEYPAD_GPIO, KEYPAD_ROW_4);
	}
}

static void switch_readings(void) {
	static GPIO_InitTypeDef gpio_init_s;

	if (is_reading_col == 1) {
		configure_gpio_output(&gpio_init_s, keypad_rows, KEYPAD_NUM_PINS_ROW);
		configure_gpio_input(&gpio_init_s, keypad_cols, KEYPAD_NUM_PINS_COL);
	} else {
		configure_gpio_input(&gpio_init_s, keypad_rows, KEYPAD_NUM_PINS_ROW);
		configure_gpio_output(&gpio_init_s, keypad_cols, KEYPAD_NUM_PINS_COL);

	}

	is_reading_col = 1 - is_reading_col;
}

void keypad_read_state(void) {
	if (is_reading_col) {
		for (uint8_t i = 0; i < KEYPAD_NUM_PINS_COL; i++) {
			uint8_t pin_state = KEYPAD_READ_GPIO(keypad_rows[i]);
			if (pin_state == expected_pressed_value) {
				printf("Col %d is pressed\n", i);
				break;
			}
		}

		// uint8_t row1 = KEYPAD_READ_GPIO(KEYPAD_ROW_1);
		// uint8_t row2 = KEYPAD_READ_GPIO(KEYPAD_ROW_2);
		// uint8_t row3 = KEYPAD_READ_GPIO(KEYPAD_ROW_3);
		// uint8_t row4 = KEYPAD_READ_GPIO(KEYPAD_ROW_4);

		// printf("Rows are %d, %d, %d, %d\n", row1, row2, row3, row4);
	} else {
		uint8_t col1 = KEYPAD_READ_GPIO(KEYPAD_COL_1);
		uint8_t col2 = KEYPAD_READ_GPIO(KEYPAD_COL_2);
		uint8_t col3 = KEYPAD_READ_GPIO(KEYPAD_COL_3);
		uint8_t col4 = KEYPAD_READ_GPIO(KEYPAD_COL_4);

		printf("Cols are %d, %d, %d, %d\n", col1, col2, col3, col4);
	}
	// switch_readings();
}
