#ifndef __BSP_ESP8266_H
#define __BSP_ESP8266_H

#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"

#define ESP_TX_PORT     GPIOB
#define ESP_RX_PORT     GPIOB
#define ESP_TX_PIN      GPIO_PIN_10
#define ESP_RX_PIN      GPIO_PIN_11
#define ESP_GPIO_AF     GPIO_AF7_USART3

#define ESP_EN_PORT     GPIOE
#define ESP_EN_PIN      GPIO_PIN_2
#define ESP_RST_PORT    GPIOG
#define ESP_RST_PIN     GPIO_PIN_15

#define ESP_USART       USART3
#define ESP_USART_BAUDRATE  115200

#define ESP_IRQ         USART3_IRQn
#define ESP_IRQHandler  USART3_IRQHandler

#define ESP8266_ENABLE      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET);
#define ESP8266_DISABLE     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
#define ESP8266_RST_High    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_SET);
#define ESP8266_RST_Low     HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_RESET);

#define MAX_LEN 1024

void ESP8266_Init(void);
void ESP_CMD_Send(const uint8_t* cmd, uint32_t timeout);


extern uint8_t RX_Buf[MAX_LEN];

#endif  //__BSP_ESP8266_H
