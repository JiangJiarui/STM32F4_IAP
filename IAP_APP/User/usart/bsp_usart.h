#ifndef __BSP_USART_H
#define __BSP_USART_H



#include "stm32f4xx.h"                  // Device header
#include <stdio.h>


#define	Usart_DMA	1


#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK                         RCC_APB2Periph_USART1
#define DEBUG_USART_BAUDRATE                    115200

#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_RX_PIN                      GPIO_Pin_10
#define DEBUG_USART_RX_AF                       GPIO_AF_USART1
#define DEBUG_USART_RX_SOURCE                   GPIO_PinSource10

#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_TX_PIN                      GPIO_Pin_9
#define DEBUG_USART_TX_AF                       GPIO_AF_USART1
#define DEBUG_USART_TX_SOURCE                   GPIO_PinSource9

#define DEBUG_USART_IRQHandler                  USART1_IRQHandler
#define DEBUG_USART_IRQ                 				USART1_IRQn

#define DEBUG_ERROR(fmt,arg...)          printf("<<-DEBUG-ERROR->> "fmt"\n",##arg)
#define DEBUG_INFO(fmt,arg...)          printf("<<-DEBUG-INFO->> "fmt"\n",##arg)

/* Usart1_RX DMA2 Channel 4 Stream 2	*/
#define DEBUG_USART_DMA_CLK 										RCC_AHB1Periph_DMA2
#define	DEBUG_USART_DMA_CHANNEL									DMA_Channel_4
#define	DEBUG_USART_DMA_STREAM									DMA2_Stream2

#define	DMA2_Stream2_IRQ							DMA2_Stream2_IRQn

#define DEBUG_USART_DR_BASE               (USART1_BASE+0x04)	
#define DMA_Buffer1													DMA_Memory_0
#define DMA_Buffer2													DMA_Memory_1




#define	Usart1_SR_IDLE		((uint32_t)0x40C11000 & (1<<4))
#define	DMA2_S2CR_Addr		((uint32_t)0x40026400 + (uint32_t)0x10 + (uint32_t)0x30)
#define	DMA2_S2CR_CT			(DMA2_S2CR_Addr & (1<<19))

#define DMA_ON()		DMA_Cmd(DEBUG_USART_DMA_STREAM, ENABLE)
#define DMA_OFF()		DMA_Cmd(DEBUG_USART_DMA_STREAM, DISABLE)



void USART_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

void USART_DMA_Config(uint32_t * Memory1_Address, uint32_t * Memory2_Address, uint32_t Buffer_Size);
void Usart_DMA_BufferSwitch(uint32_t MemoryBaseAddr, uint32_t DMA_MemoryTarget);



#endif	// __BSP_USART_H
