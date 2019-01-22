
#include<main.h>
#include "lcd.h"
#include "STC_ISP_RW.h"
#include "pump_AtoB.h"

void Send_Data_B( uchar Flag,uchar Data_H,uchar Data_L);
void   OUTPUT(uchar Send_D);   //向第二块单片机发送数据
/***************************************************************/
//泵启动
void  Pump_run(void)
  {
  Send_Data_B(0x52,0x00,0x00);
  //显示" RUN"
       LCD_WRI(0x9E);
	   LCD_WRD(0x20);
	   LCD_WRD(0x52);
	   LCD_WRD(0x55);
	   LCD_WRD(0x4E);

  }	
/***************************************************************/ 		   
 //泵停止
void  Pump_stop(void)
  {
   Send_Data_B(0x53,0x00,0x00);
   //显示"STOP"
       LCD_WRI(0x9E);
	   LCD_WRD(0x53);
	   LCD_WRD(0x54);
	   LCD_WRD(0x4F);
	   LCD_WRD(0x50);
  }	
 /*************************************************************/
 //泵流量		  
void  Pump_flow( uint p_flow) 
  {
   uchar Data_H,Data_L,segment;
   uint EEPROM_dir=0x8600,EEPROM_data;
   uint p_step;
   unsigned long P_plus;
//-------------------------------------------
   if(p_flow>=1)
   segment=(p_flow-1)/10;
   else
   segment=0;
//-------------------------------------------
//从EEPROM里读出参数值	 
	 EEPROM_dir=EEPROM_dir+segment*2;
     EEPROM_data=EEPROM_byte_read(EEPROM_dir)<<8;
	 EEPROM_data=EEPROM_data+EEPROM_byte_read(EEPROM_dir+1);
	 
//-------------------------------------------
  P_plus=EEPROM_data*(segment+1); 
  P_plus=P_plus*10;

  P_plus=P_plus/p_flow;
     if(P_plus<=65535)
	   {
	    p_step=P_plus; 
	   }
	  else
	   {
	    p_step=65535; 
	   }
//-------------------------------------------	     
  Data_H=p_step>>8;
  Data_L=p_step & 0x00FF;
  Send_Data_B(0x46,Data_H,Data_L);
  }
/************************************************************/
void Send_Data_B( uchar Flag,uchar Data_H,uchar Data_L)
  {
   uchar i;
//---------------
//中断
   P2_2=0;
   P2_2=0;
   P2_2=0;
   P2_2=1;
//----------------
//;发送去错数字 1
   OUTPUT(0x00);
   for(i=100;i<=0;i--);
//-----------------
   OUTPUT(Flag);
   for(i=100;i<=0;i--);
//-----------------
   OUTPUT(Data_H);
   for(i=100;i<=0;i--);
//-----------------
   OUTPUT(Data_L);
   for(i=100;i<=0;i--);
//-----------------
//多发一个 
   OUTPUT(0x00);
   for(i=100;i<=0;i--);
  }