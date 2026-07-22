#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h"
#include "./In_Flash_Config.h"

volatile uint8_t Received_FILE_Buffer1[Received_Buffer_Size]	__attribute__((aligned(4)));
volatile uint8_t Received_FILE_Buffer2[Received_Buffer_Size]	__attribute__((aligned(4)));

extern uint8_t Buffer1_Full_Flag;
extern uint8_t Buffer2_Full_Flag;
extern uint8_t Usart_Receive_Complete_Flag;

__ASM void Switch_To_APP(void)
{	
	cpsid i
	// set msp to new address	
	ldr r0, =0x08040000
	ldr r0, [r0]
	msr msp, r0
	cpsie i
	dsb
	isb

	// turn to reset_handler
	ldr r1, =0x08040004
	ldr r1, [r1]
	bx r1
	
	nop
}


int main(void)
{
	uint8_t * pWrite_Flash = NULL;
	uint8_t * pRead_Buffer = NULL;
	
	USART_Config();
	LED_GPIO_Config();

	USART_DMA_Config((uint32_t *)Received_FILE_Buffer1, 
										(uint32_t *)Received_FILE_Buffer2, 
										(uint32_t)Received_Buffer_Size);
	USART_DMACmd(DEBUG_USART, USART_DMAReq_Rx, ENABLE);
	
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
									FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR|FLASH_FLAG_RDERR);
	
	pWrite_Flash = (uint8_t*)Addr_Zone_APP1;
	while(FLASH_EraseSector(Sector_Zone_APP1, VoltageRange_3) != FLASH_COMPLETE)
		;
	DEBUG_INFO("FLASH erase and rewrite successful");
	
	LED1_ON;

	while(1)
		{
				if(Buffer1_Full_Flag == 1)
				{
					pRead_Buffer = (uint8_t *)Received_FILE_Buffer1;
					
					while(pRead_Buffer != Received_FILE_Buffer1 + Received_Buffer_Size)
					{
						while(FLASH_ProgramWord((uint32_t)pWrite_Flash,*(uint32_t *)pRead_Buffer) != FLASH_COMPLETE)
						{
							;
						}
						pRead_Buffer += 4;
						pWrite_Flash += 4;
					}
					Buffer1_Full_Flag = 0;

				}
				else if(Buffer2_Full_Flag == 1)
				{
					pRead_Buffer = (uint8_t *)Received_FILE_Buffer2;
					
					while(pRead_Buffer != Received_FILE_Buffer2 + Received_Buffer_Size)
					{
						while(FLASH_ProgramWord((uint32_t)pWrite_Flash,*(uint32_t *)pRead_Buffer) != FLASH_COMPLETE)
						{
							;
						}
						pRead_Buffer += 4;
						pWrite_Flash += 4;
					}
					Buffer2_Full_Flag = 0;
				}

				if(Usart_Receive_Complete_Flag == 1 && ((Buffer1_Full_Flag|Buffer2_Full_Flag) == 0))
				{
					Usart_Receive_Complete_Flag = 0;
					Switch_To_APP();
				}
		}
}



