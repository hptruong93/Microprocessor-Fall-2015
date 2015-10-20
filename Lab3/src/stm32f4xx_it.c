/**
  ******************************************************************************
  * @file    LIS302DL/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "stm32f4xx_it.h"
#include "main.h"

/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup SysTick_Example
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Counter  = 0;
extern uint8_t tick;
extern __IO int8_t XOffset;
extern __IO int8_t YOffset;
extern uint8_t Buffer[4];
__IO uint8_t TempAcceleration = 0;
uint8_t ClickReg = 0;
extern __IO uint32_t TimingDelay;
extern __IO uint8_t SingleClickDetect;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  uint8_t temp1, temp2 = 0;
  
  if (TimingDelay != 0x00)
  {
    TimingDelay_Decrement();
  }
  else
  {
    Counter ++;
    if (Counter == 10)
    {
      Buffer[0] = 0;
      Buffer[2] = 0;

      //LIS3DSH_Read(Buffer, LIS3DSH_OUT_X_L, 6);
			
      /* Remove the offsets values from data */
      //Buffer[0] -= XOffset;
      //Buffer[2] -= YOffset;

      /* Update autoreload and capture compare registers value*/
      //temp1 = ABS((int8_t)(Buffer[0]));
      //temp2 = ABS((int8_t)(Buffer[2]));
      //TempAcceleration = MAX(temp1, temp2);

			uint8_t ctrl;
			//LIS3DSH_Read(&ctrl, LIS3DSH_CTRL_REG5, 1);  
//			LIS3DSH_Read(&Buffer[0], LIS3DSH_OUT_X_L, 1);
//			LIS3DSH_Read(&Buffer[1], LIS3DSH_OUT_X_H, 1);
//			LIS3DSH_Read(&Buffer[2], LIS3DSH_OUT_Y_L, 1);
//			LIS3DSH_Read(&Buffer[3], LIS3DSH_OUT_Y_H, 1);
//			LIS3DSH_Read(&Buffer[4], LIS3DSH_OUT_Z_L, 1);
//			LIS3DSH_Read(&Buffer[5], LIS3DSH_OUT_Z_H, 1);
			LIS3DSH_Read(Buffer, LIS3DSH_OUT_X_L, 2);
			
			printf("X, Y is (%d, %d, %d, %d, %d, %d, %d)\n", Buffer[0], Buffer[1], Buffer[2], Buffer[3], Buffer[4], Buffer[5]);
			
//      if(TempAcceleration != 0)
//      {
//        if ((int8_t)Buffer[0] < -2)
//        {

//        	STM_EVAL_LEDOn(LED6);


//        	if ((int8_t)Buffer[0] <= 2)
//        	{
//        		STM_EVAL_LEDOff(LED3);
//        	}

//        	if ((int8_t)Buffer[2] <= 2)
//        	{
//        		STM_EVAL_LEDOff(LED4);
//        	}

//        	if ((int8_t)Buffer[2] >= -2)
//        	{
//        		STM_EVAL_LEDOff(LED5);
//        	}

//        }
//        if ((int8_t)Buffer[0] > 2)
//        {

//        			STM_EVAL_LEDOn(LED3);

//        			if ((int8_t)Buffer[2] <= 2)
//        			{
//        			STM_EVAL_LEDOff(LED4);
//        			}

//        			if ((int8_t)Buffer[2] >= -2)
//        			{
//                	STM_EVAL_LEDOff(LED5);
//        			}

//        			if ((int8_t)Buffer[0] >= -2)
//        			{
//                	STM_EVAL_LEDOff(LED6);
//        			}

//        }
//        if ((int8_t)Buffer[2] > 2)
//        {

//        			STM_EVAL_LEDOn(LED4);

//        			if ((int8_t)Buffer[0] <= 2)
//        			{
//        				STM_EVAL_LEDOff(LED3);
//        			}

//        			if ((int8_t)Buffer[2] >= -2)
//        			{
//        				STM_EVAL_LEDOff(LED5);
//        			}

//        			if ((int8_t)Buffer[0] >= -2)
//        			{
//        				STM_EVAL_LEDOff(LED6);
//        			}

//        }
//        if ((int8_t)Buffer[2] < -2)
//        {

//        			STM_EVAL_LEDOn(LED5);

//        			if ((int8_t)Buffer[0] <= 2)
//        			{
//        				STM_EVAL_LEDOff(LED3);
//        			}

//        			if ((int8_t)Buffer[2] <= 2)
//        			{
//        			STM_EVAL_LEDOff(LED4);
//        			}

//        			if ((int8_t)Buffer[0] >= -2)
//        			{
//        				STM_EVAL_LEDOff(LED6);
//        			}
//        }
//      }
//      else
//             {
//             	STM_EVAL_LEDOff(LED3);
//             	STM_EVAL_LEDOff(LED4);
//             	STM_EVAL_LEDOff(LED5);
//             	STM_EVAL_LEDOff(LED6);

//             }
      Counter = 0x00;
    }
  }
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
