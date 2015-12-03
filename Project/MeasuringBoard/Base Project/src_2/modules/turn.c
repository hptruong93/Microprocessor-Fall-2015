#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "map.h"

#define LEFT_TURN_THRESHOLD 65000
#define PENDING_LEFT_TURN_COUNT 50
#define RIGHT_TURN_THRESHOLD -65000
#define PENDING_RIGHT_TURN_COUNT 50

static bool pending_left_turn = false;
static int pending_left_turn_counter = 0;
static bool pending_right_turn = false;
static int pending_right_turn_counter = 0;

void turn_update(float gyro_x) {
    if (pending_left_turn) {
        pending_left_turn_counter += 1;
    }
    if (pending_right_turn) {
        pending_right_turn_counter += 1;
    }
    
    if (gyro_x > LEFT_TURN_THRESHOLD && pending_left_turn_counter == 0) {
        pending_left_turn = true;
    }
    if (gyro_x < RIGHT_TURN_THRESHOLD && pending_right_turn_counter == 0) {
        pending_right_turn = true;
    }
    
    if (pending_left_turn_counter > PENDING_LEFT_TURN_COUNT) {
        map_turn_left();
        pending_left_turn_counter = 0;
        pending_left_turn = false;   
    }
    if (pending_right_turn_counter > PENDING_RIGHT_TURN_COUNT) {
        map_turn_right();
        pending_right_turn_counter = 0;
        pending_right_turn = false;   
    }
}
