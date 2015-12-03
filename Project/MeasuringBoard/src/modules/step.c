#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "map.h"

#define TOTAL_STEPS 1000
#define PENDING_STEP_COUNT 50
#define LEFT_OR_RIGHT_STEP_THRESHOLD 1100
#define LEFT_STEP_THRESHOLD 600
#define FULL_STEP_THRESHOLD 1100

static bool pending_step = false;
static int pending_step_counter = 0;
static bool pending_step_left = false;
static bool pending_step_full = false;

void step_update(float acc_x) {
    if (pending_step) {
        pending_step_counter += 1;
    }
    
    if (acc_x > LEFT_OR_RIGHT_STEP_THRESHOLD && pending_step_counter == 0) {
        pending_step = true;
    }
    
    if (pending_step_counter > PENDING_STEP_COUNT) {
        pending_step_counter = 0;
        if (pending_step_full) {
            map_step_right();
            map_step_left();
        } else if (pending_step_left) {
            map_step_left();
        } else {
            map_step_right();
        }
        pending_step_left = false;
        pending_step_full = false;
        pending_step = false;
    } else {
        if (acc_x < LEFT_STEP_THRESHOLD) {
            pending_step_left = true;
        }
        if (acc_x > FULL_STEP_THRESHOLD && pending_step_left) {
            pending_step_full = true;
        }
    }
}
