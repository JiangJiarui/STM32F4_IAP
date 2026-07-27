#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h"
#include "./In_Flash_Config.h"
#include "./buffer/buffer.h"



#define Header_len 4


volatile uint8_t Received_FILE_Buffer1[DMA_Buffer_Size]	__attribute__((aligned(4)));
volatile uint8_t Received_FILE_Buffer2[DMA_Buffer_Size]	__attribute__((aligned(4)));




extern uint8_t Buffer1_Full_Flag;
extern uint8_t Buffer2_Full_Flag;
extern uint8_t Usart_Receive_Complete_Flag;

uint32_t count = 0;
uint16_t Frame_ReadyToRead = 0;

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


typedef enum{
			Correct, Error
}Result;


			
void Send_ACK(void)
{
	Usart_SendByte(DEBUG_USART, 0xaa);
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
			
			#if 0
			if(*(uint16_t*)xRingBuffer.pRead == 0xabcd)
			{
				uint16_t len = *(uint16_t*)(xRingBuffer.pRead + 2);
				Flash_Write(xRingBuffer.pRead+4, pWrite_Flash, len);
				pWrite_Flash = pWrite_Flash + len;
				
				for(uint32_t i = len + 10; i; i--)
				{
					xRingBuffer.pRead ++;
					if(xRingBuffer.pRead == xRingBuffer.pEnd)
					{
						xRingBuffer.pRead = xRingBuffer.buf;
					}
				}
			}
			#endif
#if 1			

			if(get_frame(&xRingBuffer, &xUsartFrame) != 0)
			{
				count++;
				if(xUsartFrame.cmd == CMD_WRITE && Usart_Receive_Complete_Flag ==0)
				{
					Flash_Write(xUsartFrame.ppayload, pWrite_Flash, xUsartFrame.length);
					pWrite_Flash += xUsartFrame.length;

				}
				else if(xUsartFrame.cmd == CMD_END)
				{
					Usart_Receive_Complete_Flag = 1;		//avoid from loop in the RingBuffer
					ClearBuffer(&xRingBuffer);

					//Switch_To_APP();				
				}
				
			}
			
#endif				
			
			
		}
}



