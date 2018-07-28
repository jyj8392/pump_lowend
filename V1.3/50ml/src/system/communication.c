#include "include.h"

struct uart2data_t frameData;


u16 CalCRC(char str[], u8 len)
{
	u32 sum = 0;
	while (len) {
		sum += str[--len];
	}
	return sum % 256;
}

/*********************************************************************************************************
**                           TaskPC PC´®¿ÚÈÎÎñ
********************************************************************************************************/
void TaskPC(void *pdata)
{
	struct sys_ctl *sysctl = pdata;
	u8 keyBufTmp;

	char frameIDStr[3] = {0, 0, 0};
	char frameAIStr[2] = {0, 0};
	char framePFCStr[3] = {0, 0, 0};
	char frameValueStr[7] = {0, 0, 0, 0, 0, 0, 0};
	char frameCRCStr[4] = {0, 0, 0, 0};
	
	char SendDataStr[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	char SendCRCStr[4] = {0, 0, 0, 0};
	char SendFrameStr[17] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0A, 0};

		
	u8 sendPressCnt = 0;

	u8 lastPLCCMDState = ISPLCRUN;
	u8 nowPLCCMDState = ISPLCRUN;
		
	while(1) {
		OSTimeDly(10);
		
		if (frameData.STX == '!' && frameData.ETX == 0x0A && frameData.revLen == 16) {
			sysctl->controlMode = 1;
			frameData.revLen = 0;
//			frameData.CRC = 0;
			memcpy(frameIDStr, frameData.revData + 1, 2);
			frameData.ID = atoi(frameIDStr);
			memcpy(frameAIStr, frameData.revData + 3, 1);
			frameData.AI = atoi(frameAIStr);
			memcpy(framePFCStr, frameData.revData + 4, 2);
			frameData.PFC = atoi(framePFCStr);
			memcpy(frameValueStr, frameData.revData + 6, 6);
			frameData.VALUE = atoi(frameValueStr);
			memcpy(frameCRCStr, frameData.revData + 12, 3);
			frameData.CRCCode = atoi(frameCRCStr);
			if (frameData.CRCCode != CalCRC(frameData.revData, 12)) 
				USART2_Putc(NACK);
			if (frameData.PFC != 1 && frameData.ID != EQUID) 
				USART2_Putc(NACK);
			switch (frameData.PFC) {
			case 1:
				sprintf(SendDataStr, "!%2d001    %2d", EQUID, EQUID);
				sprintf(SendCRCStr, "%03d", CalCRC(SendDataStr, 12));
				memcpy(SendFrameStr, SendDataStr, 12);
				memcpy(SendFrameStr+12, SendCRCStr, 3);
				USART2_Puts(SendFrameStr);
				break;
			case 2:
				
				USART2_Putc(ACK);
				break;
			case 3:
				sprintf(SendDataStr, "!%2d003%s", EQUID, EQUSERNO);
				sprintf(SendCRCStr, "%03d", CalCRC(SendDataStr, 12));
				memcpy(SendFrameStr, SendDataStr, 12);
				memcpy(SendFrameStr+12, SendCRCStr, 3);
				USART2_Puts(SendFrameStr);
				break;
			case 4:
				
				USART2_Putc(ACK);
				break;
			case 6:
				
				USART2_Putc(ACK);
				break;
			case 9:
				
				USART2_Putc(ACK);
				break;
			case 10:
				if (frameData.VALUE <= MAXFLOW * 1000) {
					sysctl->setFlow = frameData.VALUE;
					PumpSetFlow(sysctl->setFlow);
					USART2_Putc(ACK);
				} else {
					USART2_Putc(NACK);
				}
				break;
			case 11:
				if (frameData.VALUE <= 1000 && (frameData.AI >= 1 && frameData.AI <= 4)) {
					
					USART2_Putc(ACK);
				} else {
					USART2_Putc(NACK);
				}
				break;
			case 13:
				if (frameData.VALUE <= MAXPRESSURE * 1000) {
					sysctl->pressMax = frameData.VALUE;
					USART2_Putc(ACK);
				} else {
					USART2_Putc(NACK);
				}
				break;
			case 14:
				if (frameData.VALUE <= sysctl->pressMax) {
					sysctl->pressMin = frameData.VALUE;
					USART2_Putc(ACK);
				} else {
					USART2_Putc(NACK);
				}
				USART2_Putc(ACK);
				break;
			case 15:
				keyBufTmp = KEYSTART;
				if (sysctl->interface == WORKING) {
					OSMboxPostOpt(KeyMbox, (void *)&keyBufTmp, OS_POST_OPT_BROADCAST);
					USART2_Putc(ACK);
				} else {
					USART2_Putc(NACK);
				}
				break;
			case 16:
//				PumpStop();
				keyBufTmp = KEYSTOP;
				if (sysctl->interface == WORKING) {
					OSMboxPostOpt(KeyMbox, (void *)&keyBufTmp, OS_POST_OPT_BROADCAST);
					USART2_Putc(ACK);
				} else {
					USART2_Putc(NACK);
				}
				break;
			case 17:

				USART2_Putc(ACK);
				break;
			case 18:
				if (frameData.VALUE <= 100) {
					sysctl->sendPressInterval = frameData.VALUE;
					USART2_Putc(ACK);
				} else {
					USART2_Putc(NACK);
				}
				break;
			case 19:
				if (frameData.VALUE <= 5) {
					sysctl->pumpMode = 	frameData.VALUE;		
					USART2_Putc(ACK);
				} else {
					USART2_Putc(NACK);
				}
				break;
			case 40:
				if (frameData.VALUE <= 999999 && frameData.AI <= 9) {
				
					USART2_Putc(ACK);
				} else {
					USART2_Putc(NACK);
				}
				break;
			case 41:
				if (frameData.VALUE <= 9999) {
				
					USART2_Putc(ACK);
				} else {
					USART2_Putc(NACK);
				}
				break;
			case 42:
				if (frameData.VALUE <= 999999) {
				
					USART2_Putc(ACK);
				} else {
					USART2_Putc(NACK);
				}
				break;
			case 43:
				
				USART2_Putc(ACK);
				break;
			case 44:
				
				USART2_Putc(ACK);
				break;
			default:
				USART2_Putc(NACK);
				break;
			}
		} else {
			sendPressCnt++;
			if (sysctl->sendPressInterval != 0 && sendPressCnt >= sysctl->sendPressInterval * 5) {
				sprintf(SendDataStr, "!%2d090%06d", EQUID, sysctl->nowPress);
				sprintf(SendCRCStr, "%03d", CalCRC(SendDataStr, 12));
				memcpy(SendFrameStr, SendDataStr, 12);
				memcpy(SendFrameStr+12, SendCRCStr, 3);
				USART2_Puts(SendFrameStr);
			}
		}
		
		
		nowPLCCMDState = ISPLCRUN;			 
		
		if (sysctl->interface == WORKING) {
			if (lastPLCCMDState == 1 && nowPLCCMDState == 0) {
				keyBufTmp = KEYSTART;
				OSMboxPostOpt(KeyMbox, (void *)&keyBufTmp, OS_POST_OPT_BROADCAST);
			}
			else if (lastPLCCMDState == 0 && nowPLCCMDState == 1) {
				keyBufTmp = KEYSTOP;
				OSMboxPostOpt(KeyMbox, (void *)&keyBufTmp, OS_POST_OPT_BROADCAST);				
			}			
		}
		lastPLCCMDState = nowPLCCMDState;
	}
}
