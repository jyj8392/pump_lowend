#include "include.h"

/*********************************************************************************************************
**                           ±¡Ä¤¼üÅÌÓ³Éä
********************************************************************************************************/
//#define ISKEY1()			(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7))	   
//#define ISKEY2()			(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8))	 
//#define ISKEY3()			(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14))		 
//#define ISKEY4()			(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12))		
//#define ISKEY5()			(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15))		
//#define ISKEY6()			(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10))		
//#define ISKEY7()			(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8))		
//#define ISKEY8()			(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9))		
//#define ISKEY9()			(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10))		
//#define ISKEY10()			(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13))		
//#define ISKEY11()			(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12))		
//#define ISKEY12()			(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9))	
//#define ISKEY13()			(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11))		
//#define ISKEY14()			(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13))	 
//#define ISKEY15()			(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10))	 
//#define ISKEY16()			(!GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_4))	 
//#define ISKEY17()			(!GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_7))	 
//#define ISKEY18()			(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11))	 
//#define ISKEY19()			(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11))	 
#define IO1			GPIOE,GPIO_Pin_7	   
#define IO2			GPIOE,GPIO_Pin_8	 
#define IO3			GPIOE,GPIO_Pin_14		 
#define IO4			GPIOE,GPIO_Pin_12		
#define IO5			GPIOE,GPIO_Pin_15		
#define IO6			GPIOE,GPIO_Pin_10		
#define IO7			GPIOD,GPIO_Pin_8		
#define IO8			GPIOE,GPIO_Pin_9		
#define IO9			GPIOB,GPIO_Pin_10		
#define IO10			GPIOE,GPIO_Pin_13		
#define IO11			GPIOD,GPIO_Pin_12		
#define IO12			GPIOD,GPIO_Pin_9	
//#define IO13			GPIOB,GPIO_Pin_11		
//#define IO14			GPIOD,GPIO_Pin_13	 
//#define IO15			GPIOD,GPIO_Pin_10	 
//#define IO16			GPIOG,GPIO_Pin_4	 
//#define IO17			GPIOG,GPIO_Pin_7	 
//#define IO18			GPIOD,GPIO_Pin_11	 
//#define IO19			GPIOE,GPIO_Pin_11	 

#define SCANCOL1()	GPIO_ResetBits(IO12)								

#define SCANCOL2()	GPIO_ResetBits(IO11)

#define SCANCOL3()	GPIO_ResetBits(IO10)

#define SCANCOL4()	GPIO_ResetBits(IO9)									

#define ISKEYNUM1	(!GPIO_ReadInputDataBit(IO1))
#define ISKEYNUM2	(!GPIO_ReadInputDataBit(IO2))
#define ISKEYNUM3	(!GPIO_ReadInputDataBit(IO3))
#define ISKEYNUM4	(!GPIO_ReadInputDataBit(IO4))
#define ISKEYNUM5	(!GPIO_ReadInputDataBit(IO5))
#define ISKEYNUM6	(!GPIO_ReadInputDataBit(IO6))
#define ISKEYNUM7	(!GPIO_ReadInputDataBit(IO7))
#define ISKEYNUM8	(!GPIO_ReadInputDataBit(IO8))
#define ISKEYNUM9	(!GPIO_ReadInputDataBit(IO1))
#define ISKEYNUM0	(!GPIO_ReadInputDataBit(IO2))
#define ISKEYENT	(!GPIO_ReadInputDataBit(IO2))
#define ISKEYTAB	(!GPIO_ReadInputDataBit(IO3))
#define ISKEYRIGHT	(!GPIO_ReadInputDataBit(IO4))
#define ISKEYLEFT	(!GPIO_ReadInputDataBit(IO5))
#define ISKEYPUMP	(!GPIO_ReadInputDataBit(IO1))
#define ISKEYSTOP	(!GPIO_ReadInputDataBit(IO2))
#define ISKEYPURGE	(!GPIO_ReadInputDataBit(IO4))
					
void ScanKey(void)
{
	u32 val = 0xffffffff;
	GPIO_InitTypeDef GPIO_InitStructure;


	while (1) {
		SCANCOL1();
		if (ISKEYNUM1) 
			val &= 0xfffffffe;
		if (ISKEYNUM1) 
			val &= 0xfffffffd;
		if (!GPIO_ReadInputDataBit(IO3)) 
			val &= 0xfffffffb;
		if (!GPIO_ReadInputDataBit(IO4)) 
			val &= 0xfffffff7;
		if (!GPIO_ReadInputDataBit(IO5)) 
			val &= 0xffffffef;
		if (!GPIO_ReadInputDataBit(IO6)) 
			val &= 0xffffffdf;
		if (!GPIO_ReadInputDataBit(IO7)) 
			val &= 0xffffffbf;
		if (!GPIO_ReadInputDataBit(IO8)) 
			val &= 0xffffff7f;
		
	}
}
