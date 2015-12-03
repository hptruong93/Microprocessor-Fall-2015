#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define MAX_COORDINATES 1000
#define COORDINATES_ARRAY_SIZE MAX_COORDINATES * 2 + 1

typedef enum orientation {
    POS_X,
    POS_Y,
    NEG_X,
    NEG_Y
} orientation_type;

static orientation_type current_orientation = POS_X;

static int left_step_count = 0;
static int right_step_count = 0;
static int left_turn_count = 0;
static int right_turn_count = 0;

static int8_t coordinates[COORDINATES_ARRAY_SIZE];
static int coordinates_index = 3;
static int retrieved_coordinates_index = 1;

static bool terminate_processing = false;

void map_init(void) {
    memset(coordinates, 0, COORDINATES_ARRAY_SIZE * sizeof(int8_t));
}

void map_step(void) {
    switch (current_orientation) {
        case POS_X:
            coordinates[coordinates_index] = 
                coordinates[coordinates_index - 2] + 1;
            coordinates[coordinates_index + 1] = 
                coordinates[coordinates_index - 1];
            break;
        case POS_Y:
            coordinates[coordinates_index] = 
                coordinates[coordinates_index - 2];
            coordinates[coordinates_index + 1] = 
                coordinates[coordinates_index - 1] + 1;
            break;
        case NEG_X:
            coordinates[coordinates_index] = 
                coordinates[coordinates_index - 2] - 1;
            coordinates[coordinates_index + 1] = 
                coordinates[coordinates_index - 1];
            break;
        case NEG_Y:
            coordinates[coordinates_index] = 
                coordinates[coordinates_index - 2];
            coordinates[coordinates_index + 1] = 
                coordinates[coordinates_index - 1] - 1;
            break;
    }
    coordinates_index += 2;
}

void map_step_left(void) {
    if (!terminate_processing) {
        printf("Left step %d.\n", left_step_count++);
        map_step();
    }
}

void map_step_right(void) {
    if (!terminate_processing) {
        printf("Right step %d.\n", right_step_count++);
        map_step();
    }
}

void map_turn_left(void) {
    if (!terminate_processing) {
        printf("Left turn %d.\n", left_turn_count++);
        switch (current_orientation) {
            case POS_X:
                current_orientation = POS_Y;
                break;
            case POS_Y:
                current_orientation = NEG_X;
                break;
            case NEG_X:
                current_orientation = NEG_Y;
                break;
            case NEG_Y:
                current_orientation = POS_X;
                break;
        }
    }
}

void map_turn_right(void) {
    if (!terminate_processing) {
        printf("Right turn %d.\n", right_turn_count++);
        switch (current_orientation) {
            case POS_X:
                current_orientation = NEG_Y;
                break;
            case NEG_Y:
                current_orientation = NEG_X;
                break;
            case NEG_X:
                current_orientation = POS_Y;
                break;
            case POS_Y:
                current_orientation = POS_X;
                break;
        }
    }
}

int8_t *map_get_next_coordinates(int *length) {
	if (retrieved_coordinates_index >= COORDINATES_ARRAY_SIZE) {
		return NULL;
	}
	
	int8_t *next_coordinates = &coordinates[retrieved_coordinates_index];
	if (coordinates_index - retrieved_coordinates_index < 50) {
		*length = coordinates_index - retrieved_coordinates_index;
	} else {
		*length = 50;
	}
	
    retrieved_coordinates_index += 50;
	
    return next_coordinates;
}

void map_terminate_processing(void) {
    terminate_processing = true;
}
