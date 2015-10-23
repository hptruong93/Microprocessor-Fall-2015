#include "stm32f4xx_gpio.h"
#include "keypad_interface.h"

static uint8_t keypad_rows[KEYPAD_NUM_PINS_ROW] = {KEYPAD_PIN_1, KEYPAD_PIN_2, KEYPAD_PIN_3, KEYPAD_PIN_4};
static uint8_t keypad_cols[KEYPAD_NUM_PINS_COL] = {KEYPAD_PIN_5, KEYPAD_PIN_6, KEYPAD_PIN_7, KEYPAD_PIN_8};

static const uint16_t all_pins[KEYPAD_NUM_PINS_ROW * KEYPAD_NUM_PINS_COL] = {KEYPAD_PIN_1, KEYPAD_PIN_2, KEYPAD_PIN_3, KEYPAD_PIN_4, KEYPAD_PIN_5, KEYPAD_PIN_6, KEYPAD_PIN_7, KEYPAD_PIN_8};

void keypad_init(void) {
	for (uint8_t i = 0; i < KEYPAD_NUM_PINS_ROW; i++) {
		keypad_rows[i] = 0;
	}

	for (uint8_t i = 0; i < KEYPAD_NUM_PINS_COL; i++) {
		keypad_cols[i] = 0;
	}

	GPIO_InitTypeDef gpio_init_s;
	GPIO_StructInit(&gpio_init_s);
	gpio_init_s.GPIO_Pin = 0;
	for (uint8_t i = 0; i < KEYPAD_NUM_PINS_ROW; i++) {
		gpio_init_s.GPIO_Pin |= keypad_rows[i];
	}
	gpio_init_s.GPIO_Mode = GPIO_Mode_IN;
	gpio_init_s.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_s.GPIO_OType = GPIO_OType_PP;
	gpio_init_s.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(KEYPAD_GPIO, &gpio_init_s);
}

void read_state() {

}
