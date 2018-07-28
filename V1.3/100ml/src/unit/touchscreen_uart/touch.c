#include "include.h"
#include "touch.h"
extern void USART3_HEX(u8 *str, u8 len);
extern s16 T1,T2,T3,T4,T5,T6,T7,T8,T9;
extern u16 JiShiTime1, JiShiTime2, JiShiTime3, JiShiTime4;

static struct button_press buttonbit[4] = {{0,0,0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}}; 
static u8 setpowerstate[4] = {0, 0, 0, 0};
static u16 setpowertmp[4] = {0, 0, 0, 0};

static struct response readd = {0, 0, 0, 0};

static u16 littletobig(u16 a)
{
 	return (((a & 0x00FF) << 8) | ((a & 0xFF00) >> 8));
}

void makepack(u8 cmd, u8 *pack, u8 len)
{
 	u8 sendtmp[25];
	u8 i;
	sendtmp[0] = 0xA5;
	sendtmp[1] = 0x5A;
	sendtmp[2] = 0xFF;
	sendtmp[3] = cmd;
	memcpy((char*)sendtmp+4, (const char*)pack, len*2);
	for (i = 0; i < len; i++) {
		*((u16*)(sendtmp+4+i*2)) = littletobig(*((u16*)(sendtmp+4+i*2)));
	}
	sendtmp[2] = len*2 + 1;
//	USART2_HEX(sendtmp, len*2+4);
	USART3_HEX(sendtmp, len*2+4);
}

void makecurve(u8 channel, u8 *data, u8 len)
{
 	u8 sendtmp[25];
	u8 i;
	sendtmp[0] = 0xA5;
	sendtmp[1] = 0x5A;
	sendtmp[2] = 0xFF;
	sendtmp[3] = 0x84;
	sendtmp[4] = channel;
	memcpy((char*)sendtmp+5, (const char*)data, len*2);
	for (i = 0; i < len; i++) {
		*((u16*)(sendtmp+5+i*2)) = littletobig(*((u16*)(sendtmp+5+i*2)));
	}
	sendtmp[2] = len*2 + 2;
	USART3_HEX(sendtmp, len*2+5);
}

