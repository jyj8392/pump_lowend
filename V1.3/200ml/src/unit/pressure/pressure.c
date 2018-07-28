#include "include.h"

#define ON				1
#define OFF				0

#define P_CLK			GPIOA, GPIO_Pin_5
#define P_DAT			GPIOA, GPIO_Pin_6
#define P_CS			GPIOA, GPIO_Pin_4

#define ReadPData(dat)		(GPIO_ReadInputDataBit(dat))
#define SetPCTL(ctl, sw)	(sw == ON? GPIO_SetBits(ctl): GPIO_ResetBits(ctl))

void P_delay(void)
{
	int i;
	for (i = 0; i < 1000; i++);
}

extern u16 ADC_RCVTab[2];			//AD采样数据缓冲区
unsigned int ReadADValue(void)
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
	ret = ADC_RCVTab[1];
	return ret;

}


struct pumppress_t const PUMP_PressPara = {5000, 0};

unsigned int adValFFTArray[32];
unsigned int adValFFTIndex = 0;
#if PRESS_DEBUG == 0
unsigned int GetPressure(void)
{
	static double last_ret = 0;
	double ad = 0;
	double ret = 0;
	unsigned int i;
	
	unsigned int adVal = ReadADValue();
	if (adValFFTIndex < sizeof(adValFFTArray) / sizeof(u32) - 1) 
		adValFFTArray[adValFFTIndex++] = adVal;
	else {
		for(i = 0; i < sizeof(adValFFTArray) / sizeof(u32) - 1; ++i) {
			adValFFTArray[i] = adValFFTArray[i + 1];
		}
		adValFFTArray[sizeof(adValFFTArray) / sizeof(u32) - 1] = adVal;
	}
	ad = (double)(getFFT((short int*)adValFFTArray));
	
//	ret = ad;
	if (ad > PRESSPARA_S_SPLITE) {
		ret = PRESSPARA_S1_X4 * ad * ad * ad * ad + 
				PRESSPARA_S1_X3 * ad * ad * ad + 
				PRESSPARA_S1_X2 * ad * ad + 
				PRESSPARA_S1_X1 * ad + PRESSPARA_S1_X0;
	} else {
		ret = PRESSPARA_S2_X2 * ad * ad + 
				PRESSPARA_S2_X1 * ad + 
				PRESSPARA_S2_X0;
	}
	
	if (ret < 0.01) ret = 0;
	else if (ret > 99.99) ret = 99.99;
	
	if (last_ret - ret > 0.01 || ret - last_ret > 0.01)
		last_ret = ret;
	else
		ret = last_ret;	
		
	return (unsigned int)(ret * 100);
//	return (unsigned int)(ret);
}
#else
unsigned int GetPressure(void)
{
	static double last_ret = 0;
	double ad = 0;
	double ret = 0;
	
	unsigned int adVal = ReadADValue();
	if (adValFFTIndex < sizeof(adValFFTArray) / sizeof(u32) - 1) adValFFTArray[adValFFTIndex++] = adVal;
	else {
		for(unsigned int i = 0; i < sizeof(adValFFTArray) / sizeof(u32) - 1; ++i) {
			adValFFTArray[i] = adValFFTArray[i + 1];
		}
		adValFFTArray[sizeof(adValFFTArray) / sizeof(u32) - 1] = adVal;
	}
	ad = (double)(getFFT((short int*)adValFFTArray));
	
	ret = ad;
			
	return (unsigned int)(ret);
}


#endif