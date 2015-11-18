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

/* Includes ------------------------------------------------------------------*/
 #include "stm32f4xx.h"
 #include "stm32f4xx_conf.h"

/** @addtogroup Utilities
	* @{
	*/
	
/** @addtogroup STM32F4_DISCOVERY
	* @{
	*/ 

/** @addtogroup STM32F4_DISCOVERY_CC2500
	* @{
	*/
	

/** @defgroup STM32F4_DISCOVERY_CC2500_Exported_Types
	* @{
	*/
	 
/* CC2500 struct */
typedef struct
{
	uint8_t Power_Mode;                         /* Power-down/Active Mode */
	uint8_t Output_DataRate;                    /* OUT data rate 100 Hz / 400 Hz */
	uint8_t Bandwidth;
	uint8_t AntiAliasingBandwidth;
	uint8_t Axes_Enable;                        /* Axes enable */
	uint8_t Full_Scale;                         /* Full scale */
	uint8_t Self_Test;                          /* Self test */
}CC2500_InitTypeDef;

/* CC2500 High Pass Filter struct */
typedef struct
{
	uint8_t HighPassFilter_Data_Selection;      /* Internal filter bypassed or data from internal filter send to output register*/
	uint8_t HighPassFilter_CutOff_Frequency;    /* High pass filter cut-off frequency */
	uint8_t HighPassFilter_Interrupt;           /* High pass filter enabled for Freefall/WakeUp #1 or #2 */ 
}CC2500_FilterConfigTypeDef;  

/* CC2500 Interrupt struct */
typedef struct
{
	uint8_t DataReadyInterrupt;
}CC2500_InterruptConfigTypeDef;  

/**
	* @}
	*/
	
/** @defgroup STM32F4_DISCOVERY_CC2500_Exported_Constants
	* @{
	*/

/* Uncomment the following line to use the default CC2500_TIMEOUT_UserCallback() 
	 function implemented in stm32f4_discovery_CC2500.c file.
	 CC2500_TIMEOUT_UserCallback() function is called whenever a timeout condition 
	 occure during communication (waiting transmit data register empty flag(TXE)
	 or waiting receive data register is not empty flag (RXNE)). */   
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
#define CC2500_SPI                       SPI1
#define CC2500_SPI_CLK                   RCC_APB2Periph_SPI1

#define CC2500_SPI_SCK_PIN               GPIO_Pin_5                  /* PA.05 */
#define CC2500_SPI_SCK_GPIO_PORT         GPIOA                       /* GPIOA */
#define CC2500_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define CC2500_SPI_SCK_SOURCE            GPIO_PinSource5
#define CC2500_SPI_SCK_AF                GPIO_AF_SPI1

#define CC2500_SPI_MISO_PIN              GPIO_Pin_6                  /* PA.6 */
#define CC2500_SPI_MISO_GPIO_PORT        GPIOA                       /* GPIOA */
#define CC2500_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define CC2500_SPI_MISO_SOURCE           GPIO_PinSource6
#define CC2500_SPI_MISO_AF               GPIO_AF_SPI1

#define CC2500_SPI_MOSI_PIN              GPIO_Pin_7                  /* PA.7 */
#define CC2500_SPI_MOSI_GPIO_PORT        GPIOA                       /* GPIOA */
#define CC2500_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define CC2500_SPI_MOSI_SOURCE           GPIO_PinSource7
#define CC2500_SPI_MOSI_AF               GPIO_AF_SPI1

#define CC2500_SPI_CS_PIN                GPIO_Pin_9                  /* PE.03 */
#define CC2500_SPI_CS_GPIO_PORT          GPIOB                       /* GPIOE */
#define CC2500_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOB

#define CC2500_SPI_INT1_PIN              GPIO_Pin_0                  /* PE.00 */
#define CC2500_SPI_INT1_GPIO_PORT        GPIOE                       /* GPIOE */
#define CC2500_SPI_INT1_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define CC2500_SPI_INT1_EXTI_LINE        EXTI_Line0
#define CC2500_SPI_INT1_EXTI_PORT_SOURCE EXTI_PortSourceGPIOE
#define CC2500_SPI_INT1_EXTI_PIN_SOURCE  EXTI_PinSource0
#define CC2500_SPI_INT1_EXTI_IRQn        EXTI0_IRQn 

