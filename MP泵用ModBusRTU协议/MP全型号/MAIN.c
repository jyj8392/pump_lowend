

/*************************************************************/
//通信协议 ModBus
//超时40ms	0xAF00

//本程序适用芯片为：STC89C58RD+

// 设定压力存储地址	0x8000
// 设定流量存储地址	0x8200~0x8201
// 流量校准参数		0x8600~0x8613
// 流量参数出厂值		Costep[10]
// 压力参数出厂值		P_H
// 
// 本机地址			0x8400
// 本机序列号		SN[]
// 本机型号			MODEL[]
// 软件版本号		VERSION[]
// 泵最大压力		MAX_PRESSURE
// 泵最大流量		MAX_FLOW


//编写时间　2019-01-10 
/*************************************************************/

#include<main.h>
#include <stdio.h>
#include <string.h>

#include "lcd.h"
#include "STC_ISP_RW.h"
#include "pump_AtoB.h"


typedef struct _t_modbusRTU {
	uchar address;
	uchar funcode;
	uchar value[50];
	uchar valuelen;
	uchar crc[2];
} t_modbusRTU;

typedef struct _equippara {
	uchar address;
	uchar serialnumber[6];
	uchar model[8];
	uchar version[6];
	
	uint maxpressure;
	uint maxflow;
	uchar pumpstate;
} equippara;

uchar isrecvFrame = 0;
t_modbusRTU xdata recvFrame;
t_modbusRTU xdata sendFrame;
volatile uchar Com_Buffer[50];
uchar SendBuff[50];									 
volatile uchar recvLen = 0;
uchar sendLen = 0;
uint CRC16(uchar *puchMsgg, uchar usDataLen);
void SendError(uchar no, uchar funcode);

equippara equip_para;
uint modbus_overtime_ticks = 0; 
volatile uint pressure_loop_times = 0; 


uint  set_pressure; //setting pressure
uint   set_flow;	//setting flow
uint   VAL_pnow = 0;
uchar PcFlag;  //上位机发送标志位

bit RevFlag=0; //下位机接受成功标志位
//bit RunFlag=0; //泵运行标志为

void Senddata(uchar *p, uchar len);						  //串口发送协议码

uchar  KEY_SCAN(void);
uchar  KEY_SCAN_ONE(void);
uchar  TLC549(void);
void   DELAY_NS(uchar n);       //延时
         
void   Disp_contantflow(void);  //恒流泵操作界面显示
uchar  setting_pressure(void);  //设定压力
uint   setting_flow(void);      //设定流量
uint   pnow(void);               //测量压力
void  clear(uchar com,uchar time);//清除显示
void Responsion(uchar funcode, uchar *value);    //应答

void Menu_Modify(void);
void Menu_Reset(void);

uchar code curr_p[8]={"当前压力"};
uchar code set_p[8]={"设定压力"};
uchar code set_f[8]={"设定流量"};
uchar code ERROR[5]={"ERROR"};
//------------------------------
#define P_H		1410		  //压力参数调整

uchar code SN[] = "012345";
uchar code MODEL[] = "MP1006C  ";
uchar code VERSION[] = "V1.00 ";
#define MAX_PRESSURE	6		//6.0MPa
#define MAX_FLOW		100		//100ml/min

#if  MAX_FLOW == 200
uint code Costep[20]={1293,646,431,324,259,216,185,162,144,129,118,108,100,93,86,
                      81,76,72,68,65,       };		  //流量参数调整
#else
uint code Costep[MAX_FLOW]={12910,6455,4303,3228,2582,2152,1844,1614,1434,1291,		//10
1173,1075,992,921,860,806,759,717,679,645,		//20
614,586,561,537,516,496,478,461,445,430,		//30
417,404,391,380,369,359,349,340,331,323,		//40
315,307,300,293,287,280,274,269,263,258,	  //50
253,248,243,239,234,230,226,222,218,215,		//60
211,208,204,201,198,195,192,189,187,184,		//70
181,179,176,174,172,169,167,165,163,161,		//80
159,157,155,153,151,150,148,146,145,143,		//90
142,140,139,137,136,134,133,132,130,129,		//99
				     };		  //流量参数调整

