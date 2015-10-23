#include "seven_segments_interface.h"

static const uint16_t all_digits[SEVEN_SEGMENT_NUM_DIGIT] = {SEVEN_SEGMENT_DIGIT_1, SEVEN_SEGMENT_DIGIT_2, SEVEN_SEGMENT_DIGIT_3, SEVEN_SEGMENT_DIGIT_4};
static const uint16_t all_segments[SEVEN_SEGMENT_NUM_SEGMENT] = {SEVEN_SEGMENT_SEGMENT_A, SEVEN_SEGMENT_SEGMENT_B, SEVEN_SEGMENT_SEGMENT_C, SEVEN_SEGMENT_SEGMENT_D, SEVEN_SEGMENT_SEGMENT_E, SEVEN_SEGMENT_SEGMENT_F, SEVEN_SEGMENT_SEGMENT_G};

void seven_segment_init(void) {
	GPIO_InitTypeDef gpio_init_s;
	GPIO_StructInit(&gpio_init_s);
	//The segments
	gpio_init_s.GPIO_Pin = 0;
	for (uint8_t i = 0; i < SEVEN_SEGMENT_NUM_SEGMENT; i++) {
		gpio_init_s.GPIO_Pin |= all_segments[i];
	}
	gpio_init_s.GPIO_Pin |= SEVEN_SEGMENT_SEGMENT_DOT;

	gpio_init_s.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_s.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_s.GPIO_OType = GPIO_OType_PP;
	gpio_init_s.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(SEVEN_SEGMENT_SEGMENT_GPIO, &gpio_init_s);

	//The digits
	gpio_init_s.GPIO_Pin = 0;
	for (uint8_t i = 0; i < SEVEN_SEGMENT_NUM_DIGIT; i++) {
		gpio_init_s.GPIO_Pin |= all_digits[i];
	}
	GPIO_Init(SEVEN_SEGMENT_DIGIT_GPIO, &gpio_init_s);
}

static void choose_digit(uint8_t digit) {
	//Configure the digit first by setting it to low, and everything else to high
	for (uint8_t i = 0; i < SEVEN_SEGMENT_NUM_DIGIT; i++) {
		if (all_digits[i] == digit) {
			GPIO_ResetBits(SEVEN_SEGMENT_DIGIT_GPIO, digit);
		} else {
			GPIO_SetBits(SEVEN_SEGMENT_DIGIT_GPIO, all_digits[i]);
		}
	}
}

static void clear_segments(void) {
	//Clear all segments
	for (uint8_t i = 0; i < SEVEN_SEGMENT_NUM_SEGMENT; i++) {
		GPIO_ResetBits(SEVEN_SEGMENT_SEGMENT_GPIO, all_segments[i]);
	}
}

void seven_segments_clear_segments_at_digit(uint8_t digit) {
	choose_digit(digit);
	clear_segments();
}

void seven_segment_display(uint8_t digit, uint8_t value, uint8_t dot_on) {
	choose_digit(digit);
	clear_segments();
	
	static const uint16_t values[16][SEVEN_SEGMENT_NUM_SEGMENT] = {
	//   A  B  C  D  E  F  G
		{1, 1, 1, 1, 1, 1, 0}, //0
		{0, 1, 1, 0, 0, 0, 0}, //1
		{1, 1, 0, 1, 1, 0, 1}, //2
		{1, 1, 1, 1, 0, 0, 1}, //3
		{0, 1, 1, 0 ,0 ,1, 1}, //4
		{1, 0, 1, 1 ,0 ,1, 1}, //5
		{1, 0, 1, 1 ,1 ,1, 1}, //6
		{1, 1, 1, 0 ,0 ,0, 0}, //7
		{1, 1, 1, 1 ,1 ,1, 1}, //8
		{1, 1, 1, 1 ,0 ,1, 1}, //9
		{1, 1, 1, 0 ,1 ,1, 1}, //10 - A
		{0, 0, 1, 1 ,1 ,1, 1}, //11 - B
		{1, 0, 0, 1, 1, 1, 0}, //12 - C
		{0, 1, 1, 1 ,1 ,0, 1}, //13 - D
		{1, 0, 0, 1 ,1 ,1, 1}, //14 - E
		{1, 0, 0, 0 ,1 ,1, 1}, //15 - F
	};

	if (value > 15) {
		return;
	}

	//Set the appropriate segment
	for (uint8_t i = 0; i < SEVEN_SEGMENT_NUM_SEGMENT; i++) {
		if (values[value][i] == 1) {
			GPIO_SetBits(SEVEN_SEGMENT_SEGMENT_GPIO, all_segments[i]);
		}
	}

	if (dot_on) {
		GPIO_SetBits(SEVEN_SEGMENT_SEGMENT_GPIO, SEVEN_SEGMENT_SEGMENT_DOT);
	} else {
		GPIO_ResetBits(SEVEN_SEGMENT_SEGMENT_GPIO, SEVEN_SEGMENT_SEGMENT_DOT);
	}
}