static u8 bleezestate = 0;
static u8 buttonen = 0;
static u8 buttonstate[4][3] = {{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}};
void execute_str(u8 dstr)
{
	static u8 pos = 0;
	static u8 len = 0;
	if (pos >= 4) {
		switch(readd.cmd) {
		case 0x80:
			break;
		case 0x81:
			break;
		case 0x82:
			break;
		case 0x83:
			pos++;
			if (pos < 7)
				readd.addr += dstr << (pos==5?8:0);
			else if (pos == 7)
				readd.len = dstr;
			else if (pos >= 8 && pos < 8+readd.len*2) 
				readd.data += dstr << (pos==8?8:0);
			
			if (pos == 7+readd.len*2) {
				switch(readd.addr) {
				case 0x0015: 
					setpowerstate[0] = 1;
					setpowertmp[0] = readd.data;
					break;
				case 0x0025: 
					setpowerstate[1] = 1;
					setpowertmp[1] = readd.data;
					break;
				case 0x0035: 
					setpowerstate[2] = 1;
					setpowertmp[2] = readd.data;
					break;
				case 0x0045: 
					setpowerstate[3] = 1;
					setpowertmp[3] = readd.data;
					break;
				case 0x0000:
					buttonen = 1;
				 	switch(readd.data) {
					case 0x1000:
						if (buttonstate[0][0] == 0 
							&& buttonstate[0][1] == 0 
							&& buttonstate[0][2] == 0
							&& bleezestate == 0) {
							buttonbit[0].unlock = 1;
						}
						break;
					case 0x2000:
						if (buttonstate[1][0] == 0 
							&& buttonstate[1][1] == 0 
							&& buttonstate[1][2] == 0
							&& bleezestate == 0) {
							buttonbit[1].unlock = 1;
						}
						break;
					case 0x3000:
						if (buttonstate[2][0] == 0 
							&& buttonstate[2][1] == 0 
							&& buttonstate[2][2] == 0) {
							buttonbit[2].unlock = 1;
						}
						break;
					case 0x4000:
						if (buttonstate[3][0] == 0 
							&& buttonstate[3][1] == 0 
							&& buttonstate[3][2] == 0
							&& bleezestate == 0) {
							buttonbit[3].unlock = 1;
						}
						break;
					case 0x0010:
						if (buttonstate[0][0] == 0 
							&& buttonstate[0][1] == 0 
							&& buttonstate[0][2] == 0
							&& bleezestate == 0) {
							buttonstate[0][0] = 1;
							buttonbit[0].freeze = 1;
						} 
						break;
					case 0x0050:
						if (buttonstate[0][0] == 1){
							buttonstate[0][0] = 0;
							buttonbit[0].freeze = 2;
						}
						break;
					case 0x0020:
						if (buttonstate[1][0] == 0 
							&& buttonstate[1][1] == 0 
							&& buttonstate[1][2] == 0
							&& bleezestate == 0) {
							buttonstate[1][0] = 1;
							buttonbit[1].freeze = 1;
						}
						break;
					case 0x0060:
						if (buttonstate[1][0] == 1) {
							buttonstate[1][0] = 0;
							buttonbit[1].freeze = 2;
						}
						break;
					case 0x0030:
						if (buttonstate[2][0] == 0 
							&& buttonstate[2][1] == 0 
							&& buttonstate[2][2] == 0
							&& bleezestate == 0) {
							buttonstate[2][0] = 1;
							buttonbit[2].freeze = 1;
						} 
						break;
					case 0x0070:
						if (buttonstate[2][0] == 1) {
							buttonstate[2][0] = 0;
							buttonbit[2].freeze = 2;
						}
						break;
					case 0x0040:
						if (buttonstate[3][0] == 0 
							&& buttonstate[3][1] == 0 
							&& buttonstate[3][2] == 0
							&& bleezestate == 0) {
							buttonstate[3][0] = 1;
							buttonbit[3].freeze = 1;
						} 
						break;
					case 0x0080:
						if (buttonstate[3][0] == 1) {
							buttonstate[3][0] = 0;
							buttonbit[3].freeze = 2;
						}
						break;
					case 0x0001:
						if (buttonstate[0][1] == 0 
							&& buttonstate[0][0] == 0 
							&& buttonstate[0][2] == 0
							&& bleezestate == 0) {
							buttonstate[0][1] = 1;
							buttonbit[0].thaw = 1;
						} 
						break;
					case 0x0005:
						if (buttonstate[0][1] == 1) {
							buttonstate[0][1] = 0;
							buttonbit[0].thaw = 2;
						}
						break;
					case 0x0002:
						if (buttonstate[1][1] == 0 
							&& buttonstate[1][0] == 0 
							&& buttonstate[1][2] == 0
							&& bleezestate == 0) {
							buttonstate[1][1] = 1;
							buttonbit[1].thaw = 1;
						} 
						break;
					case 0x0006:
						if (buttonstate[1][1] == 1) {
							buttonstate[1][1] = 0;
							buttonbit[1].thaw = 2;
						}
						break;
					case 0x0003:
						if (buttonstate[2][1] == 0 
							&& buttonstate[2][0] == 0 
							&& buttonstate[2][2] == 0
							&& bleezestate == 0) {
							buttonstate[2][1] = 1;
							buttonbit[2].thaw = 1;
						} 
						break;
					case 0x0007:
						if (buttonstate[2][1] == 1) {
							buttonstate[2][1] = 0;
							buttonbit[2].thaw = 2;
						}
						break;
					case 0x0004:
						if (buttonstate[3][1] == 0 
							&& buttonstate[3][0] == 0 
							&& buttonstate[3][2] == 0
							&& bleezestate == 0) {
							buttonstate[3][1] = 1;
							buttonbit[3].thaw = 1;
						} 
						break;
					case 0x0008:
						if (buttonstate[3][1] == 1) {
							buttonstate[3][1] = 0;
							buttonbit[3].thaw = 2;
						}
						break;
					case 0x0100:
						if (buttonstate[0][2] == 0 
							&& buttonstate[0][0] == 0 
							&& buttonstate[0][1] == 0
							&& bleezestate == 0) {
							buttonstate[0][2] = 1;
							buttonbit[0].stick = 1;
						} 
						break;
					case 0x0500:
						if (buttonstate[0][2] == 1) {
							buttonstate[0][2] = 0;
							buttonbit[0].stick = 2;
						}
						break;
					case 0x0200:
						if (buttonstate[1][2] == 0 
							&& buttonstate[1][0] == 0 
							&& buttonstate[1][1] == 0
							&& bleezestate == 0) {
							buttonstate[1][2] = 1;
							buttonbit[1].stick = 1;
						} 					
						break;
					case 0x0600:
						if (buttonstate[1][2] == 1) {
							buttonstate[1][2] = 0;
							buttonbit[1].stick = 2;
						}
						break;
					case 0x0300:
						if (buttonstate[2][2] == 0 
							&& buttonstate[2][0] == 0 
							&& buttonstate[2][1] == 0
							&& bleezestate == 0) {
							buttonstate[2][2] = 1;
							buttonbit[2].stick = 1;
						}
						break;
					case 0x0700:
						 if (buttonstate[2][2] == 1) {
							buttonstate[2][2] = 0;
							buttonbit[2].stick = 2;
						}
						break;
					case 0x0400:
						if (buttonstate[3][2] == 0 
							&& buttonstate[3][0] == 0 
							&& buttonstate[3][1] == 0
							&& bleezestate == 0) {
							buttonstate[3][2] = 1;
							buttonbit[3].stick = 1;
						} 
						break;
					case 0x0800:
					 	if (buttonstate[3][2] == 1) {
							buttonstate[3][2] = 0;
							buttonbit[3].stick = 2;
						}
						break;
					case 0x0055:
						if (bleezestate == 0) {
							bleezestate = 1;
							buttonbit[0].bleeze = 1;
						}
						break;
					case 0x0066:
						buttonbit[0].stopall = 1;
						break;
					case 0x00F1:
						if (setpowerstate[0] == 2) {
							setpowerstate[0] = 3;
						}
						break;
					case 0x00F2:
						if (setpowerstate[1] == 2) {
							setpowerstate[1] = 3;
						}
						break;
					case 0x00F3:
						if (setpowerstate[2] == 2) {
							setpowerstate[2] = 3;
						}
						break;
					case 0x00F4:
						if (setpowerstate[3] == 2) {
							setpowerstate[3] = 3;
						}
						break;
					}
				}
 			}
			break;
		case 0x84:
			break;
		}
	}

	switch(pos) {
	case 2:
	 	len = dstr;				
		pos++;
		break;
	case 3:
		readd.cmd = dstr;		
		pos++;
		break;
	}

	if (pos - 3 > len)				//越界
		pos = 0;	

	if (pos == 7+readd.len*2) {
//		USART3_Putc(readd.data>>8);
//		USART3_Putc(readd.data);
		readd.cmd = 0;
		readd.addr = 0;
		readd.data = 0;
		pos = 0;
		len = 0;
	}

	switch(dstr) {
	case 0xA5:
		pos = 1;
		break;
	case 0x5A:
		if (pos == 1) 
			pos = 2;
		break;
	}
}