#endif
//------------------------------
void main(void)
{
	uchar i,key;
	uchar p_limit=0;
	unsigned long Val_psum = 0;
	uint Val_pCnts = 0;
	uint Pressure_loop = 0;
	uint Show_loop = 0;
	uint Protect_loop = 0;

	equip_para.address = 0x01;
	sprintf(equip_para.serialnumber, "%s", SN);
	sprintf(equip_para.model, "%s", MODEL);
	sprintf(equip_para.version, "%s", VERSION);
	
	equip_para.maxpressure = MAX_PRESSURE * 10;
	if (MAX_FLOW == 100) equip_para.maxflow = 999;
	else if (MAX_FLOW == 200) equip_para.maxflow = 200;
	else equip_para.maxflow = MAX_FLOW * 10;
	equip_para.pumpstate = 0;
//-------------------
	AUXR=0x00;
//-------------------
	// TMOD |= 0x01;
	TL0 = 0x00;
	TH0 = 0xCF;
	TR0 = 1;
	ET0 = 1;
//-------------------
	TMOD=0x21;				   //定时器1用作波特率发生器，定时器0方式1用作ModBus超时定时
	TL1=0xfd;				   //Rate：9600
	TH1=0xfd;
	SCON=0xd8;PCON=0x00;	   //0x50:模式1	0xd8:模式3
	EA=1;					   //总中断开启
	TR1=1;					   //定时器1中断开启
	ES=1;					   //串行中断
//------------------------------------------ 	 
	memset(&recvFrame, 0, sizeof(recvFrame));
	memset(&sendFrame, 0, sizeof(sendFrame));

//--------------------------------------------
	P2_7=0;
	LCD_ini();   //液晶初始化

	Disp_contantflow();  //恒流泵操作界面显示
//--------------------------------------------
  do
  {	  
		if (isrecvFrame == 1) {
			isrecvFrame = 0;
			// Senddata(Com_Buffer, recvLen);
			if (recvLen > 0 && equip_para.address == Com_Buffer[0]) {
				if (CRC16(Com_Buffer, recvLen - 2) == (Com_Buffer[recvLen - 2] << 8 | Com_Buffer[recvLen - 1])) {
					// P2_7 = 1;
					recvFrame.address = Com_Buffer[0];
					recvFrame.funcode = Com_Buffer[1];
					switch (recvFrame.funcode) {
						case 0x2B:
							recvFrame.valuelen = 3;
							break;
						case 0x06:
							recvFrame.valuelen = 4;
							break;
						case 0x03:
							recvFrame.valuelen = 4;
							break;
						case 0x05:
							recvFrame.valuelen = 4;
							break;
						case 0x01:
							recvFrame.valuelen = 4;
							break;
						default:
							SendError(0x01, recvFrame.funcode);
							continue;
					}
					memcpy(recvFrame.value, &Com_Buffer[2], recvFrame.valuelen);
					// Senddata(Com_Buffer, recvFrame.valuelen);
					Responsion(recvFrame.funcode, recvFrame.value);
				} else {
//					Senddata("1234", 4);
					// P2_7 = 1;
				}
			}
			recvLen = 0;			
		}
	  
	  
		Show_loop++;
		if (Show_loop >= 500) {	//间隔刷新显示
			Show_loop = 0;
			//"当前压力"
			LCD_WRI(0x80);
			for(i=0;i<8;i++)
			{	     
			 LCD_WRD(curr_p[i]);
			}
	//"设定压力00.0 MPa"
			LCD_WRI(0x90);
			for(i=0;i<8;i++)
			{
			 LCD_WRD(set_p[i]);
			}
		  
	//"设定流量00.0ml/m"
			LCD_WRI(0x88);
			for(i=0;i<8;i++)
			{
			 LCD_WRD(set_f[i]);
			}
		 
	//------------------------------------	 
	//--------------------------------------
	// 显示 "00.00MPa"
			LCD_WRI(0x84);
			LCD_WRD(0x30+VAL_pnow/1000);
			LCD_WRD(0x30+(VAL_pnow %1000)/100);
			LCD_WRD(0x2E);
			LCD_WRD(0x30+VAL_pnow %100/10);
			LCD_WRD(0x30+VAL_pnow %10);
			LCD_WRD(0x4D);
			LCD_WRD(0x50);
			LCD_WRD(0x61);
	//----------------------------------------
			LCD_WRI(0x94);
			LCD_WRD(0x30+set_pressure/100);
			LCD_WRD(0x30+set_pressure%100/10);
			LCD_WRD(0x2E);
			LCD_WRD(0x30+set_pressure%10);
			LCD_WRI(0x96);
			LCD_WRD(0x20);
			LCD_WRD(0x4D);
			LCD_WRD(0x50);
			LCD_WRD(0x61);
	//-----------------------------------------
			LCD_WRI(0x8c);
			// LCD_WRD(0x30+set_flow/1000);
			LCD_WRD(0x30+set_flow%1000/100);
			LCD_WRD(0x30+set_flow%100/10);
			if (MAX_FLOW != 200) LCD_WRD(0x2E);
			LCD_WRD(0x30+set_flow%10);
			//  LCD_WRI(0x8E);
			LCD_WRD(0x6D);
			LCD_WRD(0x6C);
			LCD_WRD(0x2F);
			LCD_WRD(0x6D);
		}

//---------------------------------------
//----------------------------------------
		key=KEY_SCAN_ONE();
		switch(key)
		{
			case 10:
				Menu_Modify();
				break;
			case 11:
				Menu_Reset();
				break;
			case 13:	    
				set_pressure=setting_pressure();
				break;
			case 12:	    
				set_flow=setting_flow();
				break;
			case 14:
				Pump_stop();
				equip_para.pumpstate = 0;
				break;
			case 15: 
				Pump_run();
				equip_para.pumpstate = 1;
				break;
		}
//------------------------------------------
//----------------------------------
//-----------------------------------
//计算当前压力
		Pressure_loop++;		
		if (Pressure_loop >= 100) {
			Pressure_loop = 0;
			Val_psum += pnow();	
			Val_pCnts++;
			if (Val_pCnts >= 50) {
				VAL_pnow = Val_psum/Val_pCnts;
				Val_psum = 0;
				Val_pCnts = 0;
			}
		}
		
 //--------------------------------------------
//压力保护
		if(equip_para.pumpstate == 1) {
			if (p_limit == 1) Protect_loop++;
			if(p_limit == 0 && VAL_pnow/10>set_pressure) {
				Pump_stop();
				p_limit=1;
			} else if(p_limit==1 && Protect_loop >= 10000) {
				Pump_run();
				p_limit=0;
				Protect_loop = 0;
			} 
		}
//		for(i=100;i<0;i--); 
	}while(1);

}

//******************************************************************/
//测量当前压力
uint pnow(void)
  {
   uchar AD_val;
   unsigned long  Val_ptem;
   uint Val_ptemint;
 
   AD_val=TLC549();
//-------------------------
//软件校零
   if(AD_val>3)
   AD_val=AD_val-3;	  
   else
   AD_val=0;
//-------------------------
   Val_ptem=AD_val;
   Val_ptem*=P_H;	 //比例系数
   Val_ptemint=Val_ptem/255;
//--------------------------------
   return(Val_ptemint);
  }	 
