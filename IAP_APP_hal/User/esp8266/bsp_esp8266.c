#include "./esp8266/bsp_esp8266.h"


void ESP_CMD_Send(const uint8_t* cmd, uint32_t timeout)
{
  uint8_t* p = (uint8_t*)cmd;
  uint8_t len = 0;
  while(*p != '\0')
  {
    p++;
    len++;
  }
  HAL_UART_Transmit(&ESPUartHandle, cmd, len, timeout);
}






static void ESP_GPIO_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    __GPIOE_CLK_ENABLE();
    __GPIOG_CLK_ENABLE();

    GPIO_InitStruct.Pin = ESP_EN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP; 
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ESP_EN_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ESP_RST_PIN;
    HAL_GPIO_Init(ESP_RST_PORT, &GPIO_InitStruct);

}

void ESP8266_Init()
{
	ESP_GPIO_Config();
	ESP8266_RST_High;
  ESP8266_ENABLE;
}




