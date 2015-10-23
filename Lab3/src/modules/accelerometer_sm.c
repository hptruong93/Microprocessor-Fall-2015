#include "modules/accelerometer_sm.h"
#include "interfaces/accelerometer_interface.h"

accelerometer_info accelerometer_angles;

void accelerometer_process(void) {
	static float x, y, z;
	accelerometer_read(&x, &y, &z);
	accelerometer_calculate_rotation(x, y, z);
	printf("PRY = (%.2f, %.2f, %.2f)\n", accelerometer_angles.pitch, accelerometer_angles.roll, accelerometer_angles.yaw);
}