/*****************************************************************/
/******压力设定函数********************/
uchar  setting_pressure(void)
   {
    uchar i;
    uchar ptime,val_setp;
	uint p_val;
	ptime=0;
	p_val=0;
//--------------------
//   astring[2]=0; //使PC指令无效
//--------------------
//显示"Pset"
    LCD_WRI(0x98);
	LCD_WRD(0x50);
	LCD_WRD(0x73);
	LCD_WRD(0x65);
	LCD_WRD(0x74);
//--------------------
    LCD_WRI(0x94);
	LCD_WRD(0x20);
	LCD_WRD(0x20);
	LCD_WRD(0x20);
	LCD_WRD(0x20);
	LCD_WRI(0x94);
//---------------------
   while(ptime<=2)
       {
    	    val_setp=KEY_SCAN();
	        if(val_setp<10)
			{
			  LCD_WRD(0x30+val_setp);
			 switch (ptime)
			   {
			    case 0:
				p_val=p_val+val_setp*100;
				break;
				case 1:
				p_val=p_val+val_setp*10;
				LCD_WRD(0x2E);
				break;
				case 2:
				p_val=p_val+val_setp*1;
				break;
			   }
			  ptime++;
			}
	    }
//----------------------------
	if(p_val<=equip_para.maxpressure)
	{ set_pressure=p_val;
	 EEPROM_sector_erase(0x8000);
	 EEPROM_byte_program(0x8000,set_pressure >> 8); //记忆设定值
	 EEPROM_byte_program(0x8001,set_pressure & 0x00FF); //记忆设定值
	  clear(0x98,5);
//---------------------------
/*
     Com_Buffer[0]='0';
	 Com_Buffer[1]='0';
	 Com_Buffer[2]='P';
	 Com_Buffer[3]=0x30+set_pressure/100;
	 Com_Buffer[4]=0x30+set_pressure%100/10;
	 Com_Buffer[5]='.';
	 Com_Buffer[6]=0x30+set_pressure%10;
	 Senddata_pc(Com_Buffer);
*/	 
//---------------------------
	 }
	 else 
	    { LCD_WRI(0x98);
	     for(i=0;i<=4;i++)
		 LCD_WRD(ERROR[i]);
		 }
//------------------------------
    return(set_pressure);  
   }
/***************************************************************/
/**************设定流量函数******************/
uint   setting_flow(void)
 {
    uchar i;
    uchar ptime,val_setp;
	uint p_val;
	ptime=0;
	p_val=0;
//--------------------
//   astring[2]=0; //使PC指令无效
//--------------------
//显示"Lset"
    LCD_WRI(0x98);
	LCD_WRD(0x4C);
	LCD_WRD(0x73);
	LCD_WRD(0x65);
	LCD_WRD(0x74);
//--------------------
//--------------------
    LCD_WRI(0x8C);
	LCD_WRD(0x20);
	LCD_WRD(0x20);
	LCD_WRD(0x20);
	LCD_WRD(0x20);
	LCD_WRI(0x8C);
//---------------------
   while(ptime<=2)
       {
    	val_setp=KEY_SCAN();
	      if(val_setp<10)
			{
			  LCD_WRD(0x30+val_setp);
			  switch (ptime)
			   {
			    case 0:
				p_val=p_val+val_setp*100;
				break;
				case 1:
				p_val=p_val+val_setp*10;
				if (MAX_FLOW != 200) LCD_WRD(0x2E);
				break;
				case 2:
				p_val=p_val+val_setp*1;
				break;
			//	case 3:
			//	p_val=p_val+val_setp*1;
			//	break;
			   }
			  ptime++;
			}
	    }
//----------------------------
	if(p_val<=equip_para.maxflow)
	{ 
	 set_flow=p_val;	 
	 EEPROM_sector_erase(0x8200);
	 EEPROM_byte_program(0x8200,set_flow>>8); //记忆设定值
	 EEPROM_byte_program(0x8201,set_flow&0x00FF); //记忆设定值

     Pump_flow(set_flow);	
	 clear(0x98,5);
//----------------------------------------
	 }
	 else 
	    { LCD_WRI(0x98);
	     for(i=0;i<=4;i++)
		 LCD_WRD(ERROR[i]);
		 }
//------------------------------
    return(set_flow);  
 }
//****************************************************************/
void clear( uchar com,uchar time)
  {
  uchar i;
  LCD_WRI(com);
  for(i=0;i<=time;i++)
  LCD_WRD(0x20);
  }
