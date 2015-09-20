#ifndef UTILS_H
#define UTILS_H

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
int matrix_multiplication(void* matrix_a, int row_count_a, int col_count_a, void* matrix_b, int row_count_b, int col_count_b, void* result);

#endif
