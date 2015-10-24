#include <math.h>
#include "my_types.h"
#include "utils/utils.h"

inline float radian_to_degree(float rad) {
	return rad * (180.0 / M_PI);
}

inline uint16_t array_or_const(const uint16_t* array, uint16_t length) {
	uint16_t result = 0;
	for (uint16_t i = 0; i < length; i++) {
		result |= array[i];
	}
	return result;
}

inline uint16_t array_or(uint16_t* array, uint16_t length) {
	uint16_t result = 0;
	for (uint16_t i = 0; i < length; i++) {
		result |= array[i];
	}
	return result;
}
