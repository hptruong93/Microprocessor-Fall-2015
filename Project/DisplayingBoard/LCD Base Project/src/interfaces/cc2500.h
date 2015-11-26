/**
	******************************************************************************
	* @file    stm32f4_discovery_CC2500.h
	* @author  MCD Application Team
	* @version V1.1.0
	* @date    28-October-2011
	* @brief   This file contains all the functions prototypes for the stm32f4_discovery_CC2500.c
	*          firmware driver.
	******************************************************************************
	* @attention
	*
	* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
	* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
	* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
	* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
	* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
	* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
	*
	* <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
	******************************************************************************  
	*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_DISCOVERY_CC2500_H
#define __STM32F4_DISCOVERY_CC2500_H

#ifdef __cplusplus
 extern "C" {
#endif

 #include "stm32f4xx.h"
 #include "stm32f4xx_conf.h"

/* #define USE_DEFAULT_TIMEOUT_CALLBACK */

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
	 on accurate values, they just guarantee that the application will not remain
	 stuck if the SPI communication is corrupted.
	 You may modify these timeout values depending on CPU frequency and application
	 conditions (interrupts routines ...). */   
#define CC2500_FLAG_TIMEOUT         ((uint32_t)0x1000)

/**
	* @brief  CC2500 SPI Interface pins
	*/
#define CC2500_SPI                       SPI4
#define CC2500_SPI_CLK                   RCC_APB2Periph_SPI4

#define CC2500_SPI_SCK_PIN               GPIO_Pin_2
#define CC2500_SPI_SCK_GPIO_PORT         GPIOE
#define CC2500_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOE
#define CC2500_SPI_SCK_SOURCE            GPIO_PinSource2
#define CC2500_SPI_SCK_AF                GPIO_AF_SPI4

#define CC2500_SPI_MISO_PIN              GPIO_Pin_5
#define CC2500_SPI_MISO_GPIO_PORT        GPIOE
#define CC2500_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define CC2500_SPI_MISO_SOURCE           GPIO_PinSource5
#define CC2500_SPI_MISO_AF               GPIO_AF_SPI4

#define CC2500_SPI_MOSI_PIN              GPIO_Pin_6
#define CC2500_SPI_MOSI_GPIO_PORT        GPIOE
#define CC2500_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define CC2500_SPI_MOSI_SOURCE           GPIO_PinSource6
#define CC2500_SPI_MOSI_AF               GPIO_AF_SPI4

#define CC2500_SPI_CS_PIN                GPIO_Pin_12
#define CC2500_SPI_CS_GPIO_PORT          GPIOC
#define CC2500_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOC

// #define CC2500_SPI_INT1_PIN              GPIO_Pin_4
// #define CC2500_SPI_INT1_GPIO_PORT        GPIOE
// #define CC2500_SPI_INT1_GPIO_CLK         RCC_AHB1Periph_GPIOE
// #define CC2500_SPI_INT1_EXTI_LINE        EXTI_Line4
// #define CC2500_SPI_INT1_EXTI_PORT_SOURCE EXTI_PortSourceGPIOE
// #define CC2500_SPI_INT1_EXTI_PIN_SOURCE  EXTI_PinSource4
// #define CC2500_SPI_INT1_EXTI_IRQn        EXTI0_IRQn 

// #define CC2500_SPI_INT2_PIN              GPIO_Pin_1
// #define CC2500_SPI_INT2_GPIO_PORT        GPIOE
// #define CC2500_SPI_INT2_GPIO_CLK         RCC_AHB1Periph_GPIOE
// #define CC2500_SPI_INT2_EXTI_LINE        EXTI_Line1
// #define CC2500_SPI_INT2_EXTI_PORT_SOURCE EXTI_PortSourceGPIOE
// #define CC2500_SPI_INT2_EXTI_PIN_SOURCE  EXTI_PinSource1
// #define CC2500_SPI_INT2_EXTI_IRQn        EXTI1_IRQn 


/**********************************************************************************************************/
#define CC2500_STATE_IDLE 1
#define CC2500_STATE_RX 13
#define CC2500_STATE_RX_OVERFLOW 17

#define CC2500_STATE_TX 19
#define CC2500_STATE_TX_UNDERFLOW 22



