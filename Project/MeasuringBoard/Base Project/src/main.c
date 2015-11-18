#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

#include "modules/led_rotation_sm.h"
#include "system_config.h"
#include "interfaces/cc2500.h"
#include "interfaces/cc2500_settings.h"

static uint8_t system_ticks;
static uint8_t test[1000];

void print_buffer(uint8_t* buffer, uint8_t num) {
	printf("Buffer is: ");
	for (uint8_t i = 0; i < num; i++) {
		printf("%d - ", buffer[i]);
	}
	printf("\n");
}

void write_the_value(uint8_t value, uint8_t addr) {
	uint8_t temp = value;
	CC2500_Write(&temp, addr, 1);
}

void write_config() {
	write_the_value(VAL_CC2500_IOCFG2, CC2500_IOCFG2_REG_ADDR);
	write_the_value(VAL_CC2500_IOCFG0, CC2500_IOCFG0_REG_ADDR);
	write_the_value(VAL_CC2500_FIFOTHR, CC2500_FIFOTHR_REG_ADDR);
	write_the_value(VAL_CC2500_PKTLEN, CC2500_PKTLEN_REG_ADDR);
	write_the_value(VAL_CC2500_PKTCTRL1, CC2500_PKTCTRL1_REG_ADDR);
	write_the_value(VAL_CC2500_PKTCTRL0, CC2500_PKTCTRL0_REG_ADDR);
	write_the_value(VAL_CC2500_ADDR, CC2500_ADDR_REG_ADDR);
	write_the_value(VAL_CC2500_CHANNR, CC2500_CHANNR_REG_ADDR);
	write_the_value(VAL_CC2500_FSCTRL1, CC2500_FSCTRL1_REG_ADDR);
	write_the_value(VAL_CC2500_FSCTRL0, CC2500_FSCTRL0_REG_ADDR);
	write_the_value(VAL_CC2500_FREQ2, CC2500_FREQ2_REG_ADDR);
	write_the_value(VAL_CC2500_FREQ1, CC2500_FREQ1_REG_ADDR);
	write_the_value(VAL_CC2500_FREQ0, CC2500_FREQ0_REG_ADDR);
	write_the_value(VAL_CC2500_MDMCFG4, CC2500_MDMCFG4_REG_ADDR);
	write_the_value(VAL_CC2500_MDMCFG3, CC2500_MDMCFG3_REG_ADDR);
	write_the_value(VAL_CC2500_MDMCFG2, CC2500_MDMCFG2_REG_ADDR);
	write_the_value(VAL_CC2500_MDMCFG1, CC2500_MDMCFG1_REG_ADDR);
	write_the_value(VAL_CC2500_MDMCFG0, CC2500_MDMCFG0_REG_ADDR);
	write_the_value(VAL_CC2500_DEVIATN, CC2500_DEVIATN_REG_ADDR);
	write_the_value(VAL_CC2500_MCSM1, CC2500_MCSM1_REG_ADDR);
	write_the_value(VAL_CC2500_MCSM0, CC2500_MCSM0_REG_ADDR);
	write_the_value(VAL_CC2500_FOCCFG, CC2500_FOCCFG_REG_ADDR);
	write_the_value(VAL_CC2500_BSCFG, CC2500_BSCFG_REG_ADDR);
	write_the_value(VAL_CC2500_AGCCTRL2, CC2500_AGCCTRL2_REG_ADDR);
	write_the_value(VAL_CC2500_AGCCTRL1, CC2500_AGCCTRL1_REG_ADDR);
	write_the_value(VAL_CC2500_AGCCTRL0, CC2500_AGCCTRL0_REG_ADDR);
	write_the_value(VAL_CC2500_FREND1, CC2500_FREND1_REG_ADDR);
	write_the_value(VAL_CC2500_FREND0, CC2500_FREND0_REG_ADDR);
	write_the_value(VAL_CC2500_FSCAL3, CC2500_FSCAL3_REG_ADDR);
	write_the_value(VAL_CC2500_FSCAL2, CC2500_FSCAL2_REG_ADDR);
	write_the_value(VAL_CC2500_FSCAL1, CC2500_FSCAL1_REG_ADDR);
	write_the_value(VAL_CC2500_FSCAL0, CC2500_FSCAL0_REG_ADDR);
	write_the_value(VAL_CC2500_FSTEST, CC2500_FSTEST_REG_ADDR);
	write_the_value(VAL_CC2500_TEST2, CC2500_TEST2_REG_ADDR);
	write_the_value(VAL_CC2500_TEST1, CC2500_TEST1_REG_ADDR);
	write_the_value(VAL_CC2500_TEST0, CC2500_TEST0_REG_ADDR);
}

int main() {
	printf("Here\n");
	SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY);
	CC2500_LowLevel_Init();
	system_init();
	
	printf("Done init\n");
	CC2500_Read(test, 0x30, 1);
	CC2500_Read(test + 1, 0x30, 2);
	printf("Read the thing\n");
	print_buffer(test, 3);
	
	write_config();
	
	CC2500_Read(test, 0x34, 1); //SRX
	print_buffer(test, 1);
	
	while(1){
		while (!system_ticks);
		led_rotation_rotate_leds();
		
		CC2500_Read(test, 0x35, 2); //SRX
		CC2500_Read(test + 2, 0x3f, 1); //Read rx fifo

		print_buffer(test, 4);
		
		system_ticks = 0;
	}
	
	return 0;
}

/*interrupt handler
*/

void SysTick_Handler() {
	system_ticks = 1;
}
