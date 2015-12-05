#ifndef COORDINATE_DB_H
#define COORDINATE_DB_H

#include "stm32f4xx.h"

#define COORDINATE_DB_SIZE 250
#define COORDINATE_SIZE    2

typedef int16_t COORDINATE_TYPE;

typedef struct {
	COORDINATE_TYPE x;
	COORDINATE_TYPE y;
} coordinate;

void coordinate_db_init(void);
void coordinate_db_clear(void);
uint8_t coordinate_db_insert_entry(COORDINATE_TYPE* input, uint8_t input_len);
uint8_t coordinate_db_get_len(void);
uint8_t coordinate_db_get_entry(uint8_t index, coordinate* result);


#endif