/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/
/**********************************************************************************************************/
#define CC2500_IOCFG2_REG_ADDR 0x00
#define CC2500_IOCFG1_REG_ADDR 0x01
#define CC2500_IOCFG0_REG_ADDR 0x02
#define CC2500_FIFOTHR_REG_ADDR 0x03
#define CC2500_SYNC1_REG_ADDR 0x04
#define CC2500_SYNC0_REG_ADDR 0x05
#define CC2500_PKTLEN_REG_ADDR 0x06
#define CC2500_PKTCTRL1_REG_ADDR 0x07
#define CC2500_PKTCTRL0_REG_ADDR 0x08
#define CC2500_ADDR_REG_ADDR 0x09
#define CC2500_CHANNR_REG_ADDR 0x0A
#define CC2500_FSCTRL1_REG_ADDR 0x0B
#define CC2500_FSCTRL0_REG_ADDR 0x0C
#define CC2500_FREQ2_REG_ADDR 0x0D
#define CC2500_FREQ1_REG_ADDR 0x0E
#define CC2500_FREQ0_REG_ADDR 0x0F
#define CC2500_MDMCFG4_REG_ADDR 0x10
#define CC2500_MDMCFG3_REG_ADDR 0x11
#define CC2500_MDMCFG2_REG_ADDR 0x12
#define CC2500_MDMCFG1_REG_ADDR 0x13
#define CC2500_MDMCFG0_REG_ADDR 0x14
#define CC2500_DEVIATN_REG_ADDR 0x15
#define CC2500_MCSM2_REG_ADDR 0x16
#define CC2500_MCSM1_REG_ADDR 0x17
#define CC2500_MCSM0_REG_ADDR 0x18
#define CC2500_FOCCFG_REG_ADDR 0x19
#define CC2500_BSCFG_REG_ADDR 0x1A
#define CC2500_AGCCTRL2_REG_ADDR 0x1B
#define CC2500_AGCCTRL1_REG_ADDR 0x1C
#define CC2500_AGCCTRL0_REG_ADDR 0x1D
#define CC2500_WOREVT1_REG_ADDR 0x1E
#define CC2500_WOREVT0_REG_ADDR 0x1F
#define CC2500_WORCTRL_REG_ADDR 0x20
#define CC2500_FREND1_REG_ADDR 0x21
#define CC2500_FREND0_REG_ADDR 0x22
#define CC2500_FSCAL3_REG_ADDR 0x23
#define CC2500_FSCAL2_REG_ADDR 0x24
#define CC2500_FSCAL1_REG_ADDR 0x25
#define CC2500_FSCAL0_REG_ADDR 0x26
#define CC2500_RCCTRL1_REG_ADDR 0x27
#define CC2500_RCCTRL0_REG_ADDR 0x28
#define CC2500_FSTEST_REG_ADDR 0x29
#define CC2500_PTEST_REG_ADDR 0x2A
#define CC2500_AGCTEST_REG_ADDR 0x2B
#define CC2500_TEST2_REG_ADDR 0x2C
#define CC2500_TEST1_REG_ADDR 0x2D
#define CC2500_TEST0_REG_ADDR 0x2E
/**********************************************************************************************************/
#define CC2500_SRES 0x30
#define CC2500_SRX 0x34
#define CC2500_STX 0x35
#define CC2500_SFRX 0x3A
#define CC2500_SFTX 0x3B

#define CC2500_SNOP 0x3D
#define CC2500_PARTNUM 0x30 | 0xC0
#define CC2500_MARCSTATE 0x35 | 0xC0
#define CC2500_TXBYTES 0x3A | 0xC0
#define CC2500_RXBYTES 0x3B | 0xC0
#define CC2500_RX_FIFO 0x3F
#define CC2500_TX_FIFO 0x3F
/**********************************************************************************************************/



	
#define CC2500_CS_LOW()       GPIO_ResetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN)
#define CC2500_CS_HIGH()      GPIO_SetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN)

/** @defgroup STM32F4_DISCOVERY_CC2500_Exported_Functions
	* @{
	*/ 
void CC2500_LowLevel_Init(void);
void CC2500_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void CC2500_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);

/* USER Callbacks: This is function for which prototype only is declared in
	 MEMS accelerometre driver and that should be implemented into user applicaiton. */  
/* CC2500_TIMEOUT_UserCallback() function is called whenever a timeout condition 
	 occure during communication (waiting transmit data register empty flag(TXE)
	 or waiting receive data register is not empty flag (RXNE)).
	 You can use the default timeout callback implementation by uncommenting the 
	 define USE_DEFAULT_TIMEOUT_CALLBACK in stm32f4_discovery_CC2500.h file.
	 Typically the user implementation of this callback should reset MEMS peripheral
	 and re-initialize communication or in worst case reset all the application. */
uint32_t CC2500_TIMEOUT_UserCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_CC2500_H */
/**
	* @}
	*/

/**
	* @}
	*/

/**
	* @}
	*/

/**
	* @}
	*/ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/