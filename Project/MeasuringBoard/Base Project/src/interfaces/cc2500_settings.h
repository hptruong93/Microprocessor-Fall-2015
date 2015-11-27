#ifndef CC2500_SETTINGS_H
#define CC2500_SETTINGS_H

#include "interfaces/cc2500.h"

/**
  * @brief  CC2500 Register settings
  */
#define CC2500_CONFIG_COUNT 36

uint8_t cc2500_config[CC2500_CONFIG_COUNT][2] = {
	{0x29, CC2500_IOCFG2_REG_ADDR},
	{0x06, CC2500_IOCFG0_REG_ADDR},
	{0x07, CC2500_FIFOTHR_REG_ADDR},
	{0x01, CC2500_PKTLEN_REG_ADDR},
	{0x08, CC2500_PKTCTRL1_REG_ADDR},
	{0x04, CC2500_PKTCTRL0_REG_ADDR},
	{0x00, CC2500_ADDR_REG_ADDR},
	{0x00, CC2500_CHANNR_REG_ADDR},
	{0x0C, CC2500_FSCTRL1_REG_ADDR},
	{0x00, CC2500_FSCTRL0_REG_ADDR},
	// freq set to 2.433 GHz, note f_xosc = 26MHz
	{0x5D, CC2500_FREQ2_REG_ADDR},
	{0x93, CC2500_FREQ1_REG_ADDR},
	{0xC5, CC2500_FREQ0_REG_ADDR},
	{0x0E, CC2500_MDMCFG4_REG_ADDR},
	{0x3B, CC2500_MDMCFG3_REG_ADDR},
	{0x73, CC2500_MDMCFG2_REG_ADDR},
	{0x42, CC2500_MDMCFG1_REG_ADDR},
	{0xF8, CC2500_MDMCFG0_REG_ADDR},
	{0x00, CC2500_DEVIATN_REG_ADDR},
	{0x00, CC2500_MCSM1_REG_ADDR}, //Previously 0x02
	{0x18, CC2500_MCSM0_REG_ADDR},
	{0x1D, CC2500_FOCCFG_REG_ADDR},
	{0x1C, CC2500_BSCFG_REG_ADDR},
	{0xC7, CC2500_AGCCTRL2_REG_ADDR},
	{0x40, CC2500_AGCCTRL1_REG_ADDR},
	{0xB0, CC2500_AGCCTRL0_REG_ADDR},
	{0xB6, CC2500_FREND1_REG_ADDR},
	{0x10, CC2500_FREND0_REG_ADDR},
	{0xEA, CC2500_FSCAL3_REG_ADDR},
	{0x0A, CC2500_FSCAL2_REG_ADDR},
	{0x00, CC2500_FSCAL1_REG_ADDR},
	{0x19, CC2500_FSCAL0_REG_ADDR},
	{0x59, CC2500_FSTEST_REG_ADDR},
	{0x88, CC2500_TEST2_REG_ADDR},
	{0x31, CC2500_TEST1_REG_ADDR},
	{0x0B, CC2500_TEST0_REG_ADDR}
};

#endif
