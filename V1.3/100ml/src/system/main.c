#include "include.h"

OS_STK	TaskStkInit[TaskStkInitLengh];		//Define the TaskStkInit stack
OS_STK	TaskStkKey[TaskStkKeyLengh];		//Define the TaskStkKey stack
OS_STK	TaskStkBZ[TaskStkBZLengh];			//Define the TaskStkBZ stack
OS_STK	TaskStkPUMP[TaskStkPUMPLengh];		//Define the TaskStkLCD stack
OS_STK	TaskStkPC[TaskStkPCLengh];			//Define the TaskStkPC stack
OS_STK	TaskStkMain[TaskStkMainLengh];		//Define the TaskStkMain stack

OS_EVENT *KeyMbox;		//存储键值的消息邮箱
OS_EVENT *LcdMbox;		//存储键值的消息邮箱
//OS_EVENT *StickMutex;
//OS_EVENT *FreezeMutex;
//OS_EVENT *ThawMutex;

void OSCreateEvent(void)
{
//	u8 err;
	KeyMbox = OSMboxCreate((void *)0);				//建立存储按键值的消息邮箱
	LcdMbox = OSMboxCreate((void *)0);				//建立存储按键值的消息邮箱
//	StickMutex = OSMutexCreate(PRI_Stick,&err); 	//定位互斥量
//	FreezeMutex = OSMutexCreate(PRI_Freeze,&err); 	//冷冻互斥量
//	ThawMutex = OSMutexCreate(PRI_Thaw,&err); 		//复温互斥量
}

int main(void)
{
	
	OSInit();
	OSCreateEvent();																										
	OSTaskCreate(TaskInit, (void *)0, &TaskStkInit[TaskStkInitLengh - 1], PRI_Init);	
	OSStart();

	return 0;
}

/*********************************************************************************************************
**                           TaskInit 初始化任务
********************************************************************************************************/
void TaskInit(void *pdata)
{
 	struct sys_ctl sysctl;
	pdata = pdata;
	
	
	RCC_Configuration();
 	TIM_Configuration();
 	NVIC_Configuration();
	GPIO_Configuration();
 	ADC_Configuration();
 	DMA_Configuration();
	USART_Configuration();
//// 	Tim1_Configuration();
//	SPI_Configuration();

//	TIM_Cmd(TIM3, ENABLE);

//	memset(keyMux, 0, sizeof(keyMux));
//	memset(&spiCtl, 0, sizeof(spiCtl));
//	memset(&sysctl, 0, sizeof(struct sys_ctl));
	
	Read_Flash(&sysctl);

	LCD_Initial();
	OSStatInit();
	OSTaskCreate(TaskMain, &sysctl, &TaskStkMain[TaskStkMainLengh - 1], PRI_Main);	
	OSTaskCreate(TaskPUMP, &sysctl, &TaskStkPUMP[TaskStkPUMPLengh - 1], PRI_PUMP);	
	OSTaskCreate(TaskKey, &sysctl, &TaskStkKey[TaskStkKeyLengh - 1], PRI_Key);	
	OSTaskCreate(TaskPC, &sysctl, &TaskStkPC[TaskStkPCLengh - 1], PRI_PC);	
	
//	OSTaskDel(OS_PRIO_SELF);
PumpCtl.sysctl = &sysctl;

//sysctl.setFlow = 0;
sysctl.controlMode = 0;
sysctl.pumpMode = 1;
//sysctl.flowMode = 0;
//sysctl.pressMax = 4200;
//sysctl.pressMin = 0;
//sysctl.purgeFlow = 2000;
//sysctl.backLumin = 9;
//sysctl.keyBeep = 1;
//sysctl.nowPress = 0;
//sysctl.pressOffset = 0;
//sysctl.pressProtect = 0;
//SetPulse();
//ScanKey();
//while(1) {
//	GetPressure();
//	GetStep();
////	SetStep();
//}
	while(1) {
//			UART4_Puts("!12000123456999\x0A");
//			UART4_Puts("!12000123456999");

		OSTimeDly(500);
	}
}