//*********************************************************************
// 恒流泵操作界面显示 子函数
void   Disp_contantflow(void)
{
	uchar i;
   //------------------------------------
 //"当前压力"
	LCD_WRI(0x80);
	for(i=0;i<8;i++) {	     
		LCD_WRD(curr_p[i]);
	}
//"设定压力00.0 MPa"
	LCD_WRI(0x90);
	for(i=0;i<8;i++) {
		LCD_WRD(set_p[i]);
	}
	LCD_WRI(0x96);
	LCD_WRD(0x20);
	LCD_WRD(0x4D);
	LCD_WRD(0x50);
	LCD_WRD(0x61);
//"设定流量00.0ml/m"
	LCD_WRI(0x88);
	for(i=0;i<8;i++) {
		LCD_WRD(set_f[i]);
	}
	LCD_WRI(0x8E);
	LCD_WRD(0x6D);
	LCD_WRD(0x6C);
	LCD_WRD(0x2F);
	LCD_WRD(0x6D);
//------------------------------------
//"00.0" 显示压力
	LCD_WRI(0x94);
	set_pressure=EEPROM_byte_read(0x8000) << 8;
	set_pressure|=EEPROM_byte_read(0x8001);
	if (set_pressure == 0 || set_pressure > equip_para.maxpressure) {
		set_pressure = equip_para.maxpressure;
		EEPROM_sector_erase(0x8000);
		EEPROM_byte_program(0x8000,set_pressure >> 8); //记忆设定值
		EEPROM_byte_program(0x8001,set_pressure & 0x00FF); //记忆设定值
	}
	LCD_WRD(0x30+set_pressure/100);	   
	LCD_WRD(0x30+(set_pressure %100)/10);
	LCD_WRD(0x2E);
	LCD_WRD(0x30+set_pressure %10);
//------------------------------------
//"000"显示流量
	LCD_WRI(0x8C);
	set_flow=EEPROM_byte_read(0x8200)<<8;
	set_flow|=EEPROM_byte_read(0x8201);
	if (set_flow == 0 || set_flow > equip_para.maxflow) {
		set_flow = equip_para.maxflow;
		EEPROM_sector_erase(0x8200);
		EEPROM_byte_program(0x8200,set_flow>>8); //记忆设定值
		EEPROM_byte_program(0x8201,set_flow&0x00FF); //记忆设定值
	}
//  LCD_WRD(0x30+set_flow/1000);
	LCD_WRD(0x30+set_flow%1000/100);
	LCD_WRD(0x30+(set_flow %100)/10);
	if (MAX_FLOW != 200) LCD_WRD(0x2E);
	LCD_WRD(0x30+set_flow %10);
//------------------------------------
//显示"STOP"
	LCD_WRI(0x9E);
	LCD_WRD(0x53);
	LCD_WRD(0x54);
	LCD_WRD(0x4F);
	LCD_WRD(0x50);
//------------------------------------ 
	equip_para.address = EEPROM_byte_read(0x8400);
	if (equip_para.address == 0x00 || equip_para.address > 32) {
		equip_para.address = 0x01;
		EEPROM_sector_erase(0x8400);
		EEPROM_byte_program(0x8400, equip_para.address); //记忆本机地址
	}
//------------------------------------ 
	DELAY_NS(1);  
	Pump_flow(set_flow);
}
//******************************************************************
//流量调整界面
 void Menu_Modify(void)
  {
    uchar i,key,ptime=0,segment=0;
	uint Mfset=0,Mfmea=0,Mdata;
	uint EEPROM_dir=0x8600,EEPROM_data;
	uint Flow_tem[10];
	unsigned long multi;

	uchar code Disp_Mtitle[8]={"调整流量"};
	uchar code Disp_Mfset[8]={"设定流量"};
	uchar code Disp_Mfmea[8]={"测定流量"};
	uchar code Disp_write[10]={"调整值写入"};
	uchar code Disp_cancel[14]={"调整值写入取消"};
	uchar code Disp_cuowu[4]={"错误"};
	uchar code Disp_promt1[12]={"调整值必须为"};
	uchar code Disp_promt2[10]={"10的整  倍"};
	//,0xca,0xfd,

  	EA=0;

  	LCD_WRI(0x01);
	for(i=255;i<0;i--);
//-------------------------
 if(set_flow%10!=0)
   {
     LCD_WRI(0x80);
	  for(i=0;i<4;i++)
	  {	     
		LCD_WRD(Disp_cuowu[i]);
	   }
	 LCD_WRI(0x90);
	  for(i=0;i<12;i++)
	  {	     
		LCD_WRD(Disp_promt1[i]);
	   }
	  LCD_WRI(0x88);
	  for(i=0;i<10;i++)
	  {	     
		LCD_WRD(Disp_promt2[i]);
	   }
	  LCD_WRI(0x8b);
	  LCD_WRD(0xca);
	  LCD_WRD(0xfd);
	  DELAY_NS(2);
//-----------------------------
     LCD_WRI(0x01);
	 for(i=255;i<0;i--);  
	 Disp_contantflow();

	 EA=1;
//------------------------------  
        return;
   }

//------------------------------
	 LCD_WRI(0x82);
	 for(i=0;i<8;i++)
	 {	     
		LCD_WRD(Disp_Mtitle[i]);
	 }
	 LCD_WRI(0x90);
	 for(i=0;i<8;i++)
	 {	     
		LCD_WRD(Disp_Mfset[i]);
	 }
	  LCD_WRI(0x88);
	 for(i=0;i<8;i++)
	 {	     
		LCD_WRD(Disp_Mfmea[i]);
	 }

//----------------------------------
      LCD_WRI(0x95);
	  LCD_WRD(0x30+set_flow/100);
      LCD_WRD(0x30+set_flow%100/10);
	  if (MAX_FLOW != 200) LCD_WRD(0x2E);
	  LCD_WRD(0x30+set_flow%10);

	   LCD_WRI(0x8d);
	  while(ptime<=3)
       {
    	key=KEY_SCAN();
		       if(key==14)
			     {
				  goto FModifyEnd;
				 }
	      if(key<10)
			{
			  LCD_WRD(0x30+key);
			  switch (ptime)
			   {
			    case 0:
				Mfmea=key*1000;
				break;
				case 1:
			    Mfmea=Mfmea+key*100;
			    LCD_WRD(0x2E);
				break;
				case 2:
				Mfmea=Mfmea+key*10;
				
				break;
				case 3:
				Mfmea=Mfmea+key;
				break;
			   }
			   	
			  ptime++;
			}
		  if(key==12)
		   {
		    LCD_WRI(0x8d);
			LCD_WRD(0x20); 
		    LCD_WRD(0x20);
			LCD_WRD(0x20);
			LCD_WRD(0x20);
			LCD_WRD(0x20);
			LCD_WRD(0x20);
			LCD_WRI(0x8d);
			ptime=0;
			Mfmea=0;
		   }
	    }
//----------------------------------
 //从EEPROM里读出参数值存储在step数组里
#if MAX_FLOW == 200
	for (i=0;i<20;i++)
#else
	for (i=0;i<MAX_FLOW;i++)
#endif
	 {
	// EEPROM_dir=EEPROM_dir+i*2;
	 EEPROM_dir=0x8600+i*2;
     EEPROM_data=EEPROM_byte_read(EEPROM_dir)<<8;
	 EEPROM_data=EEPROM_data+EEPROM_byte_read(EEPROM_dir+1);
	 Flow_tem[i]=EEPROM_data;
	 } 
//----------------------------------
     if(set_flow>=1)
     segment=(set_flow-1)/10;
     else
     segment=0;

     Mfset=set_flow*10;


     multi=Flow_tem[segment];
     multi=multi*Mfmea;
	 Mdata=multi/Mfset;
   	Flow_tem[segment]=Mdata;

      LCD_WRI(0x98);
	  LCD_WRD(0x30+Mdata/10000);
	  LCD_WRD(0x30+Mdata%10000/1000);
      LCD_WRD(0x30+Mdata%1000/100);
	  LCD_WRD(0x30+Mdata%100/10);
	  LCD_WRD(0x30+Mdata%10);
/*
	  LCD_WRD(0x30+Mfmea/1000);
      LCD_WRD(0x30+Mfmea%1000/100);
	  LCD_WRD(0x30+Mfmea%100/10);
	  LCD_WRD(0x30+Mfmea%10);

	  LCD_WRD(0x30+Mfset/1000);
      LCD_WRD(0x30+Mfset%1000/100);
	  LCD_WRD(0x30+Mfset%100/10);
	  LCD_WRD(0x30+Mfset%10);
  */
   do
   {
      key=KEY_SCAN();
	  if(key==15)
	  {
//----------------------------------
//参数值写进EEPROM
     EEPROM_sector_erase(0x8600);
#if MAX_FLOW == 200
	for (i=0;i<20;i++)
#else
	for (i=0;i<MAX_FLOW;i++)
#endif	 
	{
	 EEPROM_dir=0x8600+i*2;
	 EEPROM_data=Flow_tem[i];
	 EEPROM_byte_program(EEPROM_dir,EEPROM_data>>8); //记忆设定值
	 EEPROM_byte_program(EEPROM_dir+1,EEPROM_data&0x00FF); //记忆设定值

	 }
//------------------------------------
        LCD_WRI(0x98);
	    for(i=0;i<10;i++)
	    {	     
		LCD_WRD(Disp_write[i]);
	     }
		 DELAY_NS(2);
         key=20;
//----------------------------------
	  }

//-------------------------------------
     if(key==14)
	 {
	    LCD_WRI(0x98);
	    for(i=0;i<14;i++)
	    {	     
		LCD_WRD(Disp_cancel[i]);
	     }
		 DELAY_NS(5);
         key=20; 
	 }
//-------------------------------------
   }while(key!=20);    
//----------------------------------
FModifyEnd:

     LCD_WRI(0x01);
	 for(i=255;i<0;i--);  
	 Disp_contantflow();

	 EA=1;
  }
