#include "modules/accelerometer_sm.h"
#include "interfaces/accelerometer_interface.h"

accelerometer_info accelerometer_angles;

/*
* Return roll
*/
float accelerometer_calculate_angle(void) {
	static float x, y, z;
	accelerometer_read(&x, &y, &z);
	accelerometer_calculate_rotation(x, y, z);
	
	static uint16_t display = 0;
	display++;
	
	if (display == 200) {
		printf("PRY = (%.2f, %.2f, %.2f)\n", accelerometer_angles.pitch, accelerometer_angles.roll, accelerometer_angles.yaw);
		display = 0;
	}
	return accelerometer_angles.roll;
}
