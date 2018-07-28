

 #include "stm32f10x.h"

void Tim1_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;

	/* PA8����Ϊ���ܽ�(PWM) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

	
	TIM_DeInit(TIM1);

	/*TIM1ʱ������*/
	TIM_TimeBaseStructure.TIM_Prescaler = 400-1;					//Ԥ��Ƶ(ʱ�ӷ�Ƶ)72M/4000=18K
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���ϼ���
	TIM_TimeBaseStructure.TIM_Period = 200;							//װ��ֵ 18k/144=125hz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);

	/* Channel 1 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				//PWMģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//����ͨ����Ч
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//����ͨ����Ч
	TIM_OCInitStructure.TIM_Pulse = 0; 							//ռ��ʱ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//�������
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;     //�����˵ļ���  
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;  

	TIM_OC1Init(TIM1,&TIM_OCInitStructure); 						//ͨ��1
	

	/* TIM1 counter enable */
	TIM_Cmd(TIM1,ENABLE);
	
	/* TIM1 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}

//���ò���Ĵ���1
void SetT1Pwm1(u16 pulse)
{
	TIM1->CCR1=pulse;
}
