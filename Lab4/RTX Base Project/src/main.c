#include <stdio.h>
#include "system_config.h"

#include "lab4_threads.h"
#include "cmsis_os.h" // ARM::CMSIS:RTOS:Keil RTX


osThreadId thrdID1;
void thread1(void const* args) {
	while (1) {
		thread_leds(args);
	}
}
osThreadDef(thread1, osPriorityNormal, 1, 100);

osThreadId thrdID2;
void thread2(void const* args) {
	while (1) {
		thread_temperature(args);
	}
}
osThreadDef(thread2, osPriorityNormal, 1, 0);

osThreadId thrdID3;
void thread3(void const* args) {
	while (1) {
		thread_keypad(args);
	}
}
osThreadDef(thread3, osPriorityNormal, 1, 100);

osThreadId thrdID4;
void thread4(void const* args) {
	while (1) {
		thread_accelerometer(args);
	}
}
osThreadDef(thread4, osPriorityNormal, 1, 0);


int main() {
	printf("Begin\n");
	system_init();
	lab4_os_init();
	printf("Done system init\n");
	
	osKernelInitialize();
	thrdID1 = osThreadCreate(osThread(thread1), NULL);
	thrdID2 = osThreadCreate(osThread(thread2), NULL);
	thrdID3 = osThreadCreate(osThread(thread3), NULL);
	thrdID4 = osThreadCreate(osThread(thread4), NULL);
	osKernelStart();
	
	return 0;
}
