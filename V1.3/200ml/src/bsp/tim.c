

#include "stm32f10x.h"


void TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//---------------------------------------------------------------------------------------
	/* TIM2 clock enable */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* 基础设置*/
	TIM_TimeBaseStructure.TIM_Period = 2000;			//计数值   
	TIM_TimeBaseStructure.TIM_Prescaler = 36-1;    	//预分频,此值+1为分频的除数
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
//	/* 比较通道1*/
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;      		//输出比较非主动模式
//	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;  
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//极性为正
//	  
//	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
//	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);				//禁止OC1重装载,其实可以省掉这句,因为默认是4路都不重装的.
//	
//	/*比较通道2 */        
//	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;  
//	
//	TIM_OC2Init(TIM2, &TIM_OCInitStructure); 
//	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);
	
//	/* 比较通道3 */         
//	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;  
//	
//	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
//	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);
//	
//	/* 比较通道4 */       
//	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;  
//	
//	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
//	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);
	
	/*使能预装载*/
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	/*预先清除所有中断位*/
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4|TIM_IT_Update); 

	/* 4个通道和溢出都配置中断*/
//	TIM_ITConfig(TIM2, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4|TIM_IT_Update, ENABLE);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	/* 允许TIM2开始计数 */
	TIM_Cmd(TIM2, DISABLE);



//---------------------------------------------------------------------------------------
    /* TIM3 clock enable */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* 基础设置*/
	TIM_TimeBaseStructure.TIM_Period = 100;			//计数值   
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;    	//预分频,此值+1为分频的除数
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/*使能预装载*/
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	/*预先清除所有中断位*/
//	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4|TIM_IT_Update); 

	/* 4个通道和溢出都配置中断*/
//	TIM_ITConfig(TIM2, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4|TIM_IT_Update, ENABLE);

		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update); 
	

	/* 允许TIM3开始计数 */
	TIM_Cmd(TIM3, DISABLE);
//---------------------------------------------------------------------------------------
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	/* 基础设置*/
	TIM_TimeBaseStructure.TIM_Period = 100;			//计数值   
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;    	//预分频,此值+1为分频的除数
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	/*使能预装载*/
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	/*预先清除所有中断位*/
	TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4|TIM_IT_Update); 

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	/* 允许TIM4开始计数 */
	TIM_Cmd(TIM4, DISABLE);

}

void SetPulse(void)
{
	int i, j, k;
	TIM2->ARR = 2000;
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
//	for (i = 2000; i > 10; i--) {
//		TIM2->ARR = i;
//		for (j = 0; j < 200; j++)
//			for (k = 0; k < 200; k++);
//	}
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	
//	TIM_ARRPreloadConfig(TIM2, ENABLE);
}
