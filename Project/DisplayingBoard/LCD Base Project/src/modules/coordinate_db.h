#ifndef COORDINATE_DB_H
#define COORDINATE_DB_H

#include "stm32f4xx.h"

#define COORDINATE_DB_SIZE 100
#define COORDINATE_SIZE    2

typedef struct {
	uint8_t x;
	uint8_t y;
} coordinate;

void coordinate_db_init(void);
void coordinate_db_clear(void);
uint8_t coordinate_db_insert_entry(uint8_t* input, uint8_t input_len);
uint8_t coordinate_db_get_len(void);
uint8_t coordinate_db_get_entry(uint8_t index, coordinate* result);


#endif
