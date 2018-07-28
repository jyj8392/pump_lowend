#include "stm32f10x.h"

u16 ADC_RCVTab[2];			//AD采样数据缓冲区
ADC_InitTypeDef    ADC_InitStructure;
DMA_InitTypeDef    DMA_InitStructure;

void ADC_Configuration(void)
{
	ADC_InitTypeDef   ADC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* ADC1 Configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	 //单ADC模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;			 //扫描
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	 //连续
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //外部触发源
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	 //数据排列格式
	ADC_InitStructure.ADC_NbrOfChannel = 2;					//通道数
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 2, ADC_SampleTime_13Cycles5);

	/* Enable ADC1 DMA [使能ADC1 DMA]*/
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibaration register ADC校准 */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));

	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

//********************DMA1 channel1 configuration**********************************
void DMA_Configuration(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)0x4001244C;//外设地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_RCVTab;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//dma传输方向单向
	DMA_InitStructure.DMA_BufferSize = 2;//设置DMA在传输时缓冲区的长度 word (写完一组后自动从头开始）
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//设置DMA的外设递增模式，一个外设
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//设置DMA的内存递增模式，
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//内存数据字长
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//设置DMA的传输模式：连续不断的循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//设置DMA的优先级别
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//设置DMA的2个memory中的变量互相访问
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
}
