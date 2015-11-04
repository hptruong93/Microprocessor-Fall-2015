/**
  ******************************************************************************
  * @file    stm32f4_discovery_LSM9DS1.h
  * @author  Ashraf Suyyagh / Based on the LSM9DS1 driver by the MCD Application Team
  * @version V1.0.0
  * @date    11th-February-2014
  * @brief   This file contains all the functions prototypes for the stm32f4_discovery_LSM9DS1.c
  *          firmware driver.
  ******************************************************************************
	**/
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LSM9DS1_H
#define __LSM9DS1_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
 #include "stm32f4xx.h"
 #include "stm32f4xx_gpio.h"
 #include "stm32f4xx_rcc.h"
 #include "stm32f4xx_spi.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32F4_DISCOVERY
  * @{
  */ 

/** @addtogroup STM32F4_DISCOVERY_LSM9DS1
  * @{
  */
  

/** @defgroup STM32F4_DISCOVERY_LSM9DS1_Exported_Types
  * @{
  */
   
/* LSM9DS1 struct */
typedef struct
{
  uint8_t Power_Mode_Output_DataRate;         /* Ppower down or /active mode with output data rate 3.125 / 6.25 / 12.5 / 25 / 50 / 100 / 400 / 800 / 1600 HZ */
  uint8_t Axes_Enable;                        /* Axes enable */
  uint8_t Continous_Update;					 				  /* Block or update Low/High registers of data until all data is read */
	uint8_t AA_Filter_BW;												/* Choose anti-aliasing filter BW 800 / 400 / 200 / 50 Hz*/
  uint8_t Full_Scale;                         /* Full scale 2 / 4 / 6 / 8 / 16 g */
  uint8_t Self_Test;                          /* Self test */
}LSM9DS1_InitTypeDef;
 

/* LSM9DS1 Data ready interrupt struct */
typedef struct
{
  uint8_t Dataready_Interrupt;                /* Enable/Disable data ready interrupt */
  uint8_t Interrupt_signal;                   /* Interrupt Signal Active Low / Active High */
  uint8_t Interrupt_type;                     /* Interrupt type as latched or pulsed */ 
}LSM9DS1_DRYInterruptConfigTypeDef;  

/**
  * @}
  */
  
/** @defgroup STM32F4_DISCOVERY_LSM9DS1_Exported_Constants
  * @{
  */
  
  /* Uncomment the following line to use the default LSM9DS1_TIMEOUT_UserCallback() 
   function implemented in stm32f4_discovery_LSM9DS1.c file.
   LSM9DS1_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occure during communication (waiting transmit data register empty flag(TXE)
   or waiting receive data register is not empty flag (RXNE)). */   
/* #define USE_DEFAULT_TIMEOUT_CALLBACK */

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
   
#define LSM9DS1_FLAG_TIMEOUT         ((uint32_t)0x1000)

/**
  * @brief  LSM9DS1 SPI Interface pins
  */
#define LSM9DS1_SPI                       SPI1
#define LSM9DS1_SPI_CLK                   RCC_APB2Periph_SPI1

#define LSM9DS1_SPI_SCK_PIN               GPIO_Pin_5                  /* PA.05 */
#define LSM9DS1_SPI_SCK_GPIO_PORT         GPIOA                       /* GPIOA */
#define LSM9DS1_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define LSM9DS1_SPI_SCK_SOURCE            GPIO_PinSource5
#define LSM9DS1_SPI_SCK_AF                GPIO_AF_SPI1

#define LSM9DS1_SPI_MISO_PIN              GPIO_Pin_6                  /* PA.6 */
#define LSM9DS1_SPI_MISO_GPIO_PORT        GPIOA                       /* GPIOA */
#define LSM9DS1_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define LSM9DS1_SPI_MISO_SOURCE           GPIO_PinSource6
#define LSM9DS1_SPI_MISO_AF               GPIO_AF_SPI1

#define LSM9DS1_SPI_MOSI_PIN              GPIO_Pin_7                  /* PA.7 */
#define LSM9DS1_SPI_MOSI_GPIO_PORT        GPIOA                       /* GPIOA */
#define LSM9DS1_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define LSM9DS1_SPI_MOSI_SOURCE           GPIO_PinSource7
#define LSM9DS1_SPI_MOSI_AF               GPIO_AF_SPI1

#define LSM9DS1_SPI_CS_PIN                GPIO_Pin_3                  /* PE.03 */
#define LSM9DS1_SPI_CS_GPIO_PORT          GPIOE                       /* GPIOE */
#define LSM9DS1_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOE

#define LSM9DS1_SPI_INT1_PIN              GPIO_Pin_0                  /* PE.00 */
#define LSM9DS1_SPI_INT1_GPIO_PORT        GPIOE                       /* GPIOE */
#define LSM9DS1_SPI_INT1_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define LSM9DS1_SPI_INT1_EXTI_LINE        EXTI_Line0
#define LSM9DS1_SPI_INT1_EXTI_PORT_SOURCE EXTI_PortSourceGPIOE
#define LSM9DS1_SPI_INT1_EXTI_PIN_SOURCE  EXTI_PinSource0
#define LSM9DS1_SPI_INT1_EXTI_IRQn        EXTI0_IRQn 

#define LSM9DS1_SPI_INT2_PIN              GPIO_Pin_1                  /* PE.01 */
#define LSM9DS1_SPI_INT2_GPIO_PORT        GPIOE                       /* GPIOE */
#define LSM9DS1_SPI_INT2_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define LSM9DS1_SPI_INT2_EXTI_LINE        EXTI_Line1
#define LSM9DS1_SPI_INT2_EXTI_PORT_SOURCE EXTI_PortSourceGPIOE
#define LSM9DS1_SPI_INT2_EXTI_PIN_SOURCE  EXTI_PinSource1
#define LSM9DS1_SPI_INT2_EXTI_IRQn        EXTI1_IRQn 


/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/

/*******************************************************************************
*  OUT_T : Temperature sensor
*  Read only register
*  Default value: 0x00 corresponds to 25 degrees Celsius
*******************************************************************************/
#define LSM9DS1_OUT_T              0x0C
/******************************************************************************/

/*******************************************************************************
*  INFO1: Device Identification Register
*  Read only register
*  Default value: 0x21
*******************************************************************************/
#define LSM9DS1_INFO1              0x0D
/*******************************************************************************
*  INFO2: Device Identification Register
*  Read only register
*  Default value: 0x00
*******************************************************************************/
#define LSM9DS1_INFO2              0x0E

/*******************************************************************************
*  WHO_AM_I Register: Device Identification Register
*  Read only register
*  Default value: 0x3B
*******************************************************************************/
#define LSM9DS1_WHO_AM_I_ADDR      0x0F

/*******************************************************************************
*  OFF_X Register: Offset compensation register X
*  Read only register
*  Default value: 0x00
*******************************************************************************/
#define LSM9DS1_OFF_X       		   0x10

/*******************************************************************************
*  OFF_Y Register: Offset compensation register Y
*  Read only register
*  Default value: 0x00
*******************************************************************************/
#define LSM9DS1_OFF_Y      			   0x11

/*******************************************************************************
*  OFF_Z Register: Offset compensation register Z
*  Read only register
*  Default value: 0x00
*******************************************************************************/
#define LSM9DS1_OFF_Z        		   0x12

/*******************************************************************************
*  STAT: STATUS register
*  Read only register
*  Default value: 0x00
*******************************************************************************/
#define LSM9DS1_STAT       			   0x17

/*******************************************************************************
*  CTRL_REG4: Control Register 4 register
*  Read/Write  register
*  Default value: 0x07
*******************************************************************************/
#define LSM9DS1_CTRL_REG4_G       	 0x1E

/*******************************************************************************
*  CTRL_REG1: Control Register 1 register
*  Read/Write  register
*  Default value: 0x00
*******************************************************************************/
#define LSM9DS1_CTRL_REG1_G       	 0x10

/*******************************************************************************
*  CTRL_REG2: Control Register 2 register
*  Read/Write  register
*  Default value: 0x00
*******************************************************************************/
#define LSM9DS1_CTRL_REG2_G       	 0x11

/*******************************************************************************
*  CTRL_REG3: Control Register 3 register
*  Read/Write  register
*  Default value: 0x00
*******************************************************************************/
#define LSM9DS1_CTRL_REG3_G       	 0x12

/*******************************************************************************
*  CTRL_REG5: Control Register 5 register
*  Read/Write  register
*  Default value: 0x00
*******************************************************************************/
#define LSM9DS1_CTRL_REG5_G       	 0x24

/*******************************************************************************
*  STATUS: STATUS register
*  Read only register
*  Default value: 0x00
*******************************************************************************/
#define LSM9DS1_STATUS       	 	 	 0x27

/*******************************************************************************
*  Output registers: X, Y, Z axes 8 bit MSB/LSB registers (combined 16 bits result)
*  Read only register
*  Default value: 0x00 corresponds to 0g acceleration
*******************************************************************************/
#define LSM9DS1_OUT_X_L       	 	 0x28
#define LSM9DS1_OUT_X_H       	 	 0x29
#define LSM9DS1_OUT_Y_L       	 	 0x2A
#define LSM9DS1_OUT_Y_H       	 	 0x2B
#define LSM9DS1_OUT_Z_L       	 	 0x2C
#define LSM9DS1_OUT_Z_H       	 	 0x2D

/******************************************************************************/
/*************************** END REGISTER MAPPING  ****************************/
/******************************************************************************/

/******************************************************************************/
/****************************** START BIT MAPPING  ****************************/
/******************************************************************************/


#define LSM9DS1_DOR									((uint8_t)0x02)
#define LSM9DS1_DRDY								((uint8_t)0x01)
#define LSM9DS1_ZYXOR								((uint8_t)0x80)
#define LSM9DS1_ZOR									((uint8_t)0x40)
#define LSM9DS1_YOR									((uint8_t)0x20)
#define LSM9DS1_XOR									((uint8_t)0x10)
#define LSM9DS1_ZYXDA								((uint8_t)0x08)
#define LSM9DS1_ZDA									((uint8_t)0x04)
#define LSM9DS1_YDA									((uint8_t)0x02)
#define LSM9DS1_XDA									((uint8_t)0x01)

/******************************************************************************/
/******************************* END BIT MAPPING  *****************************/
/******************************************************************************/

/** @defgroup Data_Rate_selection                 
  * @{
  */
#define LSM9DS1_PWRDWN							((uint8_t)0x00)
#define LSM9DS1_DATARATE_3_125			((uint8_t)0x10)
#define LSM9DS1_DATARATE_6_25				((uint8_t)0x20)
#define LSM9DS1_DATARATE_12_5				((uint8_t)0x30)
#define LSM9DS1_DATARATE_25					((uint8_t)0x40)
#define LSM9DS1_DATARATE_50					((uint8_t)0x50)
#define LSM9DS1_DATARATE_100				((uint8_t)0x60)
#define LSM9DS1_DATARATE_400				((uint8_t)0x70)
#define LSM9DS1_DATARATE_800				((uint8_t)0x80)
#define LSM9DS1_DATARATE_1600				((uint8_t)0x90)

/**
  * @}
  */
  
  /** @defgroup Full_Scale_selection 
  * @{
  */
#define LSM9DS1_FULLSCALE_2					((uint8_t)0x00)
#define LSM9DS1_FULLSCALE_4					((uint8_t)0x08)
#define LSM9DS1_FULLSCALE_6					((uint8_t)0x10)
#define LSM9DS1_FULLSCALE_8					((uint8_t)0x18)
#define LSM9DS1_FULLSCALE_16				((uint8_t)0x20)
/**
  * @}
  */
  
 /** @defgroup Antialiasing_Filter_BW 
  * @{
  */
#define LSM9DS1_AA_BW_800						((uint8_t)0x00)
#define LSM9DS1_AA_BW_400						((uint8_t)0x40)
#define LSM9DS1_AA_BW_200						((uint8_t)0x80)
#define LSM9DS1_AA_BW_50						((uint8_t)0xc0)
/**
  * @}
  */ 
  
  /** @defgroup Self_Test_selection 
  * @{
  */
#define LSM9DS1_SELFTEST_NORMAL    	((uint8_t)0x00)
#define LSM9DS1_SELFTEST_P          ((uint8_t)0x02)
#define LSM9DS1_SELFTEST_M          ((uint8_t)0x04)
/**
  * @}
  */  

/** @defgroup Direction_XYZ_selection 
  * @{
  */
#define LSM9DS1_X_ENABLE            ((uint8_t)0x01)
#define LSM9DS1_Y_ENABLE            ((uint8_t)0x02)
#define LSM9DS1_Z_ENABLE            ((uint8_t)0x04)
/**
  * @}
  */
  
 /** @defgroup Output_Register_Update 
 * @{
 */
  #define LSM9DS1_ContinousUpdate_Enabled							  ((uint8_t)0x08)
	#define LSM9DS1_ContinousUpdate_Disabled						  ((uint8_t)0x00)
	
 /**
 * @}
 */
 
 /** @defgroup SPI_Serial_Interface_Mode_selection 
  * @{
  */
#define LSM9DS1_SERIALINTERFACE_4WIRE    	((uint8_t)0x00)
#define LSM9DS1_SERIALINTERFACE_3WIRE     ((uint8_t)0x01)
/**
  * @}
  */ 
  
  /** @defgroup Data_Ready_Interrupt_Setup 
  * @{
  */
 #define LSM9DS1_DATA_READY_INTERRUPT_DISABLED     		 ((uint8_t)0x00)     
 #define LSM9DS1_DATA_READY_INTERRUPT_ENABLED					 ((uint8_t)0x88)
 #define LSM9DS1_ACTIVE_LOW_INTERRUPT_SIGNAL				   ((uint8_t)0x00)
 #define LSM9DS1_ACTIVE_HIGH_INTERRUPT_SIGNAL			   	 ((uint8_t)0x40)
 #define LSM9DS1_INTERRUPT_REQUEST_PULSED              ((uint8_t)0x20)
 #define LSM9DS1_INTERRUPT_REQUEST_LATCHED             ((uint8_t)0x00)
  /**
  * @}
  */
  
	 /** @defgroup Sensitivity 
  * @{
  */
 #define LSM9DS1_SENSITIVITY_2G    	0.061 		      
 #define LSM9DS1_SENSITIVITY_4G		  0.122			 
 #define LSM9DS1_SENSITIVITY_6G			0.183	   
 #define LSM9DS1_SENSITIVITY_8G		  0.244	   	 
 #define LSM9DS1_SENSITIVITY_16G    0.488      
  /**
  * @}
  */
	
  /** @defgroup STM32F4_DISCOVERY_LSM9DS1_Exported_Macros
  * @{
  */
#define LSM9DS1_CS_LOW()       GPIO_ResetBits(LSM9DS1_SPI_CS_GPIO_PORT, LSM9DS1_SPI_CS_PIN)
#define LSM9DS1_CS_HIGH()      GPIO_SetBits(LSM9DS1_SPI_CS_GPIO_PORT, LSM9DS1_SPI_CS_PIN)
/**
  * @}
  */ 
	
/** @defgroup STM32F4_DISCOVERY_LSM9DS1_Exported_Functions
  * @{
  */
void LSM9DS1_LowLevel_Init(void);
void LSM9DS1_Init(LSM9DS1_InitTypeDef *LSM9DS1Struct);
void LSM9DS1_DataReadyInterruptConfig(LSM9DS1_DRYInterruptConfigTypeDef *LSM9DS1_InterruptConfigStruct);
void LSM9DS1_LowpowerCmd(void);
void LSM9DS1_FullScaleCmd(uint8_t FS_value);
void LSM9DS1_DataRateCmd(uint8_t DataRateValue);
void LSM9DS1_RebootCmd(void);
void LSM9DS1_ReadACC(float* out);
void LSM9DS1_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void LSM9DS1_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);

/* USER Callbacks: This is function for which prototype only is declared in
   MEMS accelerometre driver and that should be implemented into user applicaiton. */  
/* LSM9DS1_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occure during communication (waiting transmit data register empty flag(TXE)
   or waiting receive data register is not empty flag (RXNE)).
   You can use the default timeout callback implementation by uncommenting the 
   define USE_DEFAULT_TIMEOUT_CALLBACK in stm32f4_discovery_lis302dl.h file.
   Typically the user implementation of this callback should reset MEMS peripheral
   and re-initialize communication or in worst case reset all the application. */
uint32_t LSM9DS1_TIMEOUT_UserCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* __LSM9DS1_H */
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
