#include <string.h>
#include "coordinate_db.h"

#include "my_types.h"

static uint8_t db[COORDINATE_DB_SIZE][2];
static uint8_t next_index;

void coordinate_db_init(void) {
	next_index = 0;
}

void coordinate_db_clear(void) {
	next_index = 0;
}

uint8_t coordinate_db_insert_entry(uint8_t* input, uint8_t input_len) {
	if (next_index == COORDINATE_DB_SIZE) {
		return FALSE;
	}

	if (input_len % 2 != 0) {
		return FALSE;
	}

	memcpy(&(db[next_index][0]), input, input_len);
	next_index += input_len / 2;
	return TRUE;
}

uint8_t coordinate_db_get_len(void) {
	return next_index;
}

uint8_t coordinate_db_get_entry(uint8_t index, coordinate* result) {
	if (index < next_index) {
		memcpy((uint8_t*) result, &(db[index][0]), 2);
		return TRUE;
	}

	return FALSE;
}
