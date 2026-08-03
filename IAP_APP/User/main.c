#include "stm32f4xx.h"                  // Device header
#include "./LED/bsp_led.h"
#include "./Timer/Timer.h"
#include "./usart/bsp_usart.h"
#include "./buffer/buffer.h"


#define APP_FLAG_addr			((uint32_t)0x08004000)
#define UPGRADE_FLAG_addr ((uint32_t)0x08004004)
#define	APP_FLAG_A				((uint32_t)0xAAAAAAAA)
#define APP_FLAG_B				((uint32_t)0x55555555)
#define UPGRADE_FLAG_Set	((uint32_t)0xAAAAAAAA)
#define UPGRADE_FLAG_Reset ((uint32_t)0x55555555)

#define Secotr_Zone_Flag	FLASH_Sector_1


uint8_t upgrade_mess = 0;


Buffer xBuffer;
Frame xFrame;


__ASM void Switch_To_Boot(void)
{
	cpsid i
	// set msp to new address	
	ldr r0, =0x08000000			//Addr_Zone_Boot
	ldr r0, [r0]
	msr msp, r0
	cpsie i
	dsb
	isb

	// turn to reset_handler
	ldr r1, =0x08000004
	ldr r1, [r1]
	bx r1
}


void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = User_Timer1_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

void Send_NACK(void)
{
	Usart_SendByte(DEBUG_USART, 0xff);
}


int main(void)
{
	
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
									FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR|FLASH_FLAG_RDERR);

	NVIC_Config();
	LED_GPIO_Config();
	Timer_Config();
	USART_DeInit(DEBUG_USART);
	USART_Config();
	USART_ITConfig(DEBUG_USART,USART_IT_RXNE, ENABLE);	
	USART_Cmd(DEBUG_USART, ENABLE);

	BufferInit(&xBuffer);
	
//	DEBUG_INFO("Enter APP Program");
	
	while(1)
	{
		if(xBuffer.BufferReadyForRead >= Usart_Min_Frame_Size)
		{
			if(get_frame(&xBuffer, &xFrame) != 0)
			{
				if(xFrame.cmd == CMD_UPGRADE)
				{
					if(crc_check(&xFrame)!=Error)
						{
							if(*(uint32_t*)APP_FLAG_addr == APP_FLAG_A)
							{
								while(FLASH_EraseSector(Secotr_Zone_Flag, VoltageRange_3)!= FLASH_COMPLETE)
									;
								FLASH_ProgramWord(APP_FLAG_addr, APP_FLAG_A);
							}
							else if(*(uint32_t*)APP_FLAG_addr == APP_FLAG_B)
							{
								while(FLASH_EraseSector(Secotr_Zone_Flag, VoltageRange_3)!= FLASH_COMPLETE)
									;
								FLASH_ProgramWord(APP_FLAG_addr, APP_FLAG_B);
							}
							FLASH_ProgramWord(UPGRADE_FLAG_addr, UPGRADE_FLAG_Set);

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
