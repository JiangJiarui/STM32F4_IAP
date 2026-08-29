/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include "./buffer/buffer.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "./eth/bsp_eth.h"
#include "./netconf.h"
#include "./client/client.h"


#define APP_FLAG_addr			((uint32_t)0x08004000)
#define UPGRADE_FLAG_addr ((uint32_t)0x08004004)
#define	APP_FLAG_A				((uint32_t)0xAAAAAAAA)
#define APP_FLAG_B				((uint32_t)0x55555555)
#define UPGRADE_FLAG_Set	((uint32_t)0xAAAAAAAA)
#define UPGRADE_FLAG_Reset ((uint32_t)0x55555555)

#define Secotr_Zone_Flag	FLASH_SECTOR_1

extern ETH_HandleTypeDef heth;
extern Buffer xBuffer;


#ifndef DEBUG_ERROR
  #define DEBUG_ERROR
#endif /* DEBUG_ERROR */
#ifndef DEBUG_INFO
  #define DEBUG_INFO
#endif /* DEBUG_INFO */

#endif /* __MAIN_H */
