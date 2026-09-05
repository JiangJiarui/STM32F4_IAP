/**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
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



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* TaskHandle */
static TaskHandle_t StartupTask_Handle = NULL;
static TaskHandle_t LEDTask_Handle = NULL;
static TaskHandle_t CheckUpgradeTask_Handle = NULL;
static TaskHandle_t ESP_StartupTask_Handle = NULL;
/**************/
Buffer xBuffer;
Frame xFrame;


/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void GPIO_CLK_Init(void);
void Send_NACK(void);

/* Thread */
static void StartupTask(void);
static void LED_Task(void);
static void CheckUpgrade_Task(void);
static void ESP_StartupTask(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  BaseType_t xReturn = pdPASS;

/* Hardware Pehripherals Init *********************************************************/
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
             can eventually implement his proper time base source (a general purpose 
             timer for example or other time source), keeping in mind that Time base 
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
             handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();
	GPIO_CLK_Init();
  /* Configure the system clock to 168 MHz */
  SystemClock_Config();
  
  HAL_FLASH_Unlock();
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
									FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
  HAL_FLASH_Lock();
	LED_GPIO_Config();
  USART_Config();
	ESP8266_Init();

  BufferInit(&xBuffer);

  printf("Pehripherals initialized success\r\n");

/** Startup Task create *****************************************************************/
  xReturn = xTaskCreate((TaskFunction_t )StartupTask,
                        (const char*    )"StartupTask",
                        (uint16_t       )512, /* Stack Depth */
                        (void*          )NULL,
                        (UBaseType_t    )1, /* Prioirity */
                        (TaskHandle_t*  )&StartupTask_Handle);
	
  if(pdPASS == xReturn){
		DEBUG_ERROR("StartupTask created success");
    vTaskStartScheduler();   
  }else{
    DEBUG_ERROR("StartupTask created fail");
  }


  /* Infinite loop */
  while (1)
  {
    ; /* should never execute */
  }
}

/**
  * @brief  StartupTask initialize Eth and create functional threads, only enter once
  * @param  None
  * @retval None
  */
static void StartupTask(void)
{
  BaseType_t xReturn = pdPASS;

/** ETH Init *******************************/  	
  LwIP_Init();           
	client_init();
	
	printf("Network configuration finished\r\n");
	
/** Threads Create *************************/
taskENTER_CRITICAL();           

  xReturn = xTaskCreate((TaskFunction_t )LED_Task,
                        (const char*    )"LED_Task",
                        (uint16_t       )128, /* Stack Depth */
                        (void*          )NULL,
                        (UBaseType_t    )1, /* Prioirity */
                        (TaskHandle_t*  )&LEDTask_Handle);
  if(pdPASS == xReturn){
    DEBUG_INFO("Task1 created success");
  }else{
    DEBUG_ERROR("Task1 created fail");
  }

  xReturn = xTaskCreate((TaskFunction_t )CheckUpgrade_Task,
                        (const char*    )"CheckUpgrade_Task",
                        (uint16_t       )1024, /* Stack Depth */
                        (void*          )NULL,
                        (UBaseType_t    )3, /* Prioirity */
                        (TaskHandle_t*  )&CheckUpgradeTask_Handle);
  if(pdPASS == xReturn){
    DEBUG_INFO("CheckUpgrade_Task created success");
  }else{
    DEBUG_ERROR("CheckUpgrade_Task created fail");
  }

  xReturn = xTaskCreate((TaskFunction_t )ESP_StartupTask,
                        (const char*    )"ESP_StartupTask",
                        (uint16_t       )1024, /* Stack Depth */
                        (void*          )NULL,
                        (UBaseType_t    )4, /* Prioirity */
                        (TaskHandle_t*  )&ESP_StartupTask_Handle);
  if(pdPASS == xReturn){
    DEBUG_INFO("CheckUpgrade_Task created success");
  }else{
    DEBUG_ERROR("CheckUpgrade_Task created fail");
  }


taskEXIT_CRITICAL();           

  vTaskDelete(NULL); 
}

