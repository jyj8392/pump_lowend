/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* stm32f10x_it.c
* 所有中断响应函数，一般如果中断比较简易，则直接在此编写，否则
* 可调用HAL模块中各自对应的处理函数处理中断。
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "include.h"

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : NMIException
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{

}

/*******************************************************************************
* Function Name  : HardFaultException
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
//	while (1)
//  {
//  }
}

/*******************************************************************************
* Function Name  : MemManageException
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFaultException
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFaultException
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : DebugMonitor
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMon_Handler(void)
{
}

/*******************************************************************************
* Function Name  : SVCHandler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVC_Handler(void)
{
}

/*******************************************************************************
* Function Name  : PendSVC
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void PendSV_Handler(void)
//{
//}

/*******************************************************************************
* Function Name  : SysTickHandler
* Description    :系统时钟，一般调教到1MS中断一次
*******************************************************************************/
u32 syscnt = 0;
void SysTick_Handler(void)
{
	OS_CPU_SR  cpu_sr;


	OS_ENTER_CRITICAL();						// Tell uC/OS-II that we are starting an ISR
	if (syscnt++ > 1000000)
		syscnt = 0;
//	if (spiCtl.ovTimeCnt++ > 10000)
//		spiCtl.ovTimeCnt = 0;

	if (syscnt % 200 == 0) {
//	 	GPIOC->ODR^=GPIO_Pin_0;
	}
	OSIntNesting++;
	OS_EXIT_CRITICAL();

	OSTimeTick();								// Call uC/OS-II's OSTimeTick()

	OSIntExit();								// Tell uC/OS-II that we are leaving the ISR

}

