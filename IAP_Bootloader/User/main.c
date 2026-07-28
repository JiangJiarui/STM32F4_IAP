#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h"
#include "./In_Flash_Config.h"
#include "./buffer/buffer.h"



#define Header_len 4


volatile uint8_t Received_FILE_Buffer1[DMA_Buffer_Size]	__attribute__((aligned(4)));
volatile uint8_t Received_FILE_Buffer2[DMA_Buffer_Size]	__attribute__((aligned(4)));

uint8_t Receive_Complete_Flag = 0;

Buffer xRingBuffer;
Frame  xUsartFrame;

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

void Flash_Write(uint8_t* src, uint8_t* dest, uint16_t bufsize)
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
	uint8_t* pWrite_Flash = NULL;
	
	USART_Config();
	LED_GPIO_Config();

	USART_DMA_Config((uint32_t *)Received_FILE_Buffer1, 
										(uint32_t *)Received_FILE_Buffer2, 
										(uint32_t)DMA_Buffer_Size);
	USART_DMACmd(DEBUG_USART, USART_DMAReq_Rx, ENABLE);
	
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
									FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR|FLASH_FLAG_RDERR);
	
	pWrite_Flash = (uint8_t*)Addr_Zone_APP1;
	while(FLASH_EraseSector(Sector_Zone_APP1, VoltageRange_3) != FLASH_COMPLETE)
		;
	DEBUG_INFO("FLASH erase successful");
	
	
	BufferInit(&xRingBuffer);
	
	LED1_ON;

	
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
						Switch_To_APP();
					}
					else
					{
						Send_NACK();
					}
				}
				ClearFrame(&xUsartFrame);
			}			
			
		}
}



