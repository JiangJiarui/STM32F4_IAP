/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "./usart/bsp_usart.h"
#include "./In_Flash_Config.h"
#include "./buffer/buffer.h"
#include "./timer/timer.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern volatile uint8_t Received_FILE_Buffer1[];
extern volatile uint8_t Received_FILE_Buffer2[];
extern Buffer xRingBuffer;

extern uint8_t Receive_Complete_Flag;
extern uint8_t wait_for_upgrade_timer_timeout;


uint8_t Buffer1_Full_Flag = 0;
uint8_t Buffer2_Full_Flag = 0;
uint8_t offset = 0;
uint8_t old_offset1 = 0;
uint8_t old_offset2 = 0;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
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
  //TimingDelay_Decrement();
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
void DMA2_Stream2_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DEBUG_USART_DMA_STREAM, DMA_FLAG_TCIF2) != RESET)
	{
		if((((DEBUG_USART_DMA_STREAM->CR) & (1<<19))>>19) == SET)
		{
			Buffer1_Full_Flag = 1;
					
		}
		else
		{
			Buffer2_Full_Flag = 1;		
		}
		DMA_ClearITPendingBit(DEBUG_USART_DMA_STREAM, DMA_FLAG_TCIF2);
	}
}




void DEBUG_USART_IRQHandler(void)
{
	
		if(USART_GetITStatus(DEBUG_USART, USART_IT_IDLE) != RESET)
		{
				USART_ReceiveData(DEBUG_USART);		//Clear IDLE FLAG
			
				offset = DMA_Buffer_Size - DEBUG_USART_DMA_STREAM->NDTR;
			
				if(Buffer1_Full_Flag|Buffer2_Full_Flag)
				{
						if(Buffer1_Full_Flag)
						{
							BufferWrite(&xRingBuffer, (uint8_t*)Received_FILE_Buffer1 + old_offset1, DMA_Buffer_Size - old_offset1);
							old_offset1 = 0;
							BufferWrite(&xRingBuffer, (uint8_t*)Received_FILE_Buffer2, offset);
							old_offset2 = offset;
							Buffer1_Full_Flag = 0;
						}
						else
						{
							BufferWrite(&xRingBuffer, (uint8_t*)Received_FILE_Buffer2 + old_offset2, DMA_Buffer_Size - old_offset2);
							old_offset2 = 0;
							BufferWrite(&xRingBuffer, (uint8_t*)Received_FILE_Buffer1, offset);
							old_offset1 = offset;
							Buffer2_Full_Flag = 0;
						}
				}
				else
				{
					if((((DEBUG_USART_DMA_STREAM->CR) & (1<<19))>>19) == SET)
					{
						BufferWrite(&xRingBuffer, (uint8_t*)Received_FILE_Buffer2 + old_offset2, offset - old_offset2);
						old_offset2 = offset;
					}
					else
					{
						BufferWrite(&xRingBuffer, (uint8_t*)Received_FILE_Buffer1 + old_offset1, offset - old_offset1);
						old_offset1 = offset;
					}
				}
		
				if(Receive_Complete_Flag != 0)
				{
					offset = 0;
					old_offset1 = 0;
					old_offset2 = 0;
				}			
		}		//USART IDLE

		
}

void User_Timer1_IRQHandler(void)
{
  if(TIM_GetITStatus(User_Timer1, TIM_IT_Update) != RESET)
  {
    wait_for_upgrade_timer_timeout = 1;
		TIM_ClearITPendingBit(User_Timer1, TIM_IT_Update);
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
