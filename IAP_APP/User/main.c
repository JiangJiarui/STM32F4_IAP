#include "stm32f4xx.h"                  // Device header
#include "./LED/bsp_led.h"
#include "./Timer/Timer.h"

void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = User_Timer1_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}

int main(void)
{
	NVIC_Config();
	LED_GPIO_Config();
	Timer_Config();
	
	while(1)
	{
		;
	}
		
	
}