void ts_init(void)
{
	u16 sendtmp[2];
	sendtmp[0] = 0x0014;
	sendtmp[1] = 100;
	makepack(0x82, (u8*)sendtmp, 2);
	sendtmp[0] = 0x0024;
	sendtmp[1] = 100;
	makepack(0x82, (u8*)sendtmp, 2);
	sendtmp[0] = 0x0034;
	sendtmp[1] = 100;
	makepack(0x82, (u8*)sendtmp, 2);
	sendtmp[0] = 0x0044;
	sendtmp[1] = 100;
	makepack(0x82, (u8*)sendtmp, 2);
	sendtmp[0] = 0x0015;
	sendtmp[1] = 100;
	makepack(0x82, (u8*)sendtmp, 2);
	sendtmp[0] = 0x0025;
	sendtmp[1] = 100;
	makepack(0x82, (u8*)sendtmp, 2);
	sendtmp[0] = 0x0035;
	sendtmp[1] = 100;
	makepack(0x82, (u8*)sendtmp, 2);
	sendtmp[0] = 0x0045;
	sendtmp[1] = 100;
	makepack(0x82, (u8*)sendtmp, 2);

}

void rollinmain(void)
{
	static u16 sendtmp[20];
	//Time发送	P1
	sendtmp[0] = 0x0010;
	sendtmp[1] = (JiShiTime1/60/10 << 12) + (JiShiTime1/60%10 << 8)
					+ (JiShiTime1%60/10 << 4) + (JiShiTime1%60%10);
	makepack(0x82, (u8*)sendtmp, 2);	
	//Time发送	P2	
	sendtmp[0] = 0x0020;
	sendtmp[1] = (JiShiTime2/60/10 << 12) + (JiShiTime2/60%10 << 8)
					+ (JiShiTime2%60/10 << 4) + (JiShiTime2%60%10);
	makepack(0x82, (u8*)sendtmp, 2);	
	//Time发送	P3	
	sendtmp[0] = 0x0030;
	sendtmp[1] = (JiShiTime3/60/10 << 12) + (JiShiTime3/60%10 << 8)
					+ (JiShiTime3%60/10 << 4) + (JiShiTime3%60%10);
	makepack(0x82, (u8*)sendtmp, 2);	
	//Time发送	P4	
	sendtmp[0] = 0x0040;
	sendtmp[1] = (JiShiTime4/60/10 << 12) + (JiShiTime4/60%10 << 8)
					+ (JiShiTime4%60/10 << 4) + (JiShiTime4%60%10);
	makepack(0x82, (u8*)sendtmp, 2);	
	//Temp发送	P1
	sendtmp[0] = 0x0011;
	if (T1 <= 1500) {
		if (T1 >= 0) 
			sendtmp[1] = (' '<<8) + (0x30+abs(T1)/1000);
		else
			sendtmp[1] = ('-'<<8) + (0x30+abs(T1)/1000);
		sendtmp[2] = ((0x30+abs(T1)%1000/100)<<8) + (0x30+abs(T1)%100/10);
		sendtmp[3] = ('.'<<8) + (0x30+abs(T1)%10);
	} else {
		sendtmp[1] = (' '<<8) + ('O');
		sendtmp[2] = ('P'<<8) + ('E');
		sendtmp[3] = ('N'<<8) + (' ');
	}
	makepack(0x82, (u8*)sendtmp, 4);	
	//Temp发送	P2	
	sendtmp[0] = 0x0021;
	if (T2 <= 1500) {
		if (T2 >= 0) 
			sendtmp[1] = (' '<<8) + (0x30+abs(T2)/1000);
		else
			sendtmp[1] = ('-'<<8) + (0x30+abs(T2)/1000);
		sendtmp[2] = ((0x30+abs(T2)%1000/100)<<8) + (0x30+abs(T2)%100/10);
		sendtmp[3] = ('.'<<8) + (0x30+abs(T2)%10);
	} else {
		sendtmp[1] = (' '<<8) + ('O');
		sendtmp[2] = ('P'<<8) + ('E');
		sendtmp[3] = ('N'<<8) + (' ');
	}
	makepack(0x82, (u8*)sendtmp, 4);	
	//Temp发送	P3	
	sendtmp[0] = 0x0031;
	if (T3 <= 1500) {
		if (T3 >= 0) 
			sendtmp[1] = (' '<<8) + (0x30+abs(T3)/1000);
		else
			sendtmp[1] = ('-'<<8) + (0x30+abs(T3)/1000);
		sendtmp[2] = ((0x30+abs(T3)%1000/100)<<8) + (0x30+abs(T3)%100/10);
		sendtmp[3] = ('.'<<8) + (0x30+abs(T3)%10);
	} else {
		sendtmp[1] = (' '<<8) + ('O');
		sendtmp[2] = ('P'<<8) + ('E');
		sendtmp[3] = ('N'<<8) + (' ');
	}
	makepack(0x82, (u8*)sendtmp, 4);	
	//Temp发送	P4
	sendtmp[0] = 0x0041;
	if (T4 <= 1500) {
		if (T4 >= 0) 
			sendtmp[1] = (' '<<8) + (0x30+abs(T4)/1000);
		else
			sendtmp[1] = ('-'<<8) + (0x30+abs(T4)/1000);
		sendtmp[2] = ((0x30+abs(T4)%1000/100)<<8) + (0x30+abs(T4)%100/10);
		sendtmp[3] = ('.'<<8) + (0x30+abs(T4)%10);
	} else {
		sendtmp[1] = (' '<<8) + ('O');
		sendtmp[2] = ('P'<<8) + ('E');
		sendtmp[3] = ('N'<<8) + (' ');
	}
	makepack(0x82, (u8*)sendtmp, 4);	
	//温度曲线发送
	sendtmp[0] = (T1 + 1500) / 10;
	makecurve(0x01, (u8*)sendtmp, 1);	
	sendtmp[0] = (T2 + 1500) / 10;
	makecurve(0x02, (u8*)sendtmp, 1);	
	sendtmp[0] = (T3 + 1500) / 10;
	makecurve(0x04, (u8*)sendtmp, 1);	
	sendtmp[0] = (T4 + 1500) / 10;
	makecurve(0x08, (u8*)sendtmp, 1);	
	//Temp发送	T6	
	sendtmp[0] = 0x0061;
	if (T6 <= 1500) {
		if (T6 >= 0) 
			sendtmp[1] = (' '<<8) + (0x30+abs(T6)/1000);
		else
			sendtmp[1] = ('-'<<8) + (0x30+abs(T6)/1000);
		sendtmp[2] = ((0x30+abs(T6)%1000/100)<<8) + (0x30+abs(T6)%100/10);
		sendtmp[3] = ('.'<<8) + (0x30+abs(T6)%10);
	} else {
		sendtmp[1] = (' '<<8) + ('O');
		sendtmp[2] = ('P'<<8) + ('E');
		sendtmp[3] = ('N'<<8) + (' ');
	}
	makepack(0x82, (u8*)sendtmp, 4);	
	//Temp发送	T7	
	sendtmp[0] = 0x0071;
	if (T7 <= 1500) {
		if (T7 >= 0) 
			sendtmp[1] = (' '<<8) + (0x30+abs(T7)/1000);
		else
			sendtmp[1] = ('-'<<8) + (0x30+abs(T7)/1000);
		sendtmp[2] = ((0x30+abs(T7)%1000/100)<<8) + (0x30+abs(T7)%100/10);
		sendtmp[3] = ('.'<<8) + (0x30+abs(T7)%10);
	} else {
		sendtmp[1] = (' '<<8) + ('O');
		sendtmp[2] = ('P'<<8) + ('E');
		sendtmp[3] = ('N'<<8) + (' ');
	}
	makepack(0x82, (u8*)sendtmp, 4);	
	//Temp发送	T8	
	sendtmp[0] = 0x0081;
	if (T8 <= 1500) {
		if (T8 >= 0) 
			sendtmp[1] = (' '<<8) + (0x30+abs(T8)/1000);
		else
			sendtmp[1] = ('-'<<8) + (0x30+abs(T8)/1000);
		sendtmp[2] = ((0x30+abs(T8)%1000/100)<<8) + (0x30+abs(T8)%100/10);
		sendtmp[3] = ('.'<<8) + (0x30+abs(T8)%10);
	} else {
		sendtmp[1] = (' '<<8) + ('O');
		sendtmp[2] = ('P'<<8) + ('E');
		sendtmp[3] = ('N'<<8) + (' ');
	}
	makepack(0x82, (u8*)sendtmp, 4);	
	//Temp发送	T9
	sendtmp[0] = 0x0091;
	if (T9 <= 1500) {
		if (T9 >= 0) 
			sendtmp[1] = (' '<<8) + (0x30+abs(T9)/1000);
		else
			sendtmp[1] = ('-'<<8) + (0x30+abs(T9)/1000);
		sendtmp[2] = ((0x30+abs(T9)%1000/100)<<8) + (0x30+abs(T9)%100/10);
		sendtmp[3] = ('.'<<8) + (0x30+abs(T9)%10);
	} else {
		sendtmp[1] = (' '<<8) + ('O');
		sendtmp[2] = ('P'<<8) + ('E');
		sendtmp[3] = ('N'<<8) + (' ');
	}
	makepack(0x82, (u8*)sendtmp, 4);	
	//预冷温度值&进度条
	sendtmp[0] = 0x0051;
	if (T5 <= 1500) {
		if (T5 >= 0) 
			sendtmp[1] = (' '<<8) + (0x30+abs(T5)/1000);
		else
			sendtmp[1] = ('-'<<8) + (0x30+abs(T5)/1000);
		sendtmp[2] = ((0x30+abs(T5)%1000/100)<<8) + (0x30+abs(T5)%100/10);
		sendtmp[3] = ('.'<<8) + (0x30+abs(T5)%10);
	} else {
		sendtmp[1] = (' '<<8) + ('O');
		sendtmp[2] = ('P'<<8) + ('E');
		sendtmp[3] = ('N'<<8) + (' ');
	}
	makepack(0x82, (u8*)sendtmp, 4);
	sendtmp[0] = 0x5000;
	sendtmp[1] = 0x0006;
	sendtmp[2] = 0x0001;
	sendtmp[3] = 0x0001;
	sendtmp[4] = GeiLiStart >> 16;
	sendtmp[5] = (u16)GeiLiStart;
	if (T5 < 0){	
		sendtmp[6] = GeiLiAdj(abs(T5)*604/1700) >> 16;
		sendtmp[7] = (u16)GeiLiAdj(abs(T5)*604/1700);
	} else {
		sendtmp[6] = GeiLiAdj(0) >> 16;
		sendtmp[7] = (u16)GeiLiAdj(0);
	}
	sendtmp[8] = GeiLiStart >> 16;
	sendtmp[9] = (u16)GeiLiStart;
	sendtmp[10] = 0xFF00;
	makepack(0x82, (u8*)sendtmp, 11);
}