/*********************************************************************************************************
**                           TaskKey 键盘任务
********************************************************************************************************/
void TaskKey(void *pdata)
{
	struct sys_ctl *sysctl = pdata;
	u8 keyBuf = 0, keyBufTmp = 0;
	u8 keyMux[22];

	while(1) {
		OSTimeDly(10);

		ScanCol1();
		OSTimeDly(20);
		if (ISKEYNUM0) {
			OSTimeDly(10);
			if (ISKEYNUM0 && keyMux[KEYNUM0] == 0) {
				keyMux[KEYNUM0] = 1;
				keyBuf = KEYNUM0;
			}
		} else {
			keyMux[KEYNUM0] = 0;
		}		
		if (ISKEYNUM4) {
			OSTimeDly(10);
			if (ISKEYNUM4 && keyMux[KEYNUM4] == 0) {
				keyMux[KEYNUM4] = 1;
				keyBuf = KEYNUM4;
			}
		} else {
			keyMux[KEYNUM4] = 0;
		}
		if (ISKEYNUM8) {
			OSTimeDly(10);
			if (ISKEYNUM8 && keyMux[KEYNUM8] == 0) {
				keyMux[KEYNUM8] = 1;
				keyBuf = KEYNUM8;
			}
		} else {
			keyMux[KEYNUM8] = 0;
		}
		if (ISKEYL) {
			OSTimeDly(10);
			if (ISKEYL && keyMux[KEYL] == 0) {
				keyMux[KEYL] = 1;
				keyBuf = KEYL;
			}
		} else {
			keyMux[KEYL] = 0;
		}

		ScanCol2();
		OSTimeDly(20);
		if (ISKEYNUM1) {
			OSTimeDly(10);
			if (ISKEYNUM1 && keyMux[KEYNUM1] == 0) {
				keyMux[KEYNUM1] = 1;
				keyBuf = KEYNUM1;
			}
		} else {
			keyMux[KEYNUM1] = 0;
		}
		if (ISKEYNUM5) {
			OSTimeDly(10);
			if (ISKEYNUM5 && keyMux[KEYNUM5] == 0) {
				keyMux[KEYNUM5] = 1;
				keyBuf = KEYNUM5;
			}
		} else {
			keyMux[KEYNUM5] = 0;
		}
		if (ISKEYNUM9) {
			OSTimeDly(10);
			if (ISKEYNUM9 && keyMux[KEYNUM9] == 0) {
				keyMux[KEYNUM9] = 1;
				keyBuf = KEYNUM9;
			}
		} else {
			keyMux[KEYNUM9] = 0;
		}
		if (ISKEYP) {
			OSTimeDly(10);
			if (ISKEYP && keyMux[KEYP] == 0) {
				keyMux[KEYP] = 1;
				keyBuf = KEYP;
			}
		} else {
			keyMux[KEYP] = 0;
		}

		ScanCol3();
		OSTimeDly(20);
		if (ISKEYNUM2) {
			OSTimeDly(10);
			if (ISKEYNUM2 && keyMux[KEYNUM2] == 0) {
				keyMux[KEYNUM2] = 1;
				keyBuf = KEYNUM2;
			}
		} else {
			keyMux[KEYNUM2] = 0;
		}
		if (ISKEYNUM6) {
			OSTimeDly(10);
			if (ISKEYNUM6 && keyMux[KEYNUM6] == 0) {
				keyMux[KEYNUM6] = 1;
				keyBuf = KEYNUM6;
			}
		} else {
			keyMux[KEYNUM6] = 0;
		}		
		if (ISKEYUP) {
			OSTimeDly(10);
			if (ISKEYUP && keyMux[KEYUP] == 0) {
				keyMux[KEYUP] = 1;
				keyBuf = KEYUP;
			}
		} else {
			keyMux[KEYUP] = 0;
		}
		if (ISKEYSTOP) {
			OSTimeDly(10);
			if (ISKEYSTOP && keyMux[KEYSTOP] == 0) {
				keyMux[KEYSTOP] = 1;
				keyBuf = KEYSTOP;
			}
		} else {
			keyMux[KEYSTOP] = 0;
		}
		
		ScanCol4();
		OSTimeDly(20);
		if (ISKEYNUM3) {
			OSTimeDly(10);
			if (ISKEYNUM3 && keyMux[KEYNUM3] == 0) {
				keyMux[KEYNUM3] = 1;
				keyBuf = KEYNUM3;
			}
		} else {
			keyMux[KEYNUM3] = 0;
		}
		if (ISKEYNUM7) {
			OSTimeDly(10);
			if (ISKEYNUM7 && keyMux[KEYNUM7] == 0) {
				keyMux[KEYNUM7] = 1;
				keyBuf = KEYNUM7;
			}
		} else {
			keyMux[KEYNUM7] = 0;
		}		
		if (ISKEYDOWN) {
			OSTimeDly(10);
			if (ISKEYDOWN && keyMux[KEYDOWN] == 0) {
				keyMux[KEYDOWN] = 1;
				keyBuf = KEYDOWN;
			}
		} else {
			keyMux[KEYDOWN] = 0;
		}
		if (ISKEYSTART) {
			OSTimeDly(10);
			if (ISKEYSTART && keyMux[KEYSTART] == 0) {
				keyMux[KEYSTART] = 1;
				keyBuf = KEYSTART;
			}
		} else {
			keyMux[KEYSTART] = 0;
		}
		
		if (keyBuf != 0) {
			keyBufTmp = keyBuf;
			keyBuf = 0;
			if (sysctl->keyBeep == 1) {
				sysctl->bzOnTime = 200;
				sysctl->bzOffTime = 200;
				sysctl->bzRepts = 1;
				OSTaskCreate(TaskBZ, sysctl, &TaskStkBZ[TaskStkBZLengh - 1], PRI_BZ);
			}
			OSMboxPostOpt(KeyMbox, (void *)&keyBufTmp, OS_POST_OPT_BROADCAST);
			sysctl->controlMode = 0;
		}
		
	}
}

/*********************************************************************************************************
**                           TaskBZ 蜂鸣器报警任务
********************************************************************************************************/
void TaskBZ(void *pdata)
{
	struct sys_ctl *sysctl = pdata;
	while (sysctl->bzRepts-- > 0) {
		SetBzON();
		OSTimeDly(sysctl->bzOnTime);
		SetBzOFF();
		OSTimeDly(sysctl->bzOffTime);
	}
	OSTaskDel(OS_PRIO_SELF);
}

/*********************************************************************************************************
**                           TaskPUMP 泵任务
********************************************************************************************************/
void TaskPUMP(void *pdata)
{
	struct sys_ctl *sysctl = pdata;
	u8 *keyVal;
	u8 CRCTmp[13];
	
	while(1) {		
		OSTimeDly(1);

	}
}



