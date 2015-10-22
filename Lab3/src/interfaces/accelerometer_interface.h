#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "stm32f4xx.h"


#define GRAVITY     9.81

typedef struct accelerometer_info {
	float roll;
	float pitch;
	float yaw;
} accelerometer_info;
extern accelerometer_info accelerometer_angles;

void accelerometer_init(void);
void accelerometer_read(float* x, float *y, float *z);
void accelerometer_calculate_rotation(float x, float y, float z);

#endif
