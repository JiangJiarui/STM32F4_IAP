#ifndef __DEBUG_USART_H
#define	__DEBUG_USART_H

#include "stm32f4xx.h"
#include <stdio.h>


#define DEBUG_USART_BAUDRATE                    115200


/*******************************************************/
#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK_ENABLE()                __USART1_CLK_ENABLE();

#define RCC_PERIPHCLK_UARTx                     RCC_PERIPHCLK_USART1
#define RCC_UARTxCLKSOURCE_SYSCLK               RCC_USART1CLKSOURCE_SYSCLK

#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK_ENABLE()        __GPIOA_CLK_ENABLE()
#define DEBUG_USART_RX_PIN                      GPIO_PIN_10
#define DEBUG_USART_RX_AF                       GPIO_AF7_USART1

#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK_ENABLE()        __GPIOA_CLK_ENABLE()
#define DEBUG_USART_TX_PIN                      GPIO_PIN_9
#define DEBUG_USART_TX_AF                       GPIO_AF7_USART1

#define DEBUG_USART_IRQHandler                  USART1_IRQHandler
#define DEBUG_USART_IRQ                 		    USART1_IRQn


#define DEBUG_ERROR(fmt,arg...)          printf("<<-DEBUG-ERROR->> "fmt"\n",##arg)
#define DEBUG_INFO(fmt,arg...)          printf("<<-DEBUG-INFO->> "fmt"\n",##arg)


/************************************************************/

void Usart_SendString(uint8_t *str);
void Usart_SendByte(uint8_t byte);

void USART_Config(void);

extern UART_HandleTypeDef UartHandle;
extern UART_HandleTypeDef ESPUartHandle;


#endif /* __USART1_H */
