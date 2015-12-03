#ifndef MAP_H
#define MAP_H

#include <stdint.h>

void map_init(void);

void map_step_left(void);

void map_step_right(void);

void map_turn_left(void);

void map_turn_right(void);

int8_t *map_get_next_coordinates(int *length);

void map_terminate_processing(void);

#endif // MAP_H
