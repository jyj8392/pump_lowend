#include "include.h"

#define ON				1
#define OFF				0
//#define PUMP_POS		GPIOC, GPIO_Pin_13
#define PUMP_STEP		GPIOC, GPIO_Pin_4

#define ReadPUMPPos(dat)		(GPIO_ReadInputDataBit(dat))
#define SetPUMPStep(ctl, sw)	(sw == ON? GPIO_SetBits(ctl): GPIO_ResetBits(ctl))

struct pumpctl_t PumpCtl;

/*
通过实测，计算1个脉冲对应的液体体积
5000脉冲一圈
检测到光电开关，累加值加10000
未检测到的过程中，累加值加实际发送脉冲数*2
停止开始：计算实际运行时间，根据流速设定计算总脉冲数，对比实际发送脉冲数，不足则在减速过程中补，多了就停。
停止过程最后累加值加每次检测光电开关产生误差的平均值的比例（1/10000）
运行过程中：检测到光电开关，计算速度，若大于设定流速，则降低定时器中断速度
*/

/*********************************************************************************************************
**                           泵流量校准系数（脉冲数）
********************************************************************************************************/
/*******10ml********/
#if MAXFLOW == 10
struct pumpflow_t const PUMP_FlowPara[] = {
10, 59,
20, 117,
50, 283,
100, 585,
200, 1170,
500, 2925,
1000, 5850,
2000, 11701,
5000, 29252,
10000, 58504,
};
#endif
/***************/

/*******50ml********/
#if MAXFLOW == 50
struct pumpflow_t const PUMP_FlowPara[] = {
50, 59,
100, 117,
250, 283,
500, 585,
1000, 1170,
2500, 2925,
5000, 5850,
10000, 11701,
25000, 29252,
50000, 58504,
};
#endif
/***************/

/********100ml*******/
#if MAXFLOW == 100
struct pumpflow_t const PUMP_FlowPara[] = {
10, 59,
20, 117,
50, 283,
100, 585,
200, 1170,
500, 2925,
1000, 5850,
2000, 11701,
5000, 29252,
10000, 58504,
};
#endif
/***************/

/********200ml*******/
#if MAXFLOW == 200
struct pumpflow_t const PUMP_FlowPara[] = {
20, 59,
40, 117,
100, 283,
200, 585,
400, 1170,
1000, 2925,
2000, 5850,
40000, 11701,
10000, 29252,
20000, 58504,
};
#endif
/***************/

//void GetStep(void)
//{
//	unsigned char i = 0;
//	if (ReadPUMPPos(PUMP_POS)) {
//		i = 1;
//	} else {
//		i = 0;
//	}
//}

void delayms(u16 k)
{  
	u16 i;
	for(i=0;i<2000;i++); 	
}

void SetStep(void)
{
	static unsigned int i = 0;
	if (i <= 5000) {
	SetPUMPStep(PUMP_STEP, ON);
	delayms(2000);
	SetPUMPStep(PUMP_STEP, OFF);
	delayms(2000);
	} else {
		i = 10000;
	}
	i++;
}

void PumpSetFlow(u16 setFlow)
{
	PumpCtl.startFlow = PumpCtl.nowFlow;
	PumpCtl.dstFlow = setFlow;
	PumpCtl.nowTime = 0;
	PumpCtl.isPIDStart = 0;
	PumpCtl.dCndMax = PumpCtl.calCnt - PumpCtl.accCnt;
	PID_Init(0.8, 0, 0, PumpCtl.dstFlow / 10, PumpCtl.dstFlow * 5);
}

