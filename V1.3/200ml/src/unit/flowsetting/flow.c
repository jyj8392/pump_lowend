#include "include.h"

#define ON				1
#define OFF				0

#define F_CLK			GPIOB, GPIO_Pin_13
#define F_DAT			GPIOB, GPIO_Pin_14
#define F_CS			GPIOB, GPIO_Pin_12

#define ReadFData(dat)		(GPIO_ReadInputDataBit(dat))
#define SetFCTL(ctl, sw)	(sw == ON? GPIO_SetBits(ctl): GPIO_ResetBits(ctl))

void F_delay(void)
{
	int i;
	for (i = 0; i < 1000; i++);
}

extern u16 ADC_RCVTab[2];			//AD采样数据缓冲区
unsigned int ReadFADValue(void)
{
	unsigned int ret = 0;
    unsigned char i = 0;
	unsigned char data1 = 0;
//	SetPCTL(P_DAT, ON);
/*	SetPCTL(P_CLK, OFF);
	P_delay();
	SetPCTL(P_CS, ON);
	SetPCTL(P_CS, OFF);
	SetPCTL(P_CLK, ON);
	SetPCTL(P_CLK, OFF);
	SetPCTL(P_CLK, ON);
	SetPCTL(P_CLK, OFF);
	SetPCTL(P_CLK, ON);

	for(i = 0; i < 12; i++) {
		SetPCTL(P_CLK, OFF);
		P_delay();
		data1 = ReadPData(P_DAT);
		if(data1 == 1) {
			back_up |= (0x0800>>i);
		}
		SetPCTL(P_CLK, ON);
	}
	SetPCTL(P_CS, ON);*/
	ret = ADC_RCVTab[0];
	return ret;

}



unsigned int fadValFFTArray[32];
unsigned int fadValFFTIndex = 0;
unsigned int GetFLOWPLC(void)
{
	static double last_ret = 0;
	double ad = 0;
	double ret = 0;
	unsigned int i;
	
	unsigned int adVal = ReadFADValue();
	if (fadValFFTIndex < 31) fadValFFTArray[fadValFFTIndex++] = adVal;
	else {
		for(i = 0; i < 31; ++i) {
			fadValFFTArray[i] = fadValFFTArray[i + 1];
		}
		fadValFFTArray[31] = adVal;
	}
	ad = (double)(getFFT((short int*)fadValFFTArray));
	
	ret = ad;
			
	return (unsigned int)(ret);
}