/**
  * @brief  Check for upgrade signal and switch to bootloader 
  * @param  None
  * @retval None
  */
static void CheckUpgrade_Task(void)
{
  uint32_t SectorError;

  FLASH_EraseInitTypeDef FLASH_EraseInitStucture;
  FLASH_EraseInitStucture.TypeErase = FLASH_TYPEERASE_SECTORS;
  FLASH_EraseInitStucture.Sector = Secotr_Zone_Flag;
  FLASH_EraseInitStucture.NbSectors = 1;
  FLASH_EraseInitStucture.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  while(1)
    {
			vTaskDelay(1);
      if(xBuffer.BufferReadyForRead >= Usart_Min_Frame_Size)
      {
        if(get_frame(&xBuffer, &xFrame) != 0)
        {
          if(xFrame.cmd == CMD_UPGRADE)
          {
            if(crc_check(&xFrame)!=Error)
              {
                HAL_FLASH_Unlock();
                if(*(uint32_t*)APP_FLAG_addr == APP_FLAG_A)
                {
                  while(HAL_FLASHEx_Erase(&FLASH_EraseInitStucture, &SectorError)== HAL_OK)
                    ;
                  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, APP_FLAG_addr, APP_FLAG_A);
                }
                else if(*(uint32_t*)APP_FLAG_addr == APP_FLAG_B)
                {
                  while(HAL_FLASHEx_Erase(&FLASH_EraseInitStucture, &SectorError)!= HAL_OK)
                    ;
                  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,APP_FLAG_addr, APP_FLAG_B);
                }
                HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, UPGRADE_FLAG_addr, UPGRADE_FLAG_Set);
                HAL_FLASH_Lock();
                vTaskDelay(10);
                NVIC_SystemReset();
              }
          else
            {
                Send_NACK();
            }
          }
        }
      }
	 }
}


/**
  * @brief  LED toggle Task
  * @param  None
  * @retval None
  */
static void LED_Task(void)
{
  while (1)
  {
    LED2_TOGGLE;
    vTaskDelay(1000);
  }
}

/**
  * @brief  Startup esp8266 by uart communication
  * @param  None
  * @retval None
  */
void ESP_StartupTask(void)
{
    vTaskDelay(1000);
  /* test AT launch */
  ESP_CMD_Send((uint8_t *)"AT\r\n", 50);

  vTaskDelay(100);
  /* choose WIFI mode: 1 Station; 2 AP; 3 AP+Station */
  ESP_CMD_Send((uint8_t *)"AT+CWMODE=1\r\n", 50);
	
	vTaskDelay(100);
  /* choose WIFI mode: 1 Station; 2 AP; 3 AP+Station */
 
  ESP_CMD_Send((uint8_t *)"AT+CWJAP_DEF=\"601\",\"12167586\"\r\n", 1000);

  vTaskDelay(10000);
  ESP_CMD_Send((uint8_t *)"AT+CIFSR\r\n", 50);
	
  vTaskDelay(500);
  ESP_CMD_Send((uint8_t *)"AT+CIPMUX=0\r\n", 50);

  vTaskDelay(100);
  ESP_CMD_Send((uint8_t *)"AT+CIPSTART=\"TCP\",\"192.168.2.6\",8080\r\n", 1000);

  vTaskDelay(3000);
  vTaskDelete(NULL);
}

void Send_NACK(void)
{
	Usart_SendByte(0xff);
}

/*********************************  Application Code End  ************************************************************/



/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;	
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;	
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
	
	/* SysTick Configuration */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/configTICK_RATE_HZ);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);


  /* STM32F405x/407x/415x/417x Revision Z and upper devices: prefetch is supported  */
  if (HAL_GetREVID() >= 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

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
  * 
  */ 
static void GPIO_CLK_Init(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
}

/**
  * @}
  */ 
