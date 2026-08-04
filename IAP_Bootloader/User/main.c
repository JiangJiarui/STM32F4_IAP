#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h"
#include "./In_Flash_Config.h"
#include "./buffer/buffer.h"
#include "./timer/timer.h"

#define Header_len 4



volatile uint8_t Received_FILE_Buffer1[DMA_Buffer_Size]	__attribute__((aligned(4)));
volatile uint8_t Received_FILE_Buffer2[DMA_Buffer_Size]	__attribute__((aligned(4)));

const uint32_t APP_FLAG __attribute__((section("FLAG_ZONE1")));
const uint32_t UPGRADE_FLAG __attribute__((section("FLAG_ZONE2")));

uint8_t Receive_Complete_Flag = 0;
uint8_t wait_for_upgrade_timer_timeout = 0;

Buffer xRingBuffer;
Frame  xUsartFrame;

/********************** NVIC Configuration ******************************/

void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream2_IRQ;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = User_Timer1_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

void NVIC_DeConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream2_IRQ;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;

	NVIC_Init(&NVIC_InitStructure);
}
/****************************************************************/



__ASM void Switch_To_APP(void)
{	
	cpsid i
	// set msp to new address	
	ldr r0, =0x08020000			//Addr_Zone_OP
	ldr r0, [r0]
	msr msp, r0
	cpsie i
	dsb
	isb

	// turn to reset_handler
	ldr r1, =0x08020004
	ldr r1, [r1]
	bx r1
	
	nop
}

void Flash_Write(uint8_t* src, uint8_t* dest, uint32_t bufsize)
{
	uint8_t* pread = src;
	uint8_t* pwrite = dest;
	while(pread != src + bufsize)
	{
		while(FLASH_ProgramWord((uint32_t)pwrite,*(uint32_t *)pread) != FLASH_COMPLETE)
		{
			;
		}
		pread += 4;
		pwrite += 4;
	}	
}


void Send_ACK(void)
{
	Usart_SendByte(DEBUG_USART, 0xaa);
}

void Send_NACK(void)
{
	Usart_SendByte(DEBUG_USART, 0xff);
}

