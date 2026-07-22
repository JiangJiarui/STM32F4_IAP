#ifndef	__IN_FLASH_Config_H
#define	__IN_FLASH_Config_H



#define Addr_Zone_OP		((uint32_t)0x08020000)	//128kb
#define	Addr_Zone_APP1	((uint32_t)0x08040000)	//128kb
#define Addr_Zone_APP2	((uint32_t)0x08060000)	//128kb


#define Sector_Zone_OP			FLASH_Sector_5
#define Sector_Zone_APP1	FLASH_Sector_6
#define Sector_Zone_APP2	FLASH_Sector_7

#define Received_Buffer_Size	4  //Byte



typedef enum{
	NONE, Buffer1, Buffer2, FINISH
}Buffer_State;

extern Buffer_State eCurrent_Receiving_Buffer;		//define at _it.c



//#define Buffer1 1
//#define Buffer2 2
//#define FINISH 3
 


#endif
