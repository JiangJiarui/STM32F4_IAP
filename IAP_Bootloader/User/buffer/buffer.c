#include "buffer.h"

void BufferInit(Buffer* str)
{
	str->pEnd = str->buf + Ring_Buffer_Size;
	str->pRead = str->buf;
	str->pWrite = str->buf;
	str->BufferReadyForRead = 0;
}

void BufferReadyRelease(Buffer* pbuf)
{
	pbuf->BufferReadyForRead++;
}
uint8_t BufferReadyTake(Buffer* pbuf)
{
	if(pbuf->BufferReadyForRead)
	{
		pbuf->BufferReadyForRead--;
		return 1;
	}
	else
	{
		return 0;
	}
}

void WaitForBufferReady(Buffer* pbuf)
{
	while(BufferReadyTake(pbuf))
		;
}

void write_byte(Buffer* pbuf, uint8_t byte)
{
	*pbuf->pWrite = byte;
	pbuf->pWrite++;
	BufferReadyRelease(pbuf);
	if(pbuf->pWrite == pbuf->pEnd)
	{
		pbuf->pWrite = pbuf->buf;
	}
}

void BufferWrite(Buffer* str, uint8_t* src, uint16_t size)
{
	for(uint32_t i = 0; i < size; i++)
	{
		write_byte(str, *(src + i));
	}
}

void ClearBuffer(Buffer* pbuf)
{
	for(uint32_t i = 0; i < Ring_Buffer_Size; i++)
	{
		pbuf->buf[i] = 0;
	}
	pbuf->pRead = pbuf->buf;
	pbuf->pWrite = pbuf->buf;
}

uint8_t get_byte(Buffer* pbuf)
{
	uint8_t byte;
	

	byte = *(pbuf->pRead);
	
	if(pbuf->BufferReadyForRead)
	{
		pbuf->pRead++;
		pbuf->BufferReadyForRead--;
	}

	if(pbuf->pRead == pbuf->pEnd)
	{
		pbuf->pRead = pbuf->buf;
	}
		
	return byte;
}



uint16_t get_halfword(Buffer* pbuf)
{
	uint16_t half_L, half_H;
	
	half_L = (uint16_t)get_byte(pbuf);
	half_H = (uint16_t)get_byte(pbuf);
	
	return (half_H<<8u)|half_L;
}


Frame* get_frame(Buffer* src, Frame* tag)
{
	uint8_t current;
	while(1)
	{
		if(get_byte(src) == 0xcd)
		{
			current = *src->pRead;
			if(current == 0xab)
			{
				(void)get_byte(src);
				tag->cmd = CMD_WRITE;
				break;
			}
			else if(current == 0xae)
			{
				(void)get_byte(src);
				tag->cmd = CMD_END;
				break;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	while(!src->BufferReadyForRead)
		;
	tag->length = get_halfword(src);
	
	for(uint32_t i = 0; i < tag->length;)
	{
		if(src->BufferReadyForRead)
		{
			tag->ppayload[i] = get_byte(src);
			i++;
		}
	}
	
	for(uint8_t i = 4; i; )
	{
		if(src->BufferReadyForRead)
		{
			(void)get_byte(src);		//Reserved 4 bytes
			i--;
		}
	}

	while(src->BufferReadyForRead < 2u)
		;
	tag->crc = get_halfword(src);
	
	return tag;
}