void ctl_ts(u8 cmd);
char rollquick_checkbutton(char ts_set)
{
	switch (ts_set) {
	case 1:
		buttonstate[0][0] = 1;
		buttonbit[0].freeze = 1;
		break;
	case 2:
		buttonstate[0][0] = 0;
		buttonbit[0].freeze = 2;
		break;
	case 3:
		buttonstate[1][0] = 1;
		buttonbit[1].freeze = 1;
		break;
	case 4:
		buttonstate[1][0] = 0;
		buttonbit[1].freeze = 2;
		break;
	case 5:
		buttonstate[2][0] = 1;
		buttonbit[2].freeze = 1;
		break;
	case 6:
		buttonstate[2][0] = 0;
		buttonbit[2].freeze = 2;
		break;
	case 7:
		buttonstate[3][0] = 1;
		buttonbit[3].freeze = 1;
		break;
	case 8:
		buttonstate[3][0] = 0;
		buttonbit[3].freeze = 2;
		break;
	case 9:
		buttonstate[0][1] = 1;
		buttonbit[0].thaw = 1;
		break;
	case 10:
		buttonstate[0][1] = 0;
		buttonbit[0].thaw = 2;
		break;
	case 11:
		buttonstate[1][1] = 1;
		buttonbit[1].thaw = 1;
		break;
	case 12:
		buttonstate[1][1] = 0;
		buttonbit[1].thaw = 2;
		break;
	case 13:
		buttonstate[2][1] = 1;
		buttonbit[2].thaw = 1;
		break;
	case 14:
		buttonstate[2][1] = 0;
		buttonbit[2].thaw = 2;
		break;
	case 15:
		buttonstate[3][1] = 1;
		buttonbit[3].thaw = 1;
		break;
	case 16:
		buttonstate[3][1] = 0;
		buttonbit[3].thaw = 2;
		break;
	case 17:
		buttonstate[0][2] = 1;
		buttonbit[0].stick = 1;
		break;
	case 18:
		buttonstate[0][2] = 0;
		buttonbit[0].stick = 2;
		break;
	case 19:
		buttonstate[1][2] = 1;
		buttonbit[1].stick = 1;
		break;
	case 20:
		buttonstate[1][2] = 0;
		buttonbit[1].stick = 2;
		break;
	case 21:
		buttonstate[2][2] = 1;
		buttonbit[2].stick = 1;
		break;
	case 22:
		buttonstate[2][2] = 0;
		buttonbit[2].stick = 2;
		break;
	case 23:
		buttonstate[3][2] = 1;
		buttonbit[3].stick = 1;
		break;
	case 24:
		buttonstate[3][2] = 0;
		buttonbit[3].stick = 2;
		break;
	case 25:
		buttonbit[0].bleeze = 1;
		break;
	case 26:
		buttonbit[0].bleeze = 2;
		break;
	case 27:
		buttonbit[0].unlock = 1;
		break;
	case 28:
		buttonbit[1].unlock = 1;
		break;
	case 29:
		buttonbit[2].unlock = 1;
		break;
	case 30:
		buttonbit[3].unlock = 1;
		break;
	case 31:
		buttonbit[0].freeze = 0;buttonbit[1].freeze = 0;buttonbit[2].freeze = 0;buttonbit[3].freeze = 0;
		buttonstate[0][0] = 0;buttonstate[1][0] = 0;buttonstate[2][0] = 0;buttonstate[3][0] = 0;
		buttonbit[0].thaw = 0;buttonbit[1].thaw = 0;buttonbit[2].thaw = 0;buttonbit[3].thaw = 0;
		buttonstate[0][1] = 0;buttonstate[1][1] = 0;buttonstate[2][1] = 0;buttonstate[3][1] = 0;
		buttonbit[0].stick = 0;buttonbit[1].stick = 0;buttonbit[2].stick = 0;buttonbit[3].stick = 0;
		buttonstate[0][2] = 0;buttonstate[1][2] = 0;buttonstate[2][2] = 0;buttonstate[3][2] = 0;
		ctl_ts(2);ctl_ts(4);ctl_ts(6);ctl_ts(8);
		ctl_ts(10);ctl_ts(12);ctl_ts(14);ctl_ts(16);
		ctl_ts(18);ctl_ts(20);ctl_ts(22);ctl_ts(24);
		break;
	}
	switch (ts_set >> 4) {
	case 0x0E:
		setpowerstate[0] = 3;
		setpowertmp[0] = (ts_set&0x0F) * 10;
		break;
	case 0x0D:
		setpowerstate[1] = 3;
		setpowertmp[1] = (ts_set&0x0F) * 10;
		break;
	case 0x0B:
		setpowerstate[2] = 3;
		setpowertmp[2] = (ts_set&0x0F) * 10;
		break;
	case 0x07:
		setpowerstate[3] = 3;
		setpowertmp[3] = (ts_set&0x0F) * 10;
		break;
	}
/**********Clear Button State**************/
	if (buttonen == 1) {
		buttonen = 0;
		ctl_ts(27);
	}
/****************************************/
/**********Set Power Button**************/
	if (setpowerstate[0] == 1) {
		setpowerstate[0] = 2;
		ctl_ts(28);
	} else if (setpowerstate[0] == 3) {
		setpowerstate[0] = 0;
		ctl_ts(29);
		return (setpowertmp[0]/10+(0x0E<<4));
	} else if (setpowerstate[1] == 1) {
		setpowerstate[1] = 2;
		ctl_ts(30);
	} else if (setpowerstate[1] == 3) {
		setpowerstate[1] = 0;
		ctl_ts(31);
		return (setpowertmp[1]/10+(0x0D<<4));
	} else if (setpowerstate[2] == 1) {
		setpowerstate[2] = 2;
		ctl_ts(32);
	} else if (setpowerstate[2] == 3) {
		setpowerstate[2] = 0;
		ctl_ts(33);
		return (setpowertmp[2]/10+(0x0B<<4));
	} else if (setpowerstate[3] == 1) {
		setpowerstate[3] = 2;
		ctl_ts(34);
	} else if (setpowerstate[3] == 3) {
		setpowerstate[3] = 0;
		ctl_ts(35);
		return (setpowertmp[3]/10+(0x07<<4));
	}
/****************************************/
/***********Freeze Button****************/
	if (buttonbit[0].freeze == 1) {
		buttonbit[0].freeze = 0;
		ctl_ts(1);
		return 1;
	} else if (buttonbit[0].freeze == 2) {
		buttonbit[0].freeze = 0;
		ctl_ts(2);
		return 2;
	} 
	if (buttonbit[1].freeze == 1) {
		buttonbit[1].freeze = 0;
		ctl_ts(3);
		return 3;
	} else if (buttonbit[1].freeze == 2) {
		buttonbit[1].freeze = 0;
		ctl_ts(4);
		return 4;
	} 
	if (buttonbit[2].freeze == 1) {
		buttonbit[2].freeze = 0;
		ctl_ts(5);
		return 5;
	} else if (buttonbit[2].freeze == 2) {
		buttonbit[2].freeze = 0;
		ctl_ts(6);
		return 6;
	} 
	if (buttonbit[3].freeze == 1) {
		buttonbit[3].freeze = 0;
		ctl_ts(7);
		return 7;
	} else if (buttonbit[3].freeze == 2) {
		buttonbit[3].freeze = 0;
		ctl_ts(8);
		return 8;
	}
/****************************************/
/*************Thaw Button****************/
	if (buttonbit[0].thaw == 1) {
		buttonbit[0].thaw = 0;
		ctl_ts(9);
		return 9;
	} else if (buttonbit[0].thaw == 2) {
		buttonbit[0].thaw = 0;
		ctl_ts(10);
		return 10;
	} 
	if (buttonbit[1].thaw == 1) {
		buttonbit[1].thaw = 0;
		ctl_ts(11);
		return 11;
	} else if (buttonbit[1].thaw == 2) {
		buttonbit[1].thaw = 0;
		ctl_ts(12);
		return 12;
	} 
	if (buttonbit[2].thaw == 1) {
		buttonbit[2].thaw = 0;
		ctl_ts(13);
		return 13;
	} else if (buttonbit[2].thaw == 2) {
		buttonbit[2].thaw = 0;
		ctl_ts(14);
		return 14;
	} 
	if (buttonbit[3].thaw == 1) {
		buttonbit[3].thaw = 0;
		ctl_ts(15);
		return 15;
	} else if (buttonbit[3].thaw == 2) {
		buttonbit[3].thaw = 0;
		ctl_ts(16);
		return 16;
	}
/****************************************/
/*************Stick Button****************/
	if (buttonbit[0].stick == 1) {
		buttonbit[0].stick = 0;
		ctl_ts(17);
		return 17;
	} else if (buttonbit[0].stick == 2) {
		buttonbit[0].stick = 0;
		ctl_ts(18);
		return 18;
	} 
	if (buttonbit[1].stick == 1) {
		buttonbit[1].stick = 0;
		ctl_ts(19);
		return 19;
	} else if (buttonbit[1].stick == 2) {
		buttonbit[1].stick = 0;
		ctl_ts(20);
		return 20;
	} 
	if (buttonbit[2].stick == 1) {
		buttonbit[2].stick = 0;
		ctl_ts(21);
		return 21;
	} else if (buttonbit[2].stick == 2) {
		buttonbit[2].stick = 0;
		ctl_ts(22);
		return 22;
	} 
	if (buttonbit[3].stick == 1) {
		buttonbit[3].stick = 0;
		ctl_ts(23);
		return 23;
	} else if (buttonbit[3].stick == 2) {
		buttonbit[3].stick = 0;
		ctl_ts(24);
		return 24;
	}
/****************************************/
/*************Bleeze Button****************/
	if (buttonbit[0].bleeze == 1) {
		buttonbit[0].bleeze = 0;
		ctl_ts(25);
		return 25;
	} else if (buttonbit[0].bleeze == 2) {
		buttonbit[0].bleeze = 0;
		ctl_ts(26);
		bleezestate = 0;
	}
/****************************************/
/*************Unlock Button****************/
	if (buttonbit[0].unlock == 1) {
		buttonbit[0].unlock = 0;
//		ctl_ts(27);
		return 27;
	} else 	if (buttonbit[1].unlock == 1) {
		buttonbit[1].unlock = 0;
//		ctl_ts(27);
		return 28;
	} else 	if (buttonbit[2].unlock == 1) {
		buttonbit[2].unlock = 0;
//		ctl_ts(27);
		return 29;
	} else 	if (buttonbit[3].unlock == 1) {
		buttonbit[3].unlock = 0;
//		ctl_ts(27);
		return 30;
	}				
/****************************************/
/*************Stop All Button****************/
	if (buttonbit[0].stopall == 1) {
		buttonbit[0].stopall = 0;
		return 31;
	}
/****************************************/
	return 0;
}

