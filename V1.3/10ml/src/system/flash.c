#include "include.h"

#define FLASH_BASE_ADDR	(0x0807F000)			//0x0807F000-0x0807F7FF
#define FLASH_ADR_SETFLOW   (FLASH_BASE_ADDR + 0)	//16bit
#define FLASH_ADR_PURGEFLOW   (FLASH_BASE_ADDR + 2*1)	//16bit
#define FLASH_ADR_FLOWMODE   (FLASH_BASE_ADDR + 2*2)	//8bit
#define FLASH_ADR_PRESSOFFSET   (FLASH_BASE_ADDR + 2*3)	//16bit
#define FLASH_ADR_PRESSPROTECT   (FLASH_BASE_ADDR + 2*4)	//8bit
#define FLASH_ADR_PRESSMAX   (FLASH_BASE_ADDR + 2*5)	//16bit
#define FLASH_ADR_PRESSMIN   (FLASH_BASE_ADDR + 2*6)	//16bit
#define FLASH_ADR_BLACKLUMIN   (FLASH_BASE_ADDR + 2*7)	//8bit
#define FLASH_ADR_KEYBEEP   (FLASH_BASE_ADDR + 2*8)	//8bit

#define FLASH_ADR_FLOWPARA	(FLASH_BASE_ADDR + 2*10)

void Erase_Flash(u32 flash_addr)
{
	RCC_HSICmd(ENABLE);
	FLASH_Unlock();		/* 每次擦除Flash中数据时得先解锁 */
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(flash_addr);  
	FLASH_Lock();							   /* 上锁 */
	RCC_HSICmd(DISABLE);
}

void Write8bit_Flash(u32 flash_addr, u8 flash_data)
{
	RCC_HSICmd(ENABLE);
	FLASH_Unlock();		/* 每次擦除Flash中数据时得先解锁 */
//	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
//	FLASH_ErasePage(0x8002000);  
	FLASH_ProgramOptionByteData(flash_addr,flash_data);
	FLASH_Lock();							   /* 上锁 */
	RCC_HSICmd(DISABLE);
}

void Write16bit_Flash(u32 flash_addr, u16 flash_data)
{
	RCC_HSICmd(ENABLE);
	FLASH_Unlock();		/* 每次擦除Flash中数据时得先解锁 */
//	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
//	FLASH_ErasePage(flash_addr);  
	FLASH_ProgramHalfWord(flash_addr,flash_data);
	FLASH_Lock();							   /* 上锁 */
	RCC_HSICmd(DISABLE);
}

void Write32bit_Flash(u32 flash_addr, u32 flash_data)
{
	RCC_HSICmd(ENABLE);
	FLASH_Unlock();		/* 每次擦除Flash中数据时得先解锁 */
//	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
//	FLASH_ErasePage(0x8002000);  
	FLASH_ProgramWord(flash_addr,flash_data);
	FLASH_Lock();							   /* 上锁 */
	RCC_HSICmd(DISABLE);
}

u16 FlashImage_setFlow = 0;
u16 FlashImage_purgeFlow = 0;
u8 FlashImage_flowMode = 0;
u16 FlashImage_pressOffset = 0;
u8 FlashImage_pressProtect = 0;
u16 FlashImage_pressMax = 0;
u16 FlashImage_pressMin = 0;
u8 FlashImage_backLumin = 0;
u8 FlashImage_keyBeep = 0;
struct pumpflow_t FlashImage_flowPara[FLOWPATALEN];
	
