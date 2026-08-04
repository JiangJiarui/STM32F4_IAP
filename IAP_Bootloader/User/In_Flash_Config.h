#ifndef	__IN_FLASH_Config_H
#define	__IN_FLASH_Config_H



#define Addr_Zone_OP		((uint32_t)0x08020000)	//128kb
#define	Addr_Zone_APP1	((uint32_t)0x08040000)	//128kb
#define Addr_Zone_APP2	((uint32_t)0x08060000)	//128kb

#define APP_FLAG_DEFAULT	((uint32_t)0xFFFFFFFF)
#define	APP_FLAG_A				((uint32_t)0xAAAAAAAA)
#define APP_FLAG_B				((uint32_t)0x55555555)

#define	UPGRADE_FLAG_DEFAULT ((uint32_t)0xFFFFFFFF)
#define UPGRADE_FLAG_Set	((uint32_t)0xAAAAAAAA)
#define UPGRADE_FLAG_Reset ((uint32_t)0x55555555)

#define Sector_Zone_OP		FLASH_Sector_5
#define Sector_Zone_APP1	FLASH_Sector_6
#define Sector_Zone_APP2	FLASH_Sector_7
#define Secotr_Zone_Flag	FLASH_Sector_1

#define Sector_APP_Size  (128*1024)

#define DMA_Buffer_Size	256  //Byte


typedef enum{
	NONE, Buffer1, Buffer2, FINISH
}Buffer_State;



//#define Buffer1 1
//#define Buffer2 2
//#define FINISH 3
 


#endif