/**************************************************************************/
void Menu_Reset(void)
  {
     uchar i,key;
     uint EEPROM_dir=0x8600,EEPROM_data;
	 uchar code Reset_title[10]={"出厂值恢复"};
	 uchar code Reset_ok[6]={"确定R "};
	 uchar code Reset_cancel[6]={"取消S "};
	 uchar code Disp_resetok[14]={"出厂值恢复成功"};
	 uchar code Disp_resetnot[14]={"出厂值恢复取消"};
//------------------------------------------
        	LCD_WRI(0x01);
	       for(i=255;i<0;i--);
//-----------------------------------------
	    LCD_WRI(0x92);
	    for(i=0;i<10;i++)
	    {	     
		LCD_WRD(Reset_title[i]);
	     }
        LCD_WRI(0x98);
	    for(i=0;i<6;i++)
	    {	     
		LCD_WRD(Reset_ok[i]);
	     }
		LCD_WRI(0x9d);
	    for(i=0;i<6;i++)
	    {	     
		LCD_WRD(Reset_cancel[i]);
	     }
   do
   {
	   key=KEY_SCAN();
	   if(key==15)
	    {
//参数值写进EEPROM
              EEPROM_sector_erase(0x8600);
			#if MAX_FLOW == 200
	              for (i=0;i<20;i++)
			#else
					for (i=0;i<MAX_FLOW;i++)
			#endif
	             {
	               EEPROM_dir=0x8600+i*2;
	               EEPROM_data=Costep[i];
	               EEPROM_byte_program(EEPROM_dir,EEPROM_data>>8); //记忆设定值
	               EEPROM_byte_program(EEPROM_dir+1,EEPROM_data&0x00FF); //记忆设定值
	              } 
           LCD_WRI(0x01);
	       for(i=255;i<0;i--);
		   LCD_WRI(0x90);
	       for(i=0;i<14;i++)
	       {	     
		    LCD_WRD(Disp_resetok[i]);
	       }
		   DELAY_NS(5);
		    key=21;
		}

		if(key==14)
		{
		   LCD_WRI(0x01);
	       for(i=255;i<0;i--);
		   LCD_WRI(0x90);
	       for(i=0;i<14;i++)
	       {	     
		    LCD_WRD(Disp_resetnot[i]);
	       }
		   DELAY_NS(5);
		 key=21;
		}

	}while(key!=21);

	 LCD_WRI(0x01);
	 for(i=255;i<0;i--);  
	 Disp_contantflow();

  }
