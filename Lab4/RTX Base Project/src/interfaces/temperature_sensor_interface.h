#ifndef TEMPERATURE_SENSOR_INTERFACE_H
#define TEMPERATURE_SENSOR_INTERFACE_H
#include "stm32f4xx.h"

#define AVERAGE_SLOPE      2.5f
#define V_25               760

void temperature_sensor_init(void);
void temperature_sensor_read_temperature_raw(void);
void temperature_sensor_read(float* temperature);

#endif