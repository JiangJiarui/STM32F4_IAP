#include "./usart/bsp_usart.h"



static void NVIC_Config(void)
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

}

void USART_Config(void)
{
	/* GPIO */
	RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK|DEBUG_USART_TX_GPIO_CLK,ENABLE);
	/* USART */
  RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
	
/************* Interrupt Configuration *********************/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT, DEBUG_USART_TX_SOURCE, DEBUG_USART_TX_AF);
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT, DEBUG_USART_RX_SOURCE, DEBUG_USART_RX_AF);
	
/********************** USART ******************************/

	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(DEBUG_USART,&USART_InitStructure);

	NVIC_Config();
	
	USART_ITConfig(DEBUG_USART,USART_IT_IDLE, ENABLE);	

	USART_Cmd(DEBUG_USART, ENABLE);
}


/********************** DMA ******************************/
/* 
 * @param Memory_Address: Set the first buffer Address for DMA
 */
void USART_DMA_Config(uint32_t * Memory1_Address, uint32_t * Memory2_Address, uint32_t Buffer_Size)
{
	RCC_AHB1PeriphClockCmd(DEBUG_USART_DMA_CLK, ENABLE);
	DMA_DeInit(DEBUG_USART_DMA_STREAM);
	while(DMA_GetCmdStatus(DEBUG_USART_DMA_STREAM) != DISABLE)
	{
		;
	}
	
	DMA_InitTypeDef DMA_InitStructure;
  DMA_InitStructure.DMA_Channel = DEBUG_USART_DMA_CHANNEL;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = DEBUG_USART_DR_BASE;	 
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Memory1_Address;		
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
  DMA_InitStructure.DMA_BufferSize = Buffer_Size;		//
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 
	
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable; 
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; 
	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;    
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; 
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 

  DMA_Init(DEBUG_USART_DMA_STREAM, &DMA_InitStructure);
	
	DMA_DoubleBufferModeConfig(DEBUG_USART_DMA_STREAM, (uint32_t)Memory2_Address, DMA_Buffer1);
	DMA_DoubleBufferModeCmd(DEBUG_USART_DMA_STREAM, ENABLE);
	
	DMA_ITConfig(DEBUG_USART_DMA_STREAM, DMA_IT_TC, ENABLE);
	
	DMA_Cmd(DEBUG_USART_DMA_STREAM, ENABLE);
	
	
	while(DMA_GetCmdStatus(DEBUG_USART_DMA_STREAM) != ENABLE)
	{
		;
	}
}

void Usart_DMA_BufferSwitch(uint32_t MemoryBaseAddr, uint32_t DMA_MemoryTarget)
{
	DMA_MemoryTargetConfig(DEBUG_USART_DMA_STREAM, MemoryBaseAddr, DMA_MemoryTarget);
}


int fputc(int ch, FILE *f)
{
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USART);
}

/******************************** END ***********************************/



