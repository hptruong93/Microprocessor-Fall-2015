#include <float.h>
#include "utils.h"

int argmax(float* array, int size, float* max_value) {
    printf("Finding argmax of array with size %d\n", size);
    if (size == 0) {
        return 0;
    }

    int max_index = 0;
    *max_value = array[0];
    for (int i = 1; i < size; i++) {
        printf("Considering index %d with value %f and current max is %f\n", i, array[i], *max_value);
        if (array[i] > *max_value) {
            *max_value = array[i];
            max_index = i;
        }
    }

    printf("Found max value %f at index %d\n", *max_value, max_index);

    return max_index;
}


float get_index(float* array2D, int number_of_cols, int i, int j) {
    return array2D[number_of_cols * i + j];
}


void print_array(float* array1D, int number) {
    printf("Printing array of %d elements at %d --> ", number, array1D);
    for (int i = 0; i < number; ++i) {
        printf("%f --> ", array1D[i]); 
    }
    printf("\n");
}
