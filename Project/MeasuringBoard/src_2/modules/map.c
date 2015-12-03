#include <stdio.h>

static int left_step_count = 0;
static int right_step_count = 0;
static int left_turn_count = 0;
static int right_turn_count = 0;

void map_step_left(void) {
    printf("Left step %d.\n", left_step_count++);
}

void map_step_right(void) {
    printf("Right step %d.\n", right_step_count++);
}

void map_turn_left(void) {
    printf("Left turn %d.\n", left_turn_count++);
}

void map_turn_right(void) {
    printf("Right turn %d.\n", right_turn_count++);
}