/*******************************************************************************
* Function Name  : WWDG_IRQHandler
* Description    : This function handles WWDG interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WWDG_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : PVD_IRQHandler
* Description    : This function handles PVD interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PVD_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TAMPER_IRQHandler
* Description    : This function handles Tamper interrupt request. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TAMPER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RTC_IRQHandler    RTC中断
* Description    : RTC中断中，处理好Real_Time的值
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : FLASH_IRQHandler
* Description    : This function handles Flash interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FLASH_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RCC_IRQHandler
* Description    : This function handles RCC interrupt request. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI0_IRQHandler
* Description    : This function handles External interrupt Line 0 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI0_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI1_IRQHandler
* Description    : This function handles External interrupt Line 1 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI2_IRQHandler
* Description    : This function handles External interrupt Line 2 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI3_IRQHandler
* Description    : This function handles External interrupt Line 3 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI4_IRQHandler
* Description    : This function handles External interrupt Line 4 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMAChannel1_IRQHandler
* Description    : This function handles DMA Stream 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMAChannel2_IRQHandler
* Description    : This function handles DMA Stream 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMAChannel3_IRQHandler
* Description    : This function handles DMA Stream 3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMAChannel4_IRQHandler
* Description    : This function handles DMA Stream 4 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMAChannel5_IRQHandler
* Description    : This function handles DMA Stream 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMAChannel6_IRQHandler
* Description    : This function handles DMA Stream 6 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel6_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMAChannel7_IRQHandler
* Description    : This function handles DMA Stream 7 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel7_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADC_IRQHandler
* Description    : This function handles ADC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USB_HP_CAN_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_CAN_TX_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USB_LP_CAN_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN_RX0_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : CAN_RX1_IRQHandler
* Description    : This function handles CAN RX1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_RX1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : CAN_SCE_IRQHandler
* Description    : This function handles CAN SCE interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_SCE_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI9_5_IRQHandler(void)
{		
//    EXTI_ClearITPendingBit(EXTI_Line5);
}

/*******************************************************************************
* Function Name  : TIM1_BRK_IRQHandler
* Description    : This function handles TIM1 Break interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_BRK_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_UP_IRQHandler
* Description    : This function handles TIM1 overflow and update interrupt 
*                  request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_UP_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_TRG_COM_IRQHandler
* Description    : This function handles TIM1 Trigger and commutation interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_TRG_COM_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_CC_IRQHandler
* Description    : This function handles TIM1 capture compare interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_CC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM2_IRQHandler TIM2中断
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{
	static u8 reverse = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {

		if (PumpCtl.sysctl->pressProtect == 0) {
			if (reverse == 0) {
//				SetMSTEPON();
				GPIO_ResetBits(GPIOC,GPIO_Pin_4);
				reverse = 1;
			} else {
//				SetMSTEPOFF();
				GPIO_SetBits(GPIOC,GPIO_Pin_4);
				reverse = 0;
				PumpCtl.accCnt++;
			}
			if (PumpCtl.nowState == 2) {
				if (PumpCtl.sysctl->flowMode == 0) {
					if (PumpCtl.accCnt >= PumpCtl.calCnt) {
						TIM_Cmd(TIM2, DISABLE);
						TIM_Cmd(TIM3, DISABLE);
						PumpCtl.nowState = 0;
					}
				} else if (PumpCtl.sysctl->flowMode == 1) {
					TIM_Cmd(TIM2, DISABLE);
					TIM_Cmd(TIM3, DISABLE);
					PumpCtl.nowState = 0;
				}
			}
		}

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}   
}

/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{
	double calPID = 0;
	double pidAdj = 0;
	int rate0;
	double rate1 = 0;
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		if (PumpCtl.isPIDStart == 0 && 
			(((PumpCtl.startFlow < PumpCtl.dstFlow) && (PumpCtl.nowFlow >= PumpCtl.dstFlow)) ||
			 ((PumpCtl.startFlow > PumpCtl.dstFlow) && (PumpCtl.nowFlow <= PumpCtl.dstFlow)) ||
			 (PumpCtl.startFlow == PumpCtl.dstFlow)
			))
			PumpCtl.isPIDStart = 1;
		
		
		if (PumpCtl.isPIDStart == 0) {
			if (PumpCtl.startFlow < PumpCtl.dstFlow) { 
				rate0 = (int)(40 + PumpCtl.sysctl->nowPress / 10 + 10 * (PumpCtl.dstFlow - PumpCtl.startFlow) / PumpCtl.dstFlow);
				PumpCtl.nowFlow = (float)(PumpCtl.nowTime) / rate0 * (PumpCtl.dstFlow - PumpCtl.startFlow) + PumpCtl.startFlow;
			} else if (PumpCtl.startFlow > PumpCtl.dstFlow) {
				rate0 = (int)(40 + PumpCtl.sysctl->nowPress / 10 + 10 * (PumpCtl.startFlow - PumpCtl.dstFlow) / PumpCtl.startFlow);
				PumpCtl.nowFlow = (float)(PumpCtl.nowTime) / rate0 * (PumpCtl.dstFlow - PumpCtl.startFlow) + PumpCtl.startFlow;
			}
//			if (PumpCtl.calCnt > PumpCtl.accCnt)
				PumpCtl.nowTime++;			
		} 
//		else {
//			calPID = PID_Calc(&Cnt_PID, PumpCtl.calCnt - PumpCtl.accCnt);
//			if (calPID < 0) PumpCtl.nowFlow = PumpCtl.nowFlow + (PumpCtl.nowFlow / 1000.0);
//			else if (calPID > 0) PumpCtl.nowFlow = PumpCtl.nowFlow - (PumpCtl.nowFlow / 1000.0);				
//		}
		
		
		if (PumpCtl.nowFlow <= 0) 
			PumpCtl.nowFlow = 1;
		if (PumpCtl.nowFlow < PumpCtl.startFlow * 0.3 && PumpCtl.nowState == 2) 
			PumpCtl.nowFlow = PumpCtl.startFlow * 0.3;
		
		pidAdj = (double)(GetFlowPara(PumpCtl.nowFlow));
		if (GetFlowPara(PumpCtl.nowFlow) < 20) {
			TIM2->PSC = 360;
			PumpCtl.nowInterval = 100000 / pidAdj;
		} else {
			TIM2->PSC = 36;
			PumpCtl.nowInterval = 1000000 / pidAdj;
		}
		
		if (PumpCtl.isPIDStart == 1) {
			calPID = PID_Calc(&Cnt_PID, PumpCtl.calCnt - PumpCtl.accCnt);
			if (calPID < 0) {
				rate1 = -calPID / (PumpCtl.calCnt - PumpCtl.accCnt) * 2;
				PumpCtl.nowInterval = (u16)((float)(TIM2->ARR) * (1 - rate1) + (PumpCtl.nowInterval * 0.90 * rate1));
			}
			if (PumpCtl.calCnt < PumpCtl.accCnt)
				PumpCtl.nowInterval = TIM2->ARR * 1.05;
		}
		
		if (PumpCtl.nowInterval < 15) PumpCtl.nowInterval = 15;
		TIM2->ARR = (u16)(PumpCtl.nowInterval);
	}

	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : This function handles TIM4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
		if (PumpCtl.sysctl->pressProtect == 0) {
			PumpCtl.accTime++;
			PumpCtl.calCnt += (((float)GetFlowPara(PumpCtl.dstFlow)) / 100.0);
			
			if (PumpCtl.accTime >= PumpCtl.maxTime) {	
	//			//最大运行时长(小时):(0xffffffff / (2000000 / (PumpCtl.nowInterval-1)) / 3600
				TIM_Cmd(TIM2, DISABLE);
				TIM_Cmd(TIM3, DISABLE);
				TIM_Cmd(TIM4, DISABLE);
				PumpCtl.nowState = 0;
			}
			
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}   
}

/*******************************************************************************
* Function Name  : I2C1_EV_IRQHandler
* Description    : This function handles I2C1 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_EV_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C1_ER_IRQHandler
* Description    : This function handles I2C1 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_ER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C2_EV_IRQHandler
* Description    : This function handles I2C2 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_EV_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C2_ER_IRQHandler
* Description    : This function handles I2C2 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_ER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI1_IRQHandler
* Description    : This function handles SPI1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI2_IRQHandler
* Description    : This function handles SPI2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void USART1_IRQHandler(void)
{
  //接收中断
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
	
	//溢出-如果发生溢出需要先清空SR的溢出位,再读DR寄存器 则可清除不断入中断的问题
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);	//清溢出位
		USART_ReceiveData(USART1);				//读DR
	}

}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void USART2_IRQHandler(void)
{
	u8 tmpData = 0;

   //接收中断
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		tmpData = USART_ReceiveData(USART2);
		if (tmpData == '!') {			
			frameData.revLen = 0;
			frameData.STX = tmpData;
			syscnt = 0;
		}
		frameData.revData[frameData.revLen++] = tmpData;
		if (tmpData == 0x0A && frameData.revLen == 16) {
			frameData.ETX = tmpData;
		} else if (syscnt > 500) {
			frameData.STX = 0;
			frameData.ETX = 0;
			frameData.revLen = 0;
		}
	}
//--------------------------------------------------------------------------------------
	  
//---------------------------------------------------------------------------------------	
	//溢出-如果发生溢出需要先清空SR的溢出位,再读DR寄存器 则可清除不断入中断的问题
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE);	//清溢出位
		USART_ReceiveData(USART2);				//读DR
	}  

}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{
	//接收中断
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
	
	//溢出-如果发生溢出需要先清空SR的溢出位,再读DR寄存器 则可清除不断入中断的问题
	if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART3,USART_FLAG_ORE);	//清溢出位
		USART_ReceiveData(USART3);				//读DR
	}
}

/*******************************************************************************
* Function Name  : UART4_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART4_IRQHandler(void)
{
//	u8 tmpData = 0;
	//接收中断
	if(USART_GetITStatus(UART4,USART_IT_RXNE)==SET)
	{		
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);
//		tmpData = USART_ReceiveData(UART4);
//		if (tmpData == '!') {			
//			frameData.revLen = 0;
//			frameData.STX = tmpData;
//			syscnt = 0;
//		}
//		frameData.revData[frameData.revLen++] = tmpData;
//		if (tmpData == 0x0A && frameData.revLen == 16) {
//			frameData.ETX = tmpData;
//		} else if (syscnt > 500) {
//			frameData.STX = 0;
//			frameData.ETX = 0;
//			frameData.revLen = 0;
//		}
	}
	
	//溢出-如果发生溢出需要先清空SR的溢出位,再读DR寄存器 则可清除不断入中断的问题
	if(USART_GetFlagStatus(UART4,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(UART4,USART_FLAG_ORE);	//清溢出位
		USART_ReceiveData(UART4);				//读DR
	}
}

/*******************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : This function handles External lines 15 to 10 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RTCAlarm_IRQHandler
* Description    : This function handles RTC Alarm interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTCAlarm_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USBWakeUp_IRQHandler
* Description    : This function handles USB WakeUp interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBWakeUp_IRQHandler(void)
{
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