//******************************************************************
void Responsion(uchar funcode, uchar *value)
{
	uint crc = 0;
	uchar i = 0;

	uint reg_address = 0;
	uint reg_value = 0;
	
	switch (funcode) {
		case 0x2B:
			//check legality
			if (value[0] != 0x0E || value[1] != 0x01) return;
			if (value[2] != 0x00) {SendError(0x02, funcode); return;}
			//make response
			sendFrame.address = equip_para.address;
			sendFrame.funcode = funcode;
			sendFrame.valuelen = 32;
			sendFrame.value[i++] = 0x0E;	sendFrame.value[i++] = 0x01;	sendFrame.value[i++] = 0x01;	sendFrame.value[i++] = 0x00;	sendFrame.value[i++] = 0x00;	sendFrame.value[i++] = 0x03;
			sendFrame.value[i++] = 0x00;	sendFrame.value[i++] = 0x06;	memcpy(sendFrame.value + i, equip_para.serialnumber, sizeof(equip_para.serialnumber)); i += sizeof(equip_para.serialnumber);
			sendFrame.value[i++] = 0x01;	sendFrame.value[i++] = 0x08;	memcpy(sendFrame.value + i, equip_para.model, sizeof(equip_para.model)); i += sizeof(equip_para.model);
			sendFrame.value[i++] = 0x02;	sendFrame.value[i++] = 0x06;	memcpy(sendFrame.value + i, equip_para.version, sizeof(equip_para.version)); i += sizeof(equip_para.version);
			memcpy(SendBuff, &sendFrame, sendFrame.valuelen + 2); sendLen = sendFrame.valuelen + 2;
			crc = CRC16(SendBuff, sendLen);
			memcpy(SendBuff + sendLen, &crc, sizeof(crc)); sendLen += 2;
			Senddata(SendBuff, sendLen);
			//do something
			break;
		case 0x06:		
			reg_address = value[0] << 8 | value[1];
			reg_value = value[2] << 8 | value[3];
			if (reg_address == 0x0001) {
				if (reg_value > 0 && reg_value <= 32) {
					if (equip_para.address != reg_value) {
						//do something
						EEPROM_sector_erase(0x8400);
						EEPROM_byte_program(0x8400, (uchar)reg_value); //记忆本机地址	
						//make response
						sendFrame.address = equip_para.address;
						sendFrame.funcode = funcode;
						sendFrame.valuelen = 4;
						sendFrame.value[i++] = 0x00;	sendFrame.value[i++] = 0x01;
						sendFrame.value[i++] = 0x00;	sendFrame.value[i++] = (uchar)reg_value;
						memcpy(SendBuff, &sendFrame, sendFrame.valuelen + 2); sendLen = sendFrame.valuelen + 2;
						crc = CRC16(SendBuff, sendLen);
						memcpy(SendBuff + sendLen, &crc, sizeof(crc)); sendLen += 2;
						Senddata(SendBuff, sendLen);
						//do something
						equip_para.address = (uchar)reg_value;
					}	
				} else {
					SendError(0x03, funcode); return;
				}
			} else if (reg_address == 0x0002) {
				if (reg_value > 0 && reg_value <= equip_para.maxpressure) {
					//do something
					set_pressure = reg_value;
					EEPROM_sector_erase(0x8000);
					EEPROM_byte_program(0x8000,set_pressure >> 8);
					EEPROM_byte_program(0x8001,set_pressure & 0x00FF);
					//make response
					sendFrame.address = equip_para.address;
					sendFrame.funcode = funcode;
					sendFrame.valuelen = 4;
					sendFrame.value[i++] = 0x00;	sendFrame.value[i++] = 0x02;
					sendFrame.value[i++] = set_pressure >> 8;	sendFrame.value[i++] = set_pressure & 0x00ff;
					memcpy(SendBuff, &sendFrame, sendFrame.valuelen + 2); sendLen = sendFrame.valuelen + 2;
					crc = CRC16(SendBuff, sendLen);
					memcpy(SendBuff + sendLen, &crc, sizeof(crc)); sendLen += 2;
					Senddata(SendBuff, sendLen);					
				} else {
					SendError(0x03, funcode); return;
				}
			} else if (reg_address == 0x0003) {
				if (reg_value > 0 && reg_value <= equip_para.maxflow) {
					//do something
					set_flow = reg_value;
					EEPROM_sector_erase(0x8200);
					EEPROM_byte_program(0x8200,set_flow>>8);
					EEPROM_byte_program(0x8201,set_flow&0x00FF);
					Pump_flow(set_flow);
					//make response
					sendFrame.address = equip_para.address;
					sendFrame.funcode = funcode;
					sendFrame.valuelen = 4;
					sendFrame.value[i++] = 0x00;	sendFrame.value[i++] = 0x03;
					sendFrame.value[i++] = set_flow >> 8;	sendFrame.value[i++] = set_flow & 0x00ff;
					memcpy(SendBuff, &sendFrame, sendFrame.valuelen + 2); sendLen = sendFrame.valuelen + 2;
					crc = CRC16(SendBuff, sendLen);
					memcpy(SendBuff + sendLen, &crc, sizeof(crc)); sendLen += 2;
					Senddata(SendBuff, sendLen);					
				} else {
					SendError(0x03, funcode); return;
				}
			} else {
				SendError(0x02, funcode); return;
			}			
			break;
		case 0x03:
			reg_address = value[0] << 8 | value[1];
			reg_value = value[2] << 8 | value[3];
			if (reg_address == 0x0002) {
				if (reg_value == 0x0001) {
					//do something
					//make response
					sendFrame.address = equip_para.address;
					sendFrame.funcode = funcode;
					sendFrame.valuelen = 3;
					sendFrame.value[i++] = 0x02;
					sendFrame.value[i++] = set_pressure >> 8;	sendFrame.value[i++] = set_pressure & 0x00ff;
					memcpy(SendBuff, &sendFrame, sendFrame.valuelen + 2); sendLen = sendFrame.valuelen + 2;
					crc = CRC16(SendBuff, sendLen);
					memcpy(SendBuff + sendLen, &crc, sizeof(crc)); sendLen += 2;
					Senddata(SendBuff, sendLen);					
				} else {
					SendError(0x03, funcode); return;
				}
			} else if (reg_address == 0x0003) {
				if (reg_value == 0x0001) {
					//do something
					//make response
					sendFrame.address = equip_para.address;
					sendFrame.funcode = funcode;
					sendFrame.valuelen = 3;
					sendFrame.value[i++] = 0x02;
					sendFrame.value[i++] = set_flow >> 8;	sendFrame.value[i++] = set_flow & 0x00ff;
					memcpy(SendBuff, &sendFrame, sendFrame.valuelen + 2); sendLen = sendFrame.valuelen + 2;
					crc = CRC16(SendBuff, sendLen);
					memcpy(SendBuff + sendLen, &crc, sizeof(crc)); sendLen += 2;
					Senddata(SendBuff, sendLen);					
				} else {
					SendError(0x03, funcode); return;
				}
			} else if (reg_address == 0x0004) {
				if (reg_value == 0x0001) {
					//do something
					//make response
					sendFrame.address = equip_para.address;
					sendFrame.funcode = funcode;
					sendFrame.valuelen = 3;
					sendFrame.value[i++] = 0x02;
					sendFrame.value[i++] = (VAL_pnow / 10) >> 8;	sendFrame.value[i++] = (VAL_pnow / 10) & 0x00ff;
					memcpy(SendBuff, &sendFrame, sendFrame.valuelen + 2); sendLen = sendFrame.valuelen + 2;
					crc = CRC16(SendBuff, sendLen);
					memcpy(SendBuff + sendLen, &crc, sizeof(crc)); sendLen += 2;
					Senddata(SendBuff, sendLen);					
				} else {
					SendError(0x03, funcode); return;
				}
			} else {
				SendError(0x02, funcode); return;
			}
			break;
		case 0x05:
			reg_address = value[0] << 8 | value[1];
			reg_value = value[2] << 8 | value[3];
			if (reg_address == 0x0001) {
				if (reg_value == 0xff00) {
					//do something
					Pump_run();
					equip_para.pumpstate = 1;
					//make response
					sendFrame.address = equip_para.address;
					sendFrame.funcode = funcode;
					sendFrame.valuelen = 4;
					sendFrame.value[i++] = 0x00;	sendFrame.value[i++] = 0x01;
					sendFrame.value[i++] = 0xFF;	sendFrame.value[i++] = 0x00;
					memcpy(SendBuff, &sendFrame, sendFrame.valuelen + 2); sendLen = sendFrame.valuelen + 2;
					crc = CRC16(SendBuff, sendLen);
					memcpy(SendBuff + sendLen, &crc, sizeof(crc)); sendLen += 2;
					Senddata(SendBuff, sendLen);					
				} else if (reg_value == 0x0000) {
					//do something
					Pump_stop();
					equip_para.pumpstate = 0;
					//make response
					sendFrame.address = equip_para.address;
					sendFrame.funcode = funcode;
					sendFrame.valuelen = 4;
					sendFrame.value[i++] = 0x00;	sendFrame.value[i++] = 0x01;
					sendFrame.value[i++] = 0x00;	sendFrame.value[i++] = 0x00;
					memcpy(SendBuff, &sendFrame, sendFrame.valuelen + 2); sendLen = sendFrame.valuelen + 2;
					crc = CRC16(SendBuff, sendLen);
					memcpy(SendBuff + sendLen, &crc, sizeof(crc)); sendLen += 2;
					Senddata(SendBuff, sendLen);					
				} else {
					SendError(0x03, funcode); return;
				}
			} else {
				SendError(0x02, funcode); return;
			}			
			break;
		case 0x01:
			reg_address = value[0] << 8 | value[1];
			reg_value = value[2] << 8 | value[3];
			if (reg_address == 0x0001) {
				if (reg_value == 0x0001) {
					//do something
					//make response
					sendFrame.address = equip_para.address;
					sendFrame.funcode = funcode;
					sendFrame.valuelen = 2;
					sendFrame.value[i++] = 0x01;	sendFrame.value[i++] = equip_para.pumpstate;
					memcpy(SendBuff, &sendFrame, sendFrame.valuelen + 2); sendLen = sendFrame.valuelen + 2;
					crc = CRC16(SendBuff, sendLen);
					memcpy(SendBuff + sendLen, &crc, sizeof(crc)); sendLen += 2;
					Senddata(SendBuff, sendLen);					
				} else {
					SendError(0x03, funcode); return;
				}
			} else {
				SendError(0x02, funcode); return;
			}			
			break;
	}
	
	 
}
//******************************************************************
void serial(void) interrupt 4						 //串口中断接收
{
	if(RI) {
		RI = 0;
		Com_Buffer[recvLen++]=SBUF;	if (recvLen >= sizeof(Com_Buffer)) recvLen = 0;
		TL0 = 0x00;
		TH0 = 0xCF;
		TR0 = 1;
	}
}

