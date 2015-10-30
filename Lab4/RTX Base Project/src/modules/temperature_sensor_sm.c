#include "temperature_sensor_sm.h"
#include "interfaces/temperature_sensor_interface.h"
#include "modules/seven_segments_sm.h"

void temperature_sensor_alarm(float temperature) {
	if (temperature >= TEMPERATURE_ALARM_THRESHOLD) {
		//Alarm
		seven_segment_set_display_mode(SEVEN_SEGMENT_DISPLAY_MODE_BLINK);
	} else {
		//Normal
		seven_segment_set_display_mode(SEVEN_SEGMENT_DISPLAY_MODE_NORMAL);
	}
}
