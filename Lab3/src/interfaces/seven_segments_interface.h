#ifndef SEVEN_SEGMENTS_INTERFACE_H
#define SEVEN_SEGMENTS_INTERFACE_H

#include "stm32f4xx.h"

#define SEVEN_SEGMENT_NUM_DIGIT 4
#define SEVEN_SEGMENT_DIGIT_GPIO GPIOB
#define SEVEN_SEGMENT_DIGIT_1 GPIO_Pin_4 //hardware pin 1
#define SEVEN_SEGMENT_DIGIT_2 GPIO_Pin_5 //hardware pin 2
#define SEVEN_SEGMENT_DIGIT_3 GPIO_Pin_6 //hardware pin 6
#define SEVEN_SEGMENT_DIGIT_4 GPIO_Pin_7 //hardware pin 8

#define SEVEN_SEGMENT_NUM_SEGMENT 7
#define SEVEN_SEGMENT_SEGMENT_GPIO GPIOD
#define SEVEN_SEGMENT_SEGMENT_A GPIO_Pin_1 //hardware pin 14
#define SEVEN_SEGMENT_SEGMENT_B GPIO_Pin_2 //hardware pin 16
#define SEVEN_SEGMENT_SEGMENT_C GPIO_Pin_3 //hardware pin 13
#define SEVEN_SEGMENT_SEGMENT_D GPIO_Pin_4 //hardware pin 3
#define SEVEN_SEGMENT_SEGMENT_E GPIO_Pin_5 //hardware pin 5
#define SEVEN_SEGMENT_SEGMENT_F GPIO_Pin_6 //hardware pin 11
#define SEVEN_SEGMENT_SEGMENT_G GPIO_Pin_7 //hardware pin 15
//Special segment: the dot
#define SEVEN_SEGMENT_SEGMENT_DOT GPIO_Pin_8 //hardware pin 7


void seven_segment_init(void);
void seven_segment_display(uint8_t digit, uint8_t value, uint8_t dot_on);
void seven_segments_clear_segments_at_digit(uint8_t digit);

#endif