void T0_Handler(void) interrupt 1 using 1				//TIMER0
{
	TR0 = 0;
	isrecvFrame = 1;	
}
/**************************************************************************/
//发送数据	
void Senddata(uchar *p, uchar len)						  //串口发送协议码
{  
	uchar i;

	ES=0;

	for(i=0;i<len;i++)
	{
		ACC = p[i];
		if (P) {
			TB8 = 1;
		} else {
			TB8 = 0;
		}

		SBUF=ACC;
		// SBUF=p[i];
		while(!TI);
		TI=0;
	}

	ES=1;
}

void SendError(uchar no, uchar funcode)
{
	uint crc = 0;
	uchar i = 0;

	if (no == 0x01) {
		//make response
		sendFrame.address = equip_para.address;
		sendFrame.funcode = funcode + 0x80;
		sendFrame.valuelen = 1;
		sendFrame.value[0] = 0x01;
		memcpy(SendBuff, &sendFrame, sendFrame.valuelen + 2); sendLen = sendFrame.valuelen + 2;
		crc = CRC16(SendBuff, sendLen);
		memcpy(SendBuff + sendLen, &crc, sizeof(crc)); sendLen += 2;
		Senddata(SendBuff, sendLen);
    } else if (no == 0x02) {			
		switch (funcode) {
			case 0x2B:
				//make response
				sendFrame.address = equip_para.address;
				sendFrame.funcode = funcode + 0x80;
				sendFrame.valuelen = 2;
				sendFrame.value[0] = 0x0E;	sendFrame.value[1] = 0x02;
				memcpy(SendBuff, &sendFrame, sendFrame.valuelen + 2); sendLen = sendFrame.valuelen + 2;
				crc = CRC16(SendBuff, sendLen);
				memcpy(SendBuff + sendLen, &crc, sizeof(crc)); sendLen += 2;
				Senddata(SendBuff, sendLen);
				break;
			case 0x06:
			case 0x03:
			case 0x05:
			case 0x01:
				//make response
				sendFrame.address = equip_para.address;
				sendFrame.funcode = funcode + 0x80;
				sendFrame.valuelen = 1;
				sendFrame.value[0] = 0x02;
				memcpy(SendBuff, &sendFrame, sendFrame.valuelen + 2); sendLen = sendFrame.valuelen + 2;
				crc = CRC16(SendBuff, sendLen);
				memcpy(SendBuff + sendLen, &crc, sizeof(crc)); sendLen += 2;
				Senddata(SendBuff, sendLen);				
				break;
			default:
				break;
		}
	} else if (no == 0x03) {
		switch (funcode) {
			case 0x06:
			case 0x03:
			case 0x05:
			case 0x01:
				//make response
				sendFrame.address = equip_para.address;
				sendFrame.funcode = funcode + 0x80;
				sendFrame.valuelen = 1;
				sendFrame.value[0] = 0x03;
				memcpy(SendBuff, &sendFrame, sendFrame.valuelen + 2); sendLen = sendFrame.valuelen + 2;
				crc = CRC16(SendBuff, sendLen);
				memcpy(SendBuff + sendLen, &crc, sizeof(crc)); sendLen += 2;
				Senddata(SendBuff, sendLen);				
				break;
		}
		
	}
}

