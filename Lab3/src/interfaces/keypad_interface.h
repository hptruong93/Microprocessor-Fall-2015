#ifndef KEYPAD_INTERFACE_H
#define KEYPAD_INTERFACE_H

#include "stm32f4xx.h"

#define KEYPAD_NUM_PINS_ROW				4
#define KEYPAD_NUM_PINS_COL				4

#define KEYPAD_GPIO 					GPIOA
#define KEYPAD_PIN_1					GPIO_Pin_0
#define KEYPAD_PIN_2					GPIO_Pin_13
#define KEYPAD_PIN_3					GPIO_Pin_10
#define KEYPAD_PIN_4					GPIO_Pin_3
#define KEYPAD_PIN_5					GPIO_Pin_4
#define KEYPAD_PIN_6					GPIO_Pin_5
#define KEYPAD_PIN_7					GPIO_Pin_6
#define KEYPAD_PIN_8					GPIO_Pin_7

#define KEYPAD_ROW_1					KEYPAD_PIN_1
#define KEYPAD_ROW_2					KEYPAD_PIN_2
#define KEYPAD_ROW_3					KEYPAD_PIN_3
#define KEYPAD_ROW_4					KEYPAD_PIN_4
#define KEYPAD_COL_1					KEYPAD_PIN_5
#define KEYPAD_COL_2					KEYPAD_PIN_6
#define KEYPAD_COL_3					KEYPAD_PIN_7
#define KEYPAD_COL_4					KEYPAD_PIN_8

void keypad_init(void);
void keypad_read_state(void);

#endif