void PumpStart(u16 setFlow)
{
	if (PumpCtl.nowState == 1) return;
	
	if (GetFlowPara(setFlow) < 20) {
		TIM2->PSC = 360;
		PumpCtl.dstInterval = 100000 / GetFlowPara(setFlow);
	} else {
		TIM2->PSC = 36;
		PumpCtl.dstInterval = 1000000 / GetFlowPara(setFlow);
	}
	PumpCtl.maxTime = (0xffffffff / (2000000 / (PumpCtl.dstInterval-1)) * 100);
	
	
	PumpCtl.nowInterval = 2000;
	TIM2->ARR = PumpCtl.nowInterval;
#if SOFTSTART == 0
	PumpCtl.startFlow = setFlow;
#else
	PumpCtl.startFlow = 0;
#endif
	PumpCtl.dstFlow = setFlow;
	PumpCtl.nowFlow = PumpCtl.startFlow;
	PumpCtl.nowTime = 0;
	PumpCtl.isPIDStart = 0;
	PumpCtl.dCndMax = 0;
	PID_Init(0.8, 0, 0, PumpCtl.dstFlow / 10, PumpCtl.dstFlow / 10 * 5);
	
	PumpCtl.nowState = 1;
	PumpCtl.accCnt = 0;
	PumpCtl.accTime = 0;
	PumpCtl.calCnt = 0;
	PumpCtl.dCndMax = 0;
	
	TIM_Cmd(TIM4, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

void PumpPuge(u16 setFlow)
{
	if (PumpCtl.nowState == 1) return;
	
	PumpCtl.maxTime = PURGETIME * 100;	
	
	PumpCtl.nowInterval = 2000;
	TIM2->ARR = PumpCtl.nowInterval;
	PumpCtl.startFlow = 0;
	PumpCtl.dstFlow = setFlow;
	PumpCtl.nowFlow = PumpCtl.startFlow;
	PumpCtl.nowTime = 0;
	PumpCtl.isPIDStart = 0;
	PumpCtl.dCndMax = 0;
	PID_Init(0.8, 0, 0, PumpCtl.dstFlow / 10, PumpCtl.dstFlow * 5);
	
	PumpCtl.nowState = 1;
	PumpCtl.accCnt = 0;
	PumpCtl.accTime = 0;
	PumpCtl.calCnt = 0;
	PumpCtl.dCndMax = 0;
	
	TIM_Cmd(TIM4, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

void PumpStop(void)
{
	if (PumpCtl.nowState != 1) return;
	PumpCtl.nowState = 2;
	
	PumpCtl.startFlow = PumpCtl.dstFlow;
	PumpCtl.dstFlow = 0;
	PumpCtl.nowTime = 0;
	PumpCtl.isPIDStart = 0;
	PumpCtl.dCndMax = PumpCtl.calCnt - PumpCtl.accCnt;
	PID_Init(0.8, 0, 0, PumpCtl.dstFlow / 10, PumpCtl.dstFlow * 5);
	
	TIM_Cmd(TIM4, DISABLE);
	if (PumpCtl.sysctl->pressProtect == 1) {
		TIM_Cmd(TIM2, DISABLE);
		TIM_Cmd(TIM3, DISABLE);
		PumpCtl.nowState = 0;
		PumpCtl.sysctl->pressProtect = 0;
	}
}

u16 GetFlowPara(double flow)
{
	u8 i = 0;
	u16 cnt;
	
	if (flow == 0) flow = 100;
		
	for (i = 0; i < 10 && flow > PumpCtl.flowPara[i].index; ++i);
	if (i == 10)
		cnt = (flow - PumpCtl.flowPara[9].index) * (PumpCtl.flowPara[9].cnt - PumpCtl.flowPara[8].cnt) / (PumpCtl.flowPara[9].index - PumpCtl.flowPara[8].index) + PumpCtl.flowPara[9].cnt;
	else if (i == 0) 
		cnt = PumpCtl.flowPara[i].cnt * flow / PumpCtl.flowPara[i].index;
	else {
		cnt = (PumpCtl.flowPara[i].cnt - PumpCtl.flowPara[i - 1].cnt) * ((float)(flow - PumpCtl.flowPara[i - 1].index) / (float)(PumpCtl.flowPara[i].index - PumpCtl.flowPara[i - 1].index));
		cnt += PumpCtl.flowPara[i - 1].cnt;
	}
	if (cnt < 1) cnt = 1;
	return cnt;
//	return 58504/10000*flow;
}

u16 SetFlowPara(u16 flow, u16 curflow)
{
	u8 i = 0;
	double coefficient = (double)curflow / (double)flow ;

	for (i = 0; i < 10 && flow > PumpCtl.flowPara[i].index; ++i);
	
	if (i == 0 || flow == PumpCtl.flowPara[i].index)
		PumpCtl.flowPara[i].cnt *= coefficient;
	else {
		PumpCtl.flowPara[i - 1].cnt *= coefficient;
		PumpCtl.flowPara[i].cnt *= coefficient;
	}
	return GetFlowPara(flow);
}