void Read_Flash(struct sys_ctl *sysctl)
{
	u8 i = 0;
	sysctl->setFlow = *(u16*)(FLASH_ADR_SETFLOW);
	sysctl->purgeFlow = *(u16*)(FLASH_ADR_PURGEFLOW);
	sysctl->flowMode = *(u8*)(FLASH_ADR_FLOWMODE);
	sysctl->pressOffset = *(u16*)(FLASH_ADR_PRESSOFFSET);
	sysctl->pressProtect = *(u8*)(FLASH_ADR_PRESSPROTECT);
	sysctl->pressMax = *(u16*)(FLASH_ADR_PRESSMAX);
	sysctl->pressMin = *(u16*)(FLASH_ADR_PRESSMIN);
	sysctl->backLumin = *(u8*)(FLASH_ADR_BLACKLUMIN);
	sysctl->keyBeep = *(u8*)(FLASH_ADR_KEYBEEP);

	for (i = 0; i < sizeof(PumpCtl.flowPara); ++i)
		*((u8*)(PumpCtl.flowPara) + i) = *((u8*)(FLASH_ADR_FLOWPARA) + i);
	
	FlashImage_setFlow = *(u16*)(FLASH_ADR_SETFLOW);
	FlashImage_purgeFlow = *(u16*)(FLASH_ADR_PURGEFLOW);
	FlashImage_flowMode = *(u8*)(FLASH_ADR_FLOWMODE);
	FlashImage_pressOffset = *(u16*)(FLASH_ADR_PRESSOFFSET);
	FlashImage_pressProtect = *(u8*)(FLASH_ADR_PRESSPROTECT);
	FlashImage_pressMax = *(u16*)(FLASH_ADR_PRESSMAX);
	FlashImage_pressMin = *(u16*)(FLASH_ADR_PRESSMIN);
	FlashImage_backLumin = *(u8*)(FLASH_ADR_BLACKLUMIN);
	FlashImage_keyBeep = *(u8*)(FLASH_ADR_KEYBEEP);
	
	for (i = 0; i < sizeof(FlashImage_flowPara); ++i)
		*((u8*)(FlashImage_flowPara) + i) = *((u8*)(FLASH_ADR_FLOWPARA) + i);
	
	if (sysctl->backLumin > 9) {	//如果参数未初始化
		sysctl->setFlow = 0;
		sysctl->flowMode = 0;
		sysctl->pressMax = MAXPRESSURE * 100;
		sysctl->pressMin = 0;
		sysctl->purgeFlow = MAXFLOW * 1000;
		sysctl->backLumin = 9;
		sysctl->keyBeep = 1;
		sysctl->nowPress = 0;
		sysctl->pressOffset = 0;
		sysctl->pressProtect = 0;
	}
}

void Write_Flash(struct sys_ctl *sysctl)
{
	u8 isChangeFlowPara = 0;
	u8 i = 0;
	for (i = 0; i < sizeof(PumpCtl.flowPara); ++i) {
		if (*((u8*)(PumpCtl.flowPara) + i) != *((u8*)(FlashImage_flowPara) + i)) {
			isChangeFlowPara = 1;
			break;
		}
	}
	
	if (sysctl->setFlow != FlashImage_setFlow 
		|| sysctl->purgeFlow != FlashImage_purgeFlow
		|| sysctl->flowMode != FlashImage_flowMode
		|| sysctl->pressOffset != FlashImage_pressOffset
		|| sysctl->pressProtect != FlashImage_pressProtect
		|| sysctl->pressMax != FlashImage_pressMax
		|| sysctl->pressMin != FlashImage_pressMin
		|| sysctl->backLumin != FlashImage_backLumin
		|| sysctl->keyBeep != FlashImage_keyBeep
		|| isChangeFlowPara == 1) {
		isChangeFlowPara = 0;
		Erase_Flash(FLASH_BASE_ADDR);
		Write16bit_Flash(FLASH_ADR_SETFLOW, sysctl->setFlow);FlashImage_setFlow = sysctl->setFlow;
		Write16bit_Flash(FLASH_ADR_PURGEFLOW, sysctl->purgeFlow);FlashImage_purgeFlow = sysctl->purgeFlow;
		Write16bit_Flash(FLASH_ADR_FLOWMODE, sysctl->flowMode);FlashImage_flowMode = sysctl->flowMode;
		Write16bit_Flash(FLASH_ADR_PRESSOFFSET, sysctl->pressOffset);FlashImage_pressOffset = sysctl->pressOffset;
		Write16bit_Flash(FLASH_ADR_PRESSPROTECT, sysctl->pressProtect);FlashImage_pressProtect = sysctl->pressProtect;
		Write16bit_Flash(FLASH_ADR_PRESSMAX, sysctl->pressMax);FlashImage_pressMax = sysctl->pressMax;
		Write16bit_Flash(FLASH_ADR_PRESSMIN, sysctl->pressMin);FlashImage_pressMin = sysctl->pressMin;
		Write16bit_Flash(FLASH_ADR_BLACKLUMIN, sysctl->backLumin);FlashImage_backLumin = sysctl->backLumin;
		Write16bit_Flash(FLASH_ADR_KEYBEEP, sysctl->keyBeep);FlashImage_keyBeep = sysctl->keyBeep;
		
		for (i = 0; i < sizeof(PumpCtl.flowPara) / 2; ++i)
			Write16bit_Flash(FLASH_ADR_FLOWPARA + i*2, *((u16*)(PumpCtl.flowPara) + i));
		for (i = 0; i < sizeof(PumpCtl.flowPara) / 2; ++i) {
			*((u16*)(FlashImage_flowPara) + i) = *((u16*)(PumpCtl.flowPara) + i);

		}
	}
}
