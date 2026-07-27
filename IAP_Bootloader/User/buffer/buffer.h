#ifndef __BUFFER_H
#define __BUFFER_H

#include "stm32f4xx.h"                  // Device header

#define Ring_Buffer_Size 512
#define Usart_Frame_Size 138

#define CMD_WRITE 0xabcd
#define CMD_END 0xaecd


typedef struct{
		uint8_t  buf[Ring_Buffer_Size];
		uint8_t* pRead;
		uint8_t* pWrite;
		uint8_t* pEnd;
		volatile uint32_t BufferReadyForRead;
}Buffer;

typedef struct{
		uint16_t cmd;
		uint16_t length;
		uint8_t ppayload[Usart_Frame_Size];
		uint16_t crc;
}Frame;

void BufferInit(Buffer* str);
void write_byte(Buffer* pbuf, uint8_t byte);
void BufferReadyRelease(Buffer* pbuf);
uint8_t BufferReadyTake(Buffer* pbuf);




void BufferWrite(Buffer* str, uint8_t* src, uint16_t size);
void ClearBuffer(Buffer* pbuf);


uint8_t get_byte(Buffer* pbuf);
uint16_t get_halfword(Buffer* pbuf);
Frame* get_frame(Buffer* src, Frame* tag);


	


#endif	//__BUFFER_H
