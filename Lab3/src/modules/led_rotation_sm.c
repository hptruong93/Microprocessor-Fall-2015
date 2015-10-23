#include "interfaces/led_interface.h"
#include "modules/led_rotation_sm.h"


void led_rotation_rotate_leds(void) {
	static uint16_t fun;
	fun++;

	led_all_off();
	static const uint16_t count_max = 200;
	if (fun % count_max < count_max / 4) {
		SET_LED(LED_1);
	} else if (fun % count_max < count_max / 2) {
		SET_LED(LED_2);
	} else if (fun % count_max < 3 * count_max / 4) {
		SET_LED(LED_3);
	} else {
		SET_LED(LED_4);
	}
}