#define CC2500_SPI_INT2_PIN              GPIO_Pin_1                  /* PE.01 */
#define CC2500_SPI_INT2_GPIO_PORT        GPIOE                       /* GPIOE */
#define CC2500_SPI_INT2_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define CC2500_SPI_INT2_EXTI_LINE        EXTI_Line1
#define CC2500_SPI_INT2_EXTI_PORT_SOURCE EXTI_PortSourceGPIOE
#define CC2500_SPI_INT2_EXTI_PIN_SOURCE  EXTI_PinSource1
#define CC2500_SPI_INT2_EXTI_IRQn        EXTI1_IRQn 


/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/

#define CC2500_WHO_AM_I_ADDR                  0x0F
#define CC2500_CTRL_REG1_ADDR                 0x10
#define CC2500_CTRL_REG2_ADDR              0x11
#define CC2500_CTRL_REG3_ADDR              0x12
#define CC2500_CTRL_REG5_XL_ADDR              0x1F
#define CC2500_CTRL_REG6_XL_ADDR              0x20


#define CC2500_HP_FILTER_RESET_REG_ADDR     0x23


#define CC2500_STATUS_REG_ADDR             0x27
#define CC2500_OUT_X_ADDR                  0x28

#define CC2500_OUT_Y_ADDR                  0x2A

#define CC2500_OUT_Z_ADDR                  0x2C

#define CC2500_FF_WU_CFG1_REG_ADDR         0x30

#define CC2500_FF_WU_SRC1_REG_ADDR           0x31

#define CC2500_FF_WU_THS1_REG_ADDR          0x32

#define CC2500_FF_WU_DURATION1_REG_ADDR     0x33

#define CC2500_FF_WU_CFG2_REG_ADDR         0x34

#define CC2500_FF_WU_SRC2_REG_ADDR           0x35

#define CC2500_FF_WU_THS2_REG_ADDR          0x36

/*******************************************************************************
*  FF_WU_DURATION_2 Register: duration Register
*  Read Write register
*  Default value: 0x00
*  7:0 D7-D0 Duration value. (Duration steps and maximum values depend on the ODR chosen)
 ******************************************************************************/
#define CC2500_FF_WU_DURATION2_REG_ADDR     0x37

/******************************************************************************
*  CLICK_CFG Register: click Register
*  Read Write register
*  Default value: 0x00
*  7 Reserved
*  6 LIR: Latch Interrupt request.
*         0: interrupt request not latched
*         1: interrupt request latched
*  5 Double_Z: Enable interrupt generation on double click event on Z axis.
*              0: disable interrupt request
*              1: enable interrupt request
*  4 Single_Z: Enable interrupt generation on single click event on Z axis.
*              0: disable interrupt request
*              1: enable interrupt request
*  3 Double_Y: Enable interrupt generation on double click event on Y axis.
*              0: disable interrupt request
*              1: enable interrupt request
*  2 Single_Y: Enable interrupt generation on single click event on Y axis.
*              0: disable interrupt request
*              1: enable interrupt request
*  1 Double_X: Enable interrupt generation on double click event on X axis.
*              0: disable interrupt request
*              1: enable interrupt request
*  0 Single_y: Enable interrupt generation on single click event on X axis.
*              0: disable interrupt request
*              1: enable interrupt request
 ******************************************************************************/
#define CC2500_CLICK_CFG_REG_ADDR     0x38

//Determine various interrupts (most interested in data ready interrupt)
#define CC2500_INT1_CTRL_REG_ADDR     0x0C

/******************************************************************************
*  CLICK_SRC Register: click status Register
*  Read only register
*  Default value: 0x00
*  7 Reserved
*  6 IA: Interrupt active.
*        0: no interrupt has been generated
*        1: one or more interrupts have been generated
*  5 Double_Z: Double click on Z axis event.
*        0: no interrupt
*        1: Double Z event has occurred 
*  4 Single_Z: Z low.
*        0: no interrupt
*        1: Single Z event has occurred 
*  3 Double_Y: Y high.
*        0: no interrupt
*        1: Double Y event has occurred 
*  2 Single_Y: Y low.
*        0: no interrupt
*        1: Single Y event has occurred 
*  1 Double_X: X high.
*        0: no interrupt
*        1: Double X event has occurred 
*  0 Single_X: X low.
*        0: no interrupt
*        1: Single X event has occurred 
*******************************************************************************/
#define CC2500_CLICK_SRC_REG_ADDR        0x39

/*******************************************************************************
*  CLICK_THSY_X Register: Click threshold Y and X register
*  Read Write register
*  Default value: 0x00
*  7:4 THSy3-THSy0: Click threshold on Y axis, step 0.5g
*  3:0 THSx3-THSx0: Click threshold on X axis, step 0.5g
*******************************************************************************/
#define CC2500_CLICK_THSY_X_REG_ADDR        0x3B

