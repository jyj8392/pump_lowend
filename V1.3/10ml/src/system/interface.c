#include "include.h"

//开机界面
void interface_0_show(void)
{
//	char CurrentPressureStr[17];
//	memset(CurrentPressureStr, 0, sizeof(CurrentPressureStr));
//	sprintf(CurrentPressureStr, "当前压力%02d.02dMPa", 0, 0);
	ShowStr(0, 0, "当前压力00.00MPa", 0);
	ShowStr(1, 0, "设定压力00.00MPa", 0);
	ShowStr(2, 0, "设定流量00.00 ml", 0);
}

/*********************************************************************************************************
**                           TaskMain 系统主任务
********************************************************************************************************/
void TaskMain(void *pdata)
{
	struct sys_ctl *sysctl = pdata;
	u8 *keyVal;
	
	int i;
	u16 LastSetFlow = 0;
	u8 SetFlowPos[] = {0, 0, 0, 0};
	u8 SetFlowPosIndex;
	char FlowStr[] = "00.00";
//	u8 SetFlowMode = 0;
	
	u16 LastSetPressure = 0;
	u8 SetPressMaxPos[] = {0, 0, 0, 0};
	u8 SetPressMaxPosIndex;
	char PressMaxStr[] = "00.00";
//	u8 SetPressMinPos[] = {0, 0, 0, 0};
//	u8 SetPressMinPosIndex;
//	char PressMinStr[] = "00.00";
	char AccCntStr[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	u8 SetFlowParaAdjPos[] = {0, 0, 0, 0};
	u8 SetFlowParaAdjPosIndex;
	char FlowParaAdjStr[] = "00.00";

//	u8 SetPurgeFlowPos[] = {0, 0, 0, 0};
//	u8 SetPurgeFlowPosIndex;
//	char PurgeFlowStr[] = "0.000";
//	char LuminStepStr[] = "  9";
//	char KeyBeepStr[] = "   开";
	
//	u8 SetShowCntPage = 0;
//	char ShowCntStr[] = "00000";
//	char ShowPressStr[] = "0000";
	
	char NowPressStr[] = "+00.00";
	sysctl->pressOffset = 0;
	sysctl->interface = WORKING;
	sysctl->purgeFlow = MAXFLOW * 1000;
	ShowStr(0, 0, "当前压力00.00MPa", 0);
	ShowStr(1, 0, "设定压力00.00MPa", 0);
	ShowStr(2, 0, "设定流量00.00 ml", 0);
	sprintf(PressMaxStr, "%02d.%02d", sysctl->pressMax/100, sysctl->pressMax%100);
	ShowStr(1, 4, PressMaxStr, 0);
	sprintf(FlowStr, "%02d.%02d", sysctl->setFlow/1000, (sysctl->setFlow/10)%100);
	ShowStr(2, 4, FlowStr, 0);

	while(1) {
		if (PumpCtl.nowState != 1) 
			Write_Flash(sysctl);
//		if (DisTabs == 0) {		
			/*************串口接收后显示**************/
//			if (sysctl->controlMode == 1) {
//				PumpModeStr[0] = sysctl->pumpMode - 1 + 'A';
//				ShowASCII816Str(22, 23, PumpModeStr, 0);
//				sprintf(FlowStr, "%01d.%03d", sysctl->setFlow/1000, sysctl->setFlow%1000);
//				ShowASCII1624Str(4, 26, FlowStr, 0);
//			}
			
			/*************压力检测**************/
//			sysctl->nowPress = GetPressure();
//			if ((sysctl->nowPress - sysctl->pressOffset) >= 0)
//				sprintf(NowPressStr, "+%02d.%02d", (sysctl->nowPress - sysctl->pressOffset)/100, (sysctl->nowPress - sysctl->pressOffset)%100);
//			else
//				sprintf(NowPressStr, "-%02d.%02d", abs(sysctl->nowPress - sysctl->pressOffset)/100, abs(sysctl->nowPress - sysctl->pressOffset)%100);
//			ShowASCII1624Str(30, 26, NowPressStr, 0);
//		} else if (DisTabs == 1) {
			/*************压力设定显示**************/
//			if (sysctl->controlMode == 1) {
//				sprintf(PressMaxStr, "%02d.%02d", sysctl->pressMax/100, sysctl->pressMax%100);
//				ShowASCII816Str(1+8+5, 4, PressMaxStr, 0);
//				sprintf(PressMinStr, "%02d.%02d", sysctl->pressMin/100, sysctl->pressMin%100);
//				ShowASCII816Str(1+8+5, 24, PressMinStr, 0);
//			}
//		}
			

			/*************压力保护**************/
		if (sysctl->nowPress >= sysctl->pressMax 
			|| sysctl->nowPress < sysctl->pressMin) 
			sysctl->pressProtect = 1;
		else
			sysctl->pressProtect = 0;
		
	
/*		if (sysctl->flowMode == 0) {
			char AccCntStr[] = "0000000000";
			sprintf(AccCntStr, "% 9d", TIM2->ARR);
//			sprintf(AccCntStr, "%f.1", calPID);
			ShowASCII816Str(1, 50, AccCntStr, 0);
			sprintf(AccCntStr, "% 9d", (u32)(PumpCtl.calCnt - PumpCtl.accCnt));
//			sprintf(AccCntStr, "%06d", (u32)(PumpCtl.calCnt));
			ShowASCII816Str(20, 50, AccCntStr, 0);
		} else if (sysctl->flowMode == 1) {
			char AccCntStr[] = "0000000000               ";
			sprintf(AccCntStr, "%06d             ", PumpCtl.accCnt);
			ShowASCII816Str(1, 50, AccCntStr, 0);		
		} else if (sysctl->flowMode == 9) {
			char AccCntStr[] = "0000000000               ";
			sprintf(AccCntStr, "%06d             ", PumpCtl.accCnt);
			ShowASCII816Str(1, 50, AccCntStr, 0);		
		}*/


//		sprintf(AccCntStr, "% 9d", (u32)(PumpCtl.calCnt - PumpCtl.accCnt));
//		ShowStr(3, 0, AccCntStr, 1);
		OSTimeDly(10);
					
		if (sysctl->interface == WORKING) {
			if ((keyVal = OSMboxAccept(KeyMbox)) != NULL) {
				switch (*keyVal) {
				case KEYL:
					sysctl->interface = SETTINGFLOW;
					ShowStr(2, 4, "  .  ", 2);
					memset(SetFlowPos, 0, sizeof(SetFlowPos));
					break;
				case KEYP:
					sysctl->interface = SETTINGPRESSURE;
					ShowStr(1, 4, "00.00", 1);
					memset(SetPressMaxPos, 0, sizeof(SetPressMaxPos));
					break;
				case KEYNUM1:
				case KEYNUM2:
				case KEYNUM3:
				case KEYNUM4:
				case KEYNUM5:
				case KEYNUM6:
				case KEYNUM7:
				case KEYNUM8:
				case KEYNUM9:
				case KEYNUM0:
				
					break;
				case KEYSTART:
					PumpStart(sysctl->setFlow);
					ShowStr(3, 6, " RUN", 0);
//					if (FlowSetEnable == 1) {
//						sysctl->setFlow = SetFlowPos[0] * 1000 + SetFlowPos[1] * 100 + SetFlowPos[2] * 10 + SetFlowPos[3] * 1;
//						PumpSetFlow(sysctl->setFlow);
//						ShowStr(2, 4, FlowStr, 0);
//					} else if (SetTab0PosIndex == 2 && PumpCtl.nowState != 1) {
//						sysctl->pressOffset = sysctl->nowPress;
//						ShowASCII1624Str(30, 26, "+00.00", 0);
//					}	
					break;
				case KEYSTOP:
					PumpStop();
					ShowStr(3, 6, "STOP", 0);
					break;
				case KEYUP:
					sysctl->interface = FLOWPARAADJ;
					Clear_Screen();
					ShowStr(0, 2, "调整流量", 0);
					ShowStr(1, 0, "设定流量", 0);
					ShowStr(2, 0, "测定流量", 0);
					ShowStr(1, 0, "设定流量", 0);
					sprintf(FlowStr, "%02d.%02d", sysctl->setFlow/1000, (sysctl->setFlow/10)%100);
					ShowStr(1, 5, FlowStr, 0);
					sprintf(FlowParaAdjStr, "%02d.%02d", sysctl->setFlow/1000, (sysctl->setFlow/10)%100);
					SetFlowParaAdjPos[0] = FlowParaAdjStr[0] - 0x30;
					SetFlowParaAdjPos[1] = FlowParaAdjStr[1] - 0x30;
					SetFlowParaAdjPos[2] = FlowParaAdjStr[3] - 0x30;
					SetFlowParaAdjPos[3] = FlowParaAdjStr[4] - 0x30;
					ShowStr(2, 5, FlowParaAdjStr, 1);
					break;
				case KEYDOWN:
					sysctl->interface = PARARESETING;
					Clear_Screen();
					ShowStr(1, 2, "出厂值恢复", 0);
					ShowStr(3, 0, "确定R", 0);
					ShowStr(3, 5, "取消S", 0);
					break;
				}
			} else {
				sysctl->nowPress = GetPressure();
//				sysctl->nowPress = GetFLOWPLC();
				if ((sysctl->nowPress - sysctl->pressOffset) >= 0)
					sprintf(NowPressStr, "%02d.%02d", (sysctl->nowPress - sysctl->pressOffset)/100, (sysctl->nowPress - sysctl->pressOffset)%100);
				else
					sprintf(NowPressStr, "00.00");
				ShowStr(0, 4, NowPressStr, 0);
				
				if (LastSetFlow != sysctl->setFlow) {
					LastSetFlow = sysctl->setFlow;
					sprintf(FlowStr, "%02d.%02d", sysctl->setFlow/1000, (sysctl->setFlow/10)%100);
					ShowStr(2, 4, FlowStr, 0);
				}
				if (LastSetPressure != sysctl->pressMax) {
					LastSetPressure = sysctl->pressMax;
					sprintf(PressMaxStr, "%02d.%02d", sysctl->pressMax/100, sysctl->pressMax%100);
					ShowStr(1, 4, PressMaxStr, 0);
				}
			}
		} else if (sysctl->interface == PURGING) {
			if ((keyVal = OSMboxAccept(KeyMbox)) != NULL) {
				switch (*keyVal) {
				case KEYSTOP:
					sysctl->interface = WORKING;
					ShowStr(3, 0, "          ", 0);
					PumpStop();				
					break;
				}
				if (PumpCtl.nowState == 0) {
					sysctl->interface = WORKING;
					ShowStr(3, 0, "          ", 0);
				}
			}
		} else if (sysctl->interface == SETTINGFLOW) {
			if ((keyVal = OSMboxAccept(KeyMbox)) != NULL) {
				switch (*keyVal) {
				case KEYDOWN:
					sysctl->interface = PURGING;
					PumpPuge(sysctl->purgeFlow);
					sprintf(FlowStr, "%02d.%02d", sysctl->setFlow/1000, (sysctl->setFlow/10)%100);
					ShowStr(2, 4, FlowStr, 0);
					ShowStr(3, 0, "Purging...      ", 0);
					break;
				case KEYL:
					ShowStr(2, 4, "  .  ", 1);
					ShowStr(3, 0, "     ", 0);
					memset(SetFlowPos, 0, sizeof(SetFlowPos));
					break;
				case KEYNUM1:
				case KEYNUM2:
				case KEYNUM3:
				case KEYNUM4:
				case KEYNUM5:
				case KEYNUM6:
				case KEYNUM7:
				case KEYNUM8:
				case KEYNUM9:
				case KEYNUM0:
					for (SetFlowPosIndex = 1; SetFlowPosIndex < sizeof(SetFlowPos); ++SetFlowPosIndex)
						SetFlowPos[SetFlowPosIndex - 1] = SetFlowPos[SetFlowPosIndex];
					
					SetFlowPos[SetFlowPosIndex - 1] = (*keyVal == KEYNUM0? 0: *keyVal);
					FlowStr[0] = SetFlowPos[0] + 0x30;
					FlowStr[1] = SetFlowPos[1] + 0x30;
					FlowStr[2] = '.';
					FlowStr[3] = SetFlowPos[2] + 0x30;
					FlowStr[4] = SetFlowPos[3] + 0x30;
					ShowStr(2, 4, FlowStr, 2);
					break;
				case KEYSTART:
					if (MAXFLOW * 1000 >= SetFlowPos[0] * 10000 + SetFlowPos[1] * 1000 + SetFlowPos[2] * 100 + SetFlowPos[3] * 10) {
						sysctl->setFlow = SetFlowPos[0] * 10000 + SetFlowPos[1] * 1000 + SetFlowPos[2] * 100 + SetFlowPos[3] * 10;
						PumpSetFlow(sysctl->setFlow);
						ShowStr(2, 4, FlowStr, 0);
						sysctl->interface = WORKING;
						if (PumpCtl.nowState == 1) ShowStr(3, 6, " RUN", 0);
						else ShowStr(3, 6, "STOP", 0);
					} else {
						ShowStr(3, 0, "ERROR", 0);
					}
					break;
				case KEYSTOP:
					sprintf(FlowStr, "%02d.%02d", sysctl->setFlow/1000, (sysctl->setFlow/10)%100);
					ShowStr(2, 4, FlowStr, 0);
					ShowStr(3, 0, "     ", 0);
					sysctl->interface = WORKING;
					if (PumpCtl.nowState == 1) ShowStr(3, 6, " RUN", 0);
					else ShowStr(3, 6, "STOP", 0);
					break;
				}
			}
		} else if (sysctl->interface == SETTINGPRESSURE) {
			if ((keyVal = OSMboxAccept(KeyMbox)) != NULL) {
				switch (*keyVal) {
				case KEYP:
					ShowStr(1, 4, "00.00", 1);
					ShowStr(3, 0, "     ", 0);
					memset(SetPressMaxPos, 0, sizeof(SetPressMaxPos));
					break;
				case KEYNUM1:
				case KEYNUM2:
				case KEYNUM3:
				case KEYNUM4:
				case KEYNUM5:
				case KEYNUM6:
				case KEYNUM7:
				case KEYNUM8:
				case KEYNUM9:
				case KEYNUM0:
					for (SetPressMaxPosIndex = 1; SetPressMaxPosIndex < sizeof(SetPressMaxPos); ++SetPressMaxPosIndex)
						SetPressMaxPos[SetPressMaxPosIndex - 1] = SetPressMaxPos[SetPressMaxPosIndex];
					
					SetPressMaxPos[SetPressMaxPosIndex - 1] = (*keyVal == KEYNUM0? 0: *keyVal);
					PressMaxStr[0] = SetPressMaxPos[0] + 0x30;
					PressMaxStr[1] = SetPressMaxPos[1] + 0x30;
					PressMaxStr[2] = '.';
					PressMaxStr[3] = SetPressMaxPos[2] + 0x30;
					PressMaxStr[4] = SetPressMaxPos[3] + 0x30;
					ShowStr(1, 4, PressMaxStr, 1);
					break;
				case KEYSTART:
					if (MAXPRESSURE * 1000 >= SetPressMaxPos[0] * 1000 + SetPressMaxPos[1] * 100 + SetPressMaxPos[2] * 10 + SetPressMaxPos[3] * 1) {
						sysctl->pressMax = SetPressMaxPos[0] * 1000 + SetPressMaxPos[1] * 100 + SetPressMaxPos[2] * 10 + SetPressMaxPos[3] * 1;
						ShowStr(1, 4, PressMaxStr, 0);
						sysctl->interface = WORKING;
					} else {
						ShowStr(3, 0, "ERROR", 0);
					}
					break;
				case KEYSTOP:
					sprintf(PressMaxStr, "%02d.%02d", sysctl->pressMax/100, sysctl->pressMax%100);
					ShowStr(1, 4, PressMaxStr, 0);
					ShowStr(3, 0, "     ", 0);
					sysctl->interface = WORKING;
					break;
				}
			}				
		} else if (sysctl->interface == FLOWPARAADJ) {
			if ((keyVal = OSMboxAccept(KeyMbox)) != NULL) {
				switch (*keyVal) {
				case KEYNUM1:
				case KEYNUM2:
				case KEYNUM3:
				case KEYNUM4:
				case KEYNUM5:
				case KEYNUM6:
				case KEYNUM7:
				case KEYNUM8:
				case KEYNUM9:
				case KEYNUM0:
					for (SetFlowParaAdjPosIndex = 1; SetFlowParaAdjPosIndex < sizeof(SetFlowParaAdjPos); ++SetFlowParaAdjPosIndex)
						SetFlowParaAdjPos[SetFlowParaAdjPosIndex - 1] = SetFlowParaAdjPos[SetFlowParaAdjPosIndex];
					
					SetFlowParaAdjPos[SetFlowParaAdjPosIndex - 1] = (*keyVal == KEYNUM0? 0: *keyVal);
					FlowParaAdjStr[0] = SetFlowParaAdjPos[0] + 0x30;
					FlowParaAdjStr[1] = SetFlowParaAdjPos[1] + 0x30;
					FlowParaAdjStr[2] = '.';
					FlowParaAdjStr[3] = SetFlowParaAdjPos[2] + 0x30;
					FlowParaAdjStr[4] = SetFlowParaAdjPos[3] + 0x30;
					ShowStr(2, 5, FlowParaAdjStr, 1);
					break;
				case KEYSTART:
//					if (sysctl->setFlow != SetFlowParaAdjPos[0] * 10000 + SetFlowParaAdjPos[1] * 1000 + SetFlowParaAdjPos[2] * 100 + SetFlowParaAdjPos[3] * 10) {
						sprintf(FlowParaAdjStr, "%05d", SetFlowPara(sysctl->setFlow, SetFlowParaAdjPos[0] * 10000 + SetFlowParaAdjPos[1] * 1000 + SetFlowParaAdjPos[2] * 100 + SetFlowParaAdjPos[3] * 10));				
						Clear_Screen();
						ShowStr(3, 0, FlowParaAdjStr, 0);
						OSTimeDly(2000);
//					}
					Clear_Screen();
					ShowStr(0, 0, "当前压力00.00MPa", 0);
					ShowStr(1, 0, "设定压力00.00MPa", 0);
					ShowStr(2, 0, "设定流量00.00 ml", 0);
					sprintf(PressMaxStr, "%02d.%02d", sysctl->pressMax/100, sysctl->pressMax%100);
					ShowStr(1, 4, PressMaxStr, 0);
					sprintf(FlowStr, "%02d.%02d", sysctl->setFlow/1000, (sysctl->setFlow/10)%100);
					ShowStr(2, 4, FlowStr, 0);				
					sysctl->interface = WORKING;
					break;
				case KEYSTOP:
					Clear_Screen();
					ShowStr(0, 0, "当前压力00.00MPa", 0);
					ShowStr(1, 0, "设定压力00.00MPa", 0);
					ShowStr(2, 0, "设定流量00.00 ml", 0);
					sprintf(PressMaxStr, "%02d.%02d", sysctl->pressMax/100, sysctl->pressMax%100);
					ShowStr(1, 4, PressMaxStr, 0);
					sprintf(FlowStr, "%02d.%02d", sysctl->setFlow/1000, (sysctl->setFlow/10)%100);
					ShowStr(2, 4, FlowStr, 0);				
					sysctl->interface = WORKING;
					break;
				}
			}
		} else if (sysctl->interface == PARARESETING) {
			if ((keyVal = OSMboxAccept(KeyMbox)) != NULL) {
				switch (*keyVal) {
				case KEYSTART:
					for (i = 0; i < sizeof(PumpCtl.flowPara); ++i)
						*((u8*)(PumpCtl.flowPara) + i) = *((u8*)(PUMP_FlowPara) + i);
					Clear_Screen();
					ShowStr(1, 2, "出厂值恢复", 0);
					ShowStr(2, 3, "成功", 0);
					OSTimeDly(2000);
					Clear_Screen();
					ShowStr(0, 0, "当前压力00.00MPa", 0);
					ShowStr(1, 0, "设定压力00.00MPa", 0);
					ShowStr(2, 0, "设定流量00.00 ml", 0);
					sprintf(PressMaxStr, "%02d.%02d", sysctl->pressMax/100, sysctl->pressMax%100);
					ShowStr(1, 4, PressMaxStr, 0);
					sprintf(FlowStr, "%02d.%02d", sysctl->setFlow/1000, (sysctl->setFlow/10)%100);
					ShowStr(2, 4, FlowStr, 0);				
					sysctl->interface = WORKING;
					break;
				case KEYSTOP:
					Clear_Screen();
					ShowStr(0, 0, "当前压力00.00MPa", 0);
					ShowStr(1, 0, "设定压力00.00MPa", 0);
					ShowStr(2, 0, "设定流量00.00 ml", 0);
					sprintf(PressMaxStr, "%02d.%02d", sysctl->pressMax/100, sysctl->pressMax%100);
					ShowStr(1, 4, PressMaxStr, 0);
					sprintf(FlowStr, "%02d.%02d", sysctl->setFlow/1000, (sysctl->setFlow/10)%100);
					ShowStr(2, 4, FlowStr, 0);				
					sysctl->interface = WORKING;
					break;
				}
			}
		}
			
	}
}











