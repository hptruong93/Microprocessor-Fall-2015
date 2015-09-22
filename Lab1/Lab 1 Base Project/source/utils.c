#include <float.h>
#include "utils.h"

/*void 	arm_max_f32 (float32_t *pSrc, uint32_t blockSize, float32_t *pResult, uint32_t *pIndex)*/
int argmax(float* array, int size, float* max_value) {
	if (size == 0) {
		return 0;
	}

	int max_index = 0;
	*max_value = array[0];
	for (int i = 1; i < size; i++) {
		if (array[i] > *max_value) {
			*max_value = array[i];
			max_index = i;
		}
	}

	return max_index;
}