int main(void)
{
/********************* variable define *********************************/
	uint8_t* pWrite_Flash = NULL;
	uint32_t bytescount = 0;
	
/********************* NVIC configuration ********************************/
	NVIC_Config();
/********************* USART configuration *******************************/
	USART_Config();
	USART_ITConfig(DEBUG_USART,USART_IT_IDLE, ENABLE);
	USART_Cmd(DEBUG_USART, ENABLE);
/********************* LED configuration **********************************/
	LED_GPIO_Config();
	
/********************* DMA configuration *********************************/
	USART_DMA_Config((uint32_t *)Received_FILE_Buffer1, 
										(uint32_t *)Received_FILE_Buffer2, 
										(uint32_t)DMA_Buffer_Size);
	DMA_ITConfig(DEBUG_USART_DMA_STREAM, DMA_IT_TC, ENABLE);
	DMA_Cmd(DEBUG_USART_DMA_STREAM, ENABLE);
	while(DMA_GetCmdStatus(DEBUG_USART_DMA_STREAM) != ENABLE)
	{
		;
	}
	USART_DMACmd(DEBUG_USART, USART_DMAReq_Rx, ENABLE);
/********************* Timer configuration *********************************/
	Timer_Config();
	TIM_ClearFlag(User_Timer1,TIM_FLAG_Update);
	TIM_ITConfig(User_Timer1, TIM_IT_Update, ENABLE);

/************************************************************************/
	
	
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
									FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR|FLASH_FLAG_RDERR);
	
	if((APP_FLAG == APP_FLAG_DEFAULT)&&(UPGRADE_FLAG == UPGRADE_FLAG_DEFAULT))
	{
			FLASH_ProgramWord((uint32_t)&APP_FLAG, APP_FLAG_A);
			FLASH_ProgramWord((uint32_t)&UPGRADE_FLAG, UPGRADE_FLAG_Reset);
	}
	
	if(APP_FLAG == APP_FLAG_A)
	{
		pWrite_Flash = (uint8_t*)Addr_Zone_APP1;
		while(FLASH_EraseSector(Sector_Zone_APP1, VoltageRange_3) != FLASH_COMPLETE)
			;
	}
	else if(APP_FLAG == APP_FLAG_B)
	{
		pWrite_Flash = (uint8_t*)Addr_Zone_APP2;
		while(FLASH_EraseSector(Sector_Zone_APP2, VoltageRange_3) != FLASH_COMPLETE)
			;
	}
	
	BufferInit(&xRingBuffer);
	
	LED1_ON;
  TIM_Cmd(User_Timer1, ENABLE);

	while(1)
		{	
				if(get_frame(&xRingBuffer, &xUsartFrame) != 0)
				{
					if(xUsartFrame.cmd == CMD_UPGRADE)
					{
						if(crc_check(&xUsartFrame)!=Error)
						{
							if(UPGRADE_FLAG == UPGRADE_FLAG_Reset)
							{
								while(FLASH_EraseSector(Secotr_Zone_Flag, VoltageRange_3) != FLASH_COMPLETE)
									;
								if(APP_FLAG == APP_FLAG_A){
									FLASH_ProgramWord((uint32_t)&APP_FLAG, APP_FLAG_A);
								}
								else if(APP_FLAG == APP_FLAG_B){
									FLASH_ProgramWord((uint32_t)&APP_FLAG, APP_FLAG_B);
								}
								FLASH_ProgramWord((uint32_t)&UPGRADE_FLAG, UPGRADE_FLAG_Set);									
							}
						} // crc
						else
						{
							Send_NACK();
						}
						ClearFrame(&xUsartFrame);
					}
				}
			
				if(UPGRADE_FLAG == UPGRADE_FLAG_Set)
				{
					TIM_Cmd(User_Timer1, DISABLE);
					Send_ACK();
					while(1)
					{
							if(get_frame(&xRingBuffer, &xUsartFrame) != 0)
							{
								if(xUsartFrame.cmd == CMD_WRITE)
								{
									if(crc_check(&xUsartFrame)!=Error)
									{
										Flash_Write(xUsartFrame.ppayload, pWrite_Flash, xUsartFrame.length);
										pWrite_Flash += xUsartFrame.length;
										bytescount += xUsartFrame.length;
										Send_ACK();
									}
									else
									{
										Send_NACK();
									}
								}
								else if(xUsartFrame.cmd == CMD_END)
								{
									if(crc_check(&xUsartFrame)!=Error)
									{
										Receive_Complete_Flag = 1;
										Send_ACK();						
										break;
									}
									else
									{
										Send_NACK();
									}
								}
								ClearFrame(&xUsartFrame);
							}				
					} //while
					
					if(Receive_Complete_Flag)
					{
						while(FLASH_EraseSector(Sector_Zone_OP, VoltageRange_3) != FLASH_COMPLETE)
							;
						if(APP_FLAG == APP_FLAG_A)
						{
							Flash_Write((uint8_t*)Addr_Zone_APP1, (uint8_t*)Addr_Zone_OP, bytescount);
							FLASH_EraseSector(Secotr_Zone_Flag, VoltageRange_3);
							FLASH_ProgramWord((uint32_t)&APP_FLAG, APP_FLAG_B);
						}
						else if(APP_FLAG == APP_FLAG_B)
						{
							Flash_Write((uint8_t*)Addr_Zone_APP2, (uint8_t*)Addr_Zone_OP, bytescount);
							FLASH_EraseSector(Secotr_Zone_Flag, VoltageRange_3);
							FLASH_ProgramWord((uint32_t)&APP_FLAG, APP_FLAG_A);
						}
						
						FLASH_ProgramWord((uint32_t)&UPGRADE_FLAG, UPGRADE_FLAG_Reset);
						Usart_DeConfig();
						Switch_To_APP();
					}
			} //UPGRADE_FLAG == UPGRADE_FLAG_Set
				if(wait_for_upgrade_timer_timeout)
					{
						while(FLASH_EraseSector(Sector_Zone_OP, VoltageRange_3) != FLASH_COMPLETE)
							;
						if(APP_FLAG == APP_FLAG_A)
						{
								Flash_Write((uint8_t*)Addr_Zone_APP2, (uint8_t*)Addr_Zone_OP, Sector_APP_Size);
						}
						else if(APP_FLAG == APP_FLAG_B)
						{
								Flash_Write((uint8_t*)Addr_Zone_APP1, (uint8_t*)Addr_Zone_OP, Sector_APP_Size);
						}
						wait_for_upgrade_timer_timeout = 0;
						Switch_To_APP();
					}
	 } //while
}	//main


