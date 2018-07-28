#include "stm32f10x.h"

u16 ADC_RCVTab[2];			//AD�������ݻ�����
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
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	 //��ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;			 //ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	 //����
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //�ⲿ����Դ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	 //�������и�ʽ
	ADC_InitStructure.ADC_NbrOfChannel = 2;					//ͨ����
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 2, ADC_SampleTime_13Cycles5);

	/* Enable ADC1 DMA [ʹ��ADC1 DMA]*/
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibaration register ADCУ׼ */   
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
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)0x4001244C;//�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_RCVTab;//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//dma���䷽����
	DMA_InitStructure.DMA_BufferSize = 2;//����DMA�ڴ���ʱ�������ĳ��� word (д��һ����Զ���ͷ��ʼ��
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//����DMA���ڴ����ģʽ��
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�ڴ������ֳ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//����DMA�Ĵ���ģʽ���������ϵ�ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//����DMA�����ȼ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//����DMA��2��memory�еı����������
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
}
