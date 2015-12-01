#include <string.h>
#include "coordinate_db.h"

#include "my_types.h"

static COORDINATE_TYPE db[COORDINATE_DB_SIZE][2];
static uint8_t next_index;

void coordinate_db_init(void) {
	next_index = 0;
}

void coordinate_db_clear(void) {
	next_index = 0;
}

uint8_t coordinate_db_insert_entry(COORDINATE_TYPE* input, uint8_t input_len) {
	if (next_index == COORDINATE_DB_SIZE) {
		return FALSE;
	}

	if (input_len == 0 || input_len % 2 != 0) {
		return FALSE;
	}

	memcpy((uint8_t*) &(db[next_index][0]), input, sizeof(COORDINATE_TYPE) * input_len);

	next_index += input_len / 2;
	return TRUE;
}

uint8_t coordinate_db_get_len(void) {
	return next_index;
}

uint8_t coordinate_db_get_entry(uint8_t index, coordinate* result) {
	if (index < next_index) {
		memcpy((uint8_t*) result, &(db[index][0]), 2 * sizeof(COORDINATE_TYPE));
		return TRUE;
	}

	return FALSE;
}
