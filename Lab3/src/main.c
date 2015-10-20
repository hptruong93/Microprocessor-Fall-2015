/**
  ******************************************************************************
  * @file    LIS302DL/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup LIS302DL_Example
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t PrescalerValue = 0;
uint8_t Buffer[6];
__IO uint32_t TimingDelay = 0;
__IO int8_t XOffset;
__IO int8_t YOffset;
uint8_t tick;


/* Private function prototypes -----------------------------------------------*/
//static void TIM_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  //uint8_t ctrl = 0;
  printf("AAA");
  LIS3DSH_InitTypeDef  LIS3DSH_InitStruct;

  /* Set configuration of LIS302DL*/
  LIS3DSH_InitStruct.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_400;
  LIS3DSH_InitStruct.Axes_Enable = LIS3DSH_X_ENABLE | LIS3DSH_Y_ENABLE | LIS3DSH_Z_ENABLE;
	LIS3DSH_InitStruct.Full_Scale = LIS3DSH_FULLSCALE_4;
	LIS3DSH_InitStruct.AA_Filter_BW = LIS3DSH_AA_BW_800;
  LIS3DSH_InitStruct.Continous_Update = LIS3DSH_ContinousUpdate_Enabled;
  LIS3DSH_InitStruct.Self_Test = LIS3DSH_SELFTEST_NORMAL;

  LIS3DSH_Init(&LIS3DSH_InitStruct);
  /* SysTick end of count event each 100ms */
  SysTick_Config(SystemCoreClock/1000);

  /* Required delay for the MEMS Accelerometre: Turn-on time = 3/Output data Rate 
                                                             = 3/100 = 30ms */
  Delay(30);
  tick = 0;
	
  while(1)
  {
  }
}

/**
  * @brief  Configures the TIM Peripheral.
  * @param  None
  * @retval None
  */



/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/**
  * @brief  MEMS accelerometre management of the timeout situation.
  * @param  None.
  * @retval None.
  */
//uint32_t LIS302DL_TIMEOUT_UserCallback(void)
//{
//  /* MEMS Accelerometer Timeout error occured */
//  while (1)
//  {   
//  }
//}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