void ctl_ts(u8 cmd)
{
	u16 sendtmp[20];
	switch (cmd) {
	case 1:
		sendtmp[0] = 0x1030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0001;
		sendtmp[4] = Freeze0Start >> 16;
		sendtmp[5] = (u16)Freeze0Start;
		sendtmp[6] = Freeze0End >> 16;
		sendtmp[7] = (u16)Freeze0End;
		sendtmp[8] = Freeze0Start >> 16;
		sendtmp[9] = (u16)Freeze0Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 2:
		sendtmp[0] = 0x1030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0000;
		sendtmp[4] = Freeze0Start >> 16;
		sendtmp[5] = (u16)Freeze0Start;
		sendtmp[6] = Freeze0End >> 16;
		sendtmp[7] = (u16)Freeze0End;
		sendtmp[8] = Freeze0Start >> 16;
		sendtmp[9] = (u16)Freeze0Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 3:
		sendtmp[0] = 0x2030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0001;
		sendtmp[4] = Freeze1Start >> 16;
		sendtmp[5] = (u16)Freeze1Start;
		sendtmp[6] = Freeze1End >> 16;
		sendtmp[7] = (u16)Freeze1End;
		sendtmp[8] = Freeze1Start >> 16;
		sendtmp[9] = (u16)Freeze1Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 4:
		sendtmp[0] = 0x2030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0000;
		sendtmp[4] = Freeze1Start >> 16;
		sendtmp[5] = (u16)Freeze1Start;
		sendtmp[6] = Freeze1End >> 16;
		sendtmp[7] = (u16)Freeze1End;
		sendtmp[8] = Freeze1Start >> 16;
		sendtmp[9] = (u16)Freeze1Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 5:
		sendtmp[0] = 0x3030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0001;
		sendtmp[4] = Freeze2Start >> 16;
		sendtmp[5] = (u16)Freeze2Start;
		sendtmp[6] = Freeze2End >> 16;
		sendtmp[7] = (u16)Freeze2End;
		sendtmp[8] = Freeze2Start >> 16;
		sendtmp[9] = (u16)Freeze2Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 6:
		sendtmp[0] = 0x3030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0000;
		sendtmp[4] = Freeze2Start >> 16;
		sendtmp[5] = (u16)Freeze2Start;
		sendtmp[6] = Freeze2End >> 16;
		sendtmp[7] = (u16)Freeze2End;
		sendtmp[8] = Freeze2Start >> 16;
		sendtmp[9] = (u16)Freeze2Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 7:
		sendtmp[0] = 0x4030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0001;
		sendtmp[4] = Freeze3Start >> 16;
		sendtmp[5] = (u16)Freeze3Start;
		sendtmp[6] = Freeze3End >> 16;
		sendtmp[7] = (u16)Freeze3End;
		sendtmp[8] = Freeze3Start >> 16;
		sendtmp[9] = (u16)Freeze3Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 8:
		sendtmp[0] = 0x4030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0000;
		sendtmp[4] = Freeze3Start >> 16;
		sendtmp[5] = (u16)Freeze3Start;
		sendtmp[6] = Freeze3End >> 16;
		sendtmp[7] = (u16)Freeze3End;
		sendtmp[8] = Freeze3Start >> 16;
		sendtmp[9] = (u16)Freeze3Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 9:
		sendtmp[0] = 0x1030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0001;
		sendtmp[4] = Thaw0Start >> 16;
		sendtmp[5] = (u16)Thaw0Start;
		sendtmp[6] = Thaw0End >> 16;
		sendtmp[7] = (u16)Thaw0End;
		sendtmp[8] = Thaw0Start >> 16;
		sendtmp[9] = (u16)Thaw0Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 10:
		sendtmp[0] = 0x1030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0000;
		sendtmp[4] = Freeze0Start >> 16;
		sendtmp[5] = (u16)Freeze0Start;
		sendtmp[6] = Freeze0End >> 16;
		sendtmp[7] = (u16)Freeze0End;
		sendtmp[8] = Freeze0Start >> 16;
		sendtmp[9] = (u16)Freeze0Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 11:
		sendtmp[0] = 0x2030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0001;
		sendtmp[4] = Thaw1Start >> 16;
		sendtmp[5] = (u16)Thaw1Start;
		sendtmp[6] = Thaw1End >> 16;
		sendtmp[7] = (u16)Thaw1End;
		sendtmp[8] = Thaw1Start >> 16;
		sendtmp[9] = (u16)Thaw1Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 12:
		sendtmp[0] = 0x2030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0000;
		sendtmp[4] = Thaw1Start >> 16;
		sendtmp[5] = (u16)Thaw1Start;
		sendtmp[6] = Thaw1End >> 16;
		sendtmp[7] = (u16)Thaw1End;
		sendtmp[8] = Thaw1Start >> 16;
		sendtmp[9] = (u16)Thaw1Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 13:
		sendtmp[0] = 0x3030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0001;
		sendtmp[4] = Thaw2Start >> 16;
		sendtmp[5] = (u16)Thaw2Start;
		sendtmp[6] = Thaw2End >> 16;
		sendtmp[7] = (u16)Thaw2End;
		sendtmp[8] = Thaw2Start >> 16;
		sendtmp[9] = (u16)Thaw2Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 14:
		sendtmp[0] = 0x3030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0000;
		sendtmp[4] = Thaw2Start >> 16;
		sendtmp[5] = (u16)Thaw2Start;
		sendtmp[6] = Thaw2End >> 16;
		sendtmp[7] = (u16)Thaw2End;
		sendtmp[8] = Thaw2Start >> 16;
		sendtmp[9] = (u16)Thaw2Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 15:
		sendtmp[0] = 0x4030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0001;
		sendtmp[4] = Thaw3Start >> 16;
		sendtmp[5] = (u16)Thaw3Start;
		sendtmp[6] = Thaw3End >> 16;
		sendtmp[7] = (u16)Thaw3End;
		sendtmp[8] = Thaw3Start >> 16;
		sendtmp[9] = (u16)Thaw3Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 16:
		sendtmp[0] = 0x4030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0000;
		sendtmp[4] = Thaw3Start >> 16;
		sendtmp[5] = (u16)Thaw3Start;
		sendtmp[6] = Thaw3End >> 16;
		sendtmp[7] = (u16)Thaw3End;
		sendtmp[8] = Thaw3Start >> 16;
		sendtmp[9] = (u16)Thaw3Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 17:
		sendtmp[0] = 0x1030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0001;
		sendtmp[4] = Stick0Start >> 16;
		sendtmp[5] = (u16)Stick0Start;
		sendtmp[6] = Stick0End >> 16;
		sendtmp[7] = (u16)Stick0End;
		sendtmp[8] = Stick0Start >> 16;
		sendtmp[9] = (u16)Stick0Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 18:
		sendtmp[0] = 0x1030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0000;
		sendtmp[4] = Stick0Start >> 16;
		sendtmp[5] = (u16)Stick0Start;
		sendtmp[6] = Stick0End >> 16;
		sendtmp[7] = (u16)Stick0End;
		sendtmp[8] = Stick0Start >> 16;
		sendtmp[9] = (u16)Stick0Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 19:
		sendtmp[0] = 0x2030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0001;
		sendtmp[4] = Stick1Start >> 16;
		sendtmp[5] = (u16)Stick1Start;
		sendtmp[6] = Stick1End >> 16;
		sendtmp[7] = (u16)Stick1End;
		sendtmp[8] = Stick1Start >> 16;
		sendtmp[9] = (u16)Stick1Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 20:
		sendtmp[0] = 0x2030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0000;
		sendtmp[4] = Stick1Start >> 16;
		sendtmp[5] = (u16)Stick1Start;
		sendtmp[6] = Stick1End >> 16;
		sendtmp[7] = (u16)Stick1End;
		sendtmp[8] = Stick1Start >> 16;
		sendtmp[9] = (u16)Stick1Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 21:
		sendtmp[0] = 0x3030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0001;
		sendtmp[4] = Stick2Start >> 16;
		sendtmp[5] = (u16)Stick2Start;
		sendtmp[6] = Stick2End >> 16;
		sendtmp[7] = (u16)Stick2End;
		sendtmp[8] = Stick2Start >> 16;
		sendtmp[9] = (u16)Stick2Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 22:
		sendtmp[0] = 0x3030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0000;
		sendtmp[4] = Stick2Start >> 16;
		sendtmp[5] = (u16)Stick2Start;
		sendtmp[6] = Stick2End >> 16;
		sendtmp[7] = (u16)Stick2End;
		sendtmp[8] = Stick2Start >> 16;
		sendtmp[9] = (u16)Stick2Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 23:
		sendtmp[0] = 0x4030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0001;
		sendtmp[4] = Stick3Start >> 16;
		sendtmp[5] = (u16)Stick3Start;
		sendtmp[6] = Stick3End >> 16;
		sendtmp[7] = (u16)Stick3End;
		sendtmp[8] = Stick3Start >> 16;
		sendtmp[9] = (u16)Stick3Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 24:
		sendtmp[0] = 0x4030;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0000;
		sendtmp[4] = Stick3Start >> 16;
		sendtmp[5] = (u16)Stick3Start;
		sendtmp[6] = Stick3End >> 16;
		sendtmp[7] = (u16)Stick3End;
		sendtmp[8] = Stick3Start >> 16;
		sendtmp[9] = (u16)Stick3Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 25:
		sendtmp[0] = 0x1060;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0001;
		sendtmp[4] = BleezeStart >> 16;
		sendtmp[5] = (u16)BleezeStart;
		sendtmp[6] = BleezeEnd >> 16;
		sendtmp[7] = (u16)BleezeEnd;
		sendtmp[8] = BleezeStart >> 16;
		sendtmp[9] = (u16)BleezeStart;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 26:
		sendtmp[0] = 0x1060;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0000;
		sendtmp[4] = BleezeStart >> 16;
		sendtmp[5] = (u16)BleezeStart;
		sendtmp[6] = BleezeEnd >> 16;
		sendtmp[7] = (u16)BleezeEnd;
		sendtmp[8] = BleezeStart >> 16;
		sendtmp[9] = (u16)BleezeStart;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 27:
		sendtmp[0] = 0x0000;
		sendtmp[1] = 0x0000;
		makepack(0x82, (u8*)sendtmp, 2);
		break;
	case 28:
		sendtmp[0] = 0x1000;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0001;
		sendtmp[4] = SetPower0Start >> 16;
		sendtmp[5] = (u16)SetPower0Start;
		sendtmp[6] = SetPower0End >> 16;
		sendtmp[7] = (u16)SetPower0End;
		sendtmp[8] = SetPower0Start >> 16;
		sendtmp[9] = (u16)SetPower0Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 29:
		sendtmp[0] = 0x1000;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0000;
		sendtmp[4] = SetPower0Start >> 16;
		sendtmp[5] = (u16)SetPower0Start;
		sendtmp[6] = SetPower0End >> 16;
		sendtmp[7] = (u16)SetPower0End;
		sendtmp[8] = SetPower0Start >> 16;
		sendtmp[9] = (u16)SetPower0Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		sendtmp[0] = 0x0014;
		sendtmp[1] = setpowertmp[0];
		makepack(0x82, (u8*)sendtmp, 2);
		break;
	case 30:
		sendtmp[0] = 0x2000;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0001;
		sendtmp[4] = SetPower1Start >> 16;
		sendtmp[5] = (u16)SetPower1Start;
		sendtmp[6] = SetPower1End >> 16;
		sendtmp[7] = (u16)SetPower1End;
		sendtmp[8] = SetPower1Start >> 16;
		sendtmp[9] = (u16)SetPower1Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 31:
		sendtmp[0] = 0x2000;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0000;
		sendtmp[4] = SetPower1Start >> 16;
		sendtmp[5] = (u16)SetPower1Start;
		sendtmp[6] = SetPower1End >> 16;
		sendtmp[7] = (u16)SetPower1End;
		sendtmp[8] = SetPower1Start >> 16;
		sendtmp[9] = (u16)SetPower1Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		sendtmp[0] = 0x0024;
		sendtmp[1] = setpowertmp[1];
		makepack(0x82, (u8*)sendtmp, 2);
		break;
	case 32:
		sendtmp[0] = 0x3000;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0001;
		sendtmp[4] = SetPower2Start >> 16;
		sendtmp[5] = (u16)SetPower2Start;
		sendtmp[6] = SetPower0End >> 16;
		sendtmp[7] = (u16)SetPower2End;
		sendtmp[8] = SetPower0Start >> 16;
		sendtmp[9] = (u16)SetPower2Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 33:
		sendtmp[0] = 0x3000;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0000;
		sendtmp[4] = SetPower2Start >> 16;
		sendtmp[5] = (u16)SetPower2Start;
		sendtmp[6] = SetPower2End >> 16;
		sendtmp[7] = (u16)SetPower2End;
		sendtmp[8] = SetPower2Start >> 16;
		sendtmp[9] = (u16)SetPower2Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		sendtmp[0] = 0x0034;
		sendtmp[1] = setpowertmp[2];
		makepack(0x82, (u8*)sendtmp, 2);
		break;
	case 34:
		sendtmp[0] = 0x4000;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0001;
		sendtmp[4] = SetPower3Start >> 16;
		sendtmp[5] = (u16)SetPower3Start;
		sendtmp[6] = SetPower3End >> 16;
		sendtmp[7] = (u16)SetPower3End;
		sendtmp[8] = SetPower3Start >> 16;
		sendtmp[9] = (u16)SetPower3Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		break;
	case 35:
		sendtmp[0] = 0x4000;
		sendtmp[1] = 0x0006;
		sendtmp[2] = 0x0001;
		sendtmp[3] = 0x0000;
		sendtmp[4] = SetPower3Start >> 16;
		sendtmp[5] = (u16)SetPower3Start;
		sendtmp[6] = SetPower3End >> 16;
		sendtmp[7] = (u16)SetPower3End;
		sendtmp[8] = SetPower3Start >> 16;
		sendtmp[9] = (u16)SetPower3Start;
		sendtmp[10] = 0xFF00;
		makepack(0x82, (u8*)sendtmp, 11);
		sendtmp[0] = 0x0044;
		sendtmp[1] = setpowertmp[3];
		makepack(0x82, (u8*)sendtmp, 2);
		break;
	}
}