/*******************************************************************************
*  CLICK_THSZ Register: Click threshold Z register
*  Read Write register
*  Default value: 0x00
*  7:4 Reserved
*  3:0 THSz3-THSz0: Click threshold on Z axis, step 0.5g
*******************************************************************************/
#define CC2500_CLICK_THSZ_REG_ADDR         0x3C

/*******************************************************************************
*  CLICK_TimeLimit Register: Time Limit register
*  Read Write register
*  Default value: 0x00
*  7:0 Dur7-Dur0: Time Limit value, step 0.5g
*******************************************************************************/
#define CC2500_CLICK_TIMELIMIT_REG_ADDR        0x3D

/*******************************************************************************
*  CLICK_Latency Register: Latency register
*  Read Write register
*  Default value: 0x00
*  7:0 Lat7-Lat0: Latency value, step 1msec
*******************************************************************************/
#define CC2500_CLICK_LATENCY_REG_ADDR        0x3E

/*******************************************************************************
*  CLICK_Window Register: Window register
*  Read Write register
*  Default value: 0x00
*  7:0 Win7-Win0: Window value, step 1msec
*******************************************************************************/
#define CC2500_CLICK_WINDOW_REG_ADDR        0x3F

/******************************************************************************/
/**************************** END REGISTER MAPPING  ***************************/
/******************************************************************************/

#define CC2500_SENSITIVITY_2_3G                         18  /* 18 mg/digit*/
#define CC2500_SENSITIVITY_9_2G                         72  /* 72 mg/digit*/

/** @defgroup Data_Rate_selection                 
	* @{
	*/
#define CC2500_DATARATE_100                             ((uint8_t)(3 << 5))
#define CC2500_DATARATE_400                             ((uint8_t)(5 << 5))
/**
	* @}
	*/
	
/** @defgroup Power_Mode_selection 
	* @{
	*/
#define CC2500_LOWPOWERMODE_POWERDOWN                   ((uint8_t)0x00)
#define CC2500_LOWPOWERMODE_ACTIVE                      ((uint8_t)0x40)
/**
	* @}
	*/
	
/** @defgroup Full_Scale_selection 
	* @{
	*/
#define CC2500_FULLSCALE_2G                             ((uint8_t)(0 << 3))
#define CC2500_FULLSCALE_4G                             ((uint8_t)(2 << 3))
#define CC2500_FULLSCALE_8G                             ((uint8_t)(3 << 3))
#define CC2500_FULLSCALE_16G                            ((uint8_t)(1 << 3))

/**
	* @}
	*/
	
/** @defgroup Bandwidth_selection 
	* @{
	*/
#define CC2500_BANDWIDTH_ODR_BASED                             ((uint8_t)(0 << 2))
#define CC2500_BANDWIDTH_ANTI_ALIASING_BASED                   ((uint8_t)(1 << 2))

/**
	* @}
	*/
	
/**
	* @}
	*/
	
/** @defgroup AntiAliasing_Bandwidth_selection 
	* @{
	*/
#define CC2500_ANTI_ALIASING_BANDWIDTH_408_HZ                  ((uint8_t)(0))
#define CC2500_ANTI_ALIASING_BANDWIDTH_211_HZ                  ((uint8_t)(1))
#define CC2500_ANTI_ALIASING_BANDWIDTH_105_HZ                  ((uint8_t)(2))
#define CC2500_ANTI_ALIASING_BANDWIDTH_050_HZ                  ((uint8_t)(3))

/**
	* @}
	*/
	
/** @defgroup Self_Test_selection 
	* @{
	*/
#define CC2500_SELFTEST_NORMAL                          ((uint8_t)0x00)
#define CC2500_SELFTEST_P                               ((uint8_t)0x10)
#define CC2500_SELFTEST_M                               ((uint8_t)0x08)
/**
	* @}
	*/  

/** @defgroup Direction_XYZ_selection 
	* @{
	*/
#define CC2500_X_ENABLE                                 ((uint8_t)(1 << 3))
#define CC2500_Y_ENABLE                                 ((uint8_t)(1 << 4))
#define CC2500_Z_ENABLE                                 ((uint8_t)(1 << 5))
#define CC2500_XYZ_ENABLE                               ((uint8_t)(7 << 3))
/**
	* @}
	*/
 
 /** @defgroup SPI_Serial_Interface_Mode_selection 
	* @{
	*/
#define CC2500_SERIALINTERFACE_4WIRE                    ((uint8_t)0x00)
#define CC2500_SERIALINTERFACE_3WIRE                    ((uint8_t)0x80)
/**
	* @}
	*/ 

 /** @defgroup Boot_Mode_selection 
	* @{
	*/