/****************************************************************************/

//CRC???????
/* CRC ??????*/
    static uchar auchCRCHi[] = {
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
    0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,
    0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,
    0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,
    0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
    0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
    0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,
    0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
    0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,
    0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
    0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,
    0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,
    0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
    0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,
    0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
    0x80,0x41,0x00,0xC1,0x81,0x40
    } ;
    /* CRC??????*/
    static uchar auchCRCLo[] = {
    0x00,0xC0,0xC1,0x01,0xC3,0x03,0x02,0xC2,0xC6,0x06,
    0x07,0xC7,0x05,0xC5,0xC4,0x04,0xCC,0x0C,0x0D,0xCD,
    0x0F,0xCF,0xCE,0x0E,0x0A,0xCA,0xCB,0x0B,0xC9,0x09,
    0x08,0xC8,0xD8,0x18,0x19,0xD9,0x1B,0xDB,0xDA,0x1A,
    0x1E,0xDE,0xDF,0x1F,0xDD,0x1D,0x1C,0xDC,0x14,0xD4,
    0xD5,0x15,0xD7,0x17,0x16,0xD6,0xD2,0x12,0x13,0xD3,
    0x11,0xD1,0xD0,0x10,0xF0,0x30,0x31,0xF1,0x33,0xF3,
    0xF2,0x32,0x36,0xF6,0xF7,0x37,0xF5,0x35,0x34,0xF4,
    0x3C,0xFC,0xFD,0x3D,0xFF,0x3F,0x3E,0xFE,0xFA,0x3A,
    0x3B,0xFB,0x39,0xF9,0xF8,0x38,0x28,0xE8,0xE9,0x29,
    0xEB,0x2B,0x2A,0xEA,0xEE,0x2E,0x2F,0xEF,0x2D,0xED,
    0xEC,0x2C,0xE4,0x24,0x25,0xE5,0x27,0xE7,0xE6,0x26,
    0x22,0xE2,0xE3,0x23,0xE1,0x21,0x20,0xE0,0xA0,0x60,
    0x61,0xA1,0x63,0xA3,0xA2,0x62,0x66,0xA6,0xA7,0x67,
    0xA5,0x65,0x64,0xA4,0x6C,0xAC,0xAD,0x6D,0xAF,0x6F,
    0x6E,0xAE,0xAA,0x6A,0x6B,0xAB,0x69,0xA9,0xA8,0x68,
    0x78,0xB8,0xB9,0x79,0xBB,0x7B,0x7A,0xBA,0xBE,0x7E,
    0x7F,0xBF,0x7D,0xBD,0xBC,0x7C,0xB4,0x74,0x75,0xB5,
    0x77,0xB7,0xB6,0x76,0x72,0xB2,0xB3,0x73,0xB1,0x71,
    0x70,0xB0,0x50,0x90,0x91,0x51,0x93,0x53,0x52,0x92,
    0x96,0x56,0x57,0x97,0x55,0x95,0x94,0x54,0x9C,0x5C,
    0x5D,0x9D,0x5F,0x9F,0x9E,0x5E,0x5A,0x9A,0x9B,0x5B,
    0x99,0x59,0x58,0x98,0x88,0x48,0x49,0x89,0x4B,0x8B,
    0x8A,0x4A,0x4E,0x8E,0x8F,0x4F,0x8D,0x4D,0x4C,0x8C,
    0x44,0x84,0x85,0x45,0x87,0x47,0x46,0x86,0x82,0x42,
    0x43,0x83,0x41,0x81,0x80,0x40
    } ;
	
uint CRC16(uchar *puchMsgg, uchar usDataLen)
{
    uchar uchCRCHi = 0xFF;
    uchar uchCRCLo = 0xFF;
    uchar uIndex;
    while (usDataLen--)
    {
		uIndex = uchCRCHi ^ *puchMsgg++;
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
    }
    return ((uchCRCHi << 8) | uchCRCLo);
}