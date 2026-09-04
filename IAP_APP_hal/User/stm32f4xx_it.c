/**
  ******************************************************************************
  * @file    Templates/Src/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
#include "./usart/bsp_debug_usart.h"
#include "FreeRTOS.h"
#include "task.h"


/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
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
//void SVC_Handler(void)
//{
//}

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
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{
  uint32_t ulReturn;
  ulReturn = taskENTER_CRITICAL_FROM_ISR();
  
  HAL_IncTick();
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
#endif  /* INCLUDE_xTaskGetSchedulerState */  
  xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
  }
#endif  /* INCLUDE_xTaskGetSchedulerState */
  
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/


/**
  * @brief 	This funciton handles Usart1 RX Flag interrupt and transmit back the same content
	* @param 	None
	* @retval None
  */ 
void  DEBUG_USART_IRQHandler(void)
{
	uint8_t temp;
  
	if(__HAL_UART_GET_FLAG( &UartHandle, UART_FLAG_RXNE ) != RESET)
	{		
		temp = (uint8_t)(UartHandle.Instance->DR & 0xFF);
		BufferWrite(&xBuffer, &temp, 1);
	}
}

void USART3_IRQHandler(void)
{
  uint8_t ch=0; 
  
	if(__HAL_UART_GET_FLAG(&ESPUartHandle, UART_FLAG_RXNE ) != RESET)
	{		
    ch =( uint16_t)READ_REG(ESPUartHandle.Instance->DR);
    WRITE_REG(UartHandle.Instance->DR,ch); 
	}
}

void ETH_IRQHandler(void)
{
    uint32_t ulReturn;

  ulReturn = taskENTER_CRITICAL_FROM_ISR();
  
  HAL_ETH_IRQHandler(&heth);
  
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

/**
  * @}
  */ 

/**
  * @}
  */
