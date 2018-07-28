


#include "stm32f10x.h"

RCC_ClocksTypeDef RCC_ClockFreq;

void RCC_Configuration(void)
{
	SystemInit();//Դ��system_stm32f10x.c�ļ�,ֻ��Ҫ���ô˺���,������RCC������.�����뿴2_RCC

	/**************************************************
	��ȡRCC����Ϣ,������
	��ο�RCC_ClocksTypeDef�ṹ�������,��ʱ��������ɺ�,
	���������ֵ��ֱ�ӷ�ӳ�������������ֵ�����Ƶ��
	***************************************************/
	RCC_GetClocksFreq(&RCC_ClockFreq);
	
	/* ������ÿ�ʹ�ⲿ����ͣ���ʱ��,����һ��NMI�ж�,����Ҫ�õĿ����ε�*/
	//RCC_ClockSecuritySystemCmd(ENABLE);

	
	//SYSTICK��Ƶ--1ms��ϵͳʱ���ж�
	if (SysTick_Config(SystemCoreClock / 1000))
  	{ 
  	  	/* Capture error */ 
    	while (1);
  	}


}
 /********************************************
**������:SysTickDelay
**����:ʹ��ϵͳʱ�ӵ�Ӳ�ӳ�
**ע������:һ���,��Ҫ���ж��е��ñ�����,����������������.�������������ȫ���ж�,��Ҫʹ�ô˺���
********************************************/

volatile u16 Timer1;
void SysTickDelay(u16 dly_ms)
{
	Timer1=dly_ms;
	while(Timer1);
}
