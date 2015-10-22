#include "system_config.h"
#include "stm32f4xx_conf.h"
#include "interfaces/accelerometer_interface.h"

void system_init(void) {
	accelerometer_init();
}