#define CC2500_BOOT_NORMALMODE                          ((uint8_t)0x00)
#define CC2500_BOOT_REBOOTMEMORY                        ((uint8_t)0x40)
/**
	* @}
	*/   

 /** @defgroup Filtered_Data_Selection_Mode_selection 
	* @{
	*/
#define CC2500_FILTEREDDATASELECTION_BYPASSED           ((uint8_t)0x00)
#define CC2500_FILTEREDDATASELECTION_OUTPUTREGISTER     ((uint8_t)0x20)
/**
	* @}
	*/ 
	
 /** @defgroup High_Pass_Filter_Interrupt_selection 
	* @{
	*/  
#define CC2500_HIGHPASSFILTERINTERRUPT_OFF              ((uint8_t)0x00)
#define CC2500_HIGHPASSFILTERINTERRUPT_1                ((uint8_t)0x04)
#define CC2500_HIGHPASSFILTERINTERRUPT_2                ((uint8_t)0x08)
#define CC2500_HIGHPASSFILTERINTERRUPT_1_2              ((uint8_t)0x0C)
/**
	* @}
	*/ 
	
 /** @defgroup High_Pass_Filter_selection 
	* @{
	*/
#define CC2500_HIGHPASSFILTER_LEVEL_0                   ((uint8_t)0x00)
#define CC2500_HIGHPASSFILTER_LEVEL_1                   ((uint8_t)0x01)
#define CC2500_HIGHPASSFILTER_LEVEL_2                   ((uint8_t)0x02)
#define CC2500_HIGHPASSFILTER_LEVEL_3                   ((uint8_t)0x03)
/**
	* @}
	*/

/** @defgroup Data_Ready_Interrupt_selection 
	* @{
	*/
#define CC2500_INTERRUPT_DATA_READY_OFF                ((uint8_t)0x00)
#define CC2500_INTERRUPT_DATA_READY_ON                 ((uint8_t)0x01)
/**
	* @}
	*/

/** @defgroup latch_Interrupt_Request_selection 
	* @{
	*/
#define CC2500_INTERRUPTREQUEST_NOTLATCHED              ((uint8_t)0x00)
#define CC2500_INTERRUPTREQUEST_LATCHED                 ((uint8_t)0x40)
/**
	* @}
	*/

/** @defgroup Click_Interrupt_XYZ_selection 
	* @{
	*/
#define CC2500_CLICKINTERRUPT_XYZ_DISABLE               ((uint8_t)0x00)
#define CC2500_CLICKINTERRUPT_X_ENABLE                  ((uint8_t)0x01)
#define CC2500_CLICKINTERRUPT_Y_ENABLE                  ((uint8_t)0x04)
#define CC2500_CLICKINTERRUPT_Z_ENABLE                  ((uint8_t)0x10)
#define CC2500_CLICKINTERRUPT_XYZ_ENABLE                ((uint8_t)0x15)
/**
	* @}
	*/

/** @defgroup Double_Click_Interrupt_XYZ_selection 
	* @{
	*/
#define CC2500_DOUBLECLICKINTERRUPT_XYZ_DISABLE         ((uint8_t)0x00)
#define CC2500_DOUBLECLICKINTERRUPT_X_ENABLE            ((uint8_t)0x02)
#define CC2500_DOUBLECLICKINTERRUPT_Y_ENABLE            ((uint8_t)0x08)
#define CC2500_DOUBLECLICKINTERRUPT_Z_ENABLE            ((uint8_t)0x20)
#define CC2500_DOUBLECLICKINTERRUPT_XYZ_ENABLE          ((uint8_t)0x2A)
/**
	* @}
	*/
/**
	* @}
	*/ 

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



	
#define CC2500_CS_LOW()       GPIO_ResetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN)
#define CC2500_CS_HIGH()      GPIO_SetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN)

/** @defgroup STM32F4_DISCOVERY_CC2500_Exported_Functions
	* @{
	*/ 
void CC2500_LowLevel_Init(void);
void CC2500_Init(CC2500_InitTypeDef *CC2500_InitStruct);
void CC2500_InterruptConfig(CC2500_InterruptConfigTypeDef *CC2500_InterruptConfigStruct);
void CC2500_FilterConfig(CC2500_FilterConfigTypeDef *CC2500_FilterConfigStruct);
void CC2500_LowpowerCmd(uint8_t LowPowerMode);
void CC2500_FullScaleCmd(uint8_t FS_value);
void CC2500_DataRateCmd(uint8_t DataRateValue);
void CC2500_RebootCmd(void);
void CC2500_ReadACC(int32_t* out);
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
