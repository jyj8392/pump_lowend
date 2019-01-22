

#include<main.h>
#include "lcd.h"

sbit LCD_RS = P2^3;
sbit LCD_RW = P2^4;
sbit LCD_PSB = P2^6;
sbit LCD_E = P2^5;

/*void delay()
{
	unsigned int idata i,j;
	for(i=0;i<10;i++)
		for(j=0;j<10;j++);
} */
void LCD_busy()
{
	unsigned char idata busy_flag = 0;

	do{		
		LCD_E = 1;
		busy_flag = P0;
		LCD_E = 0;
	}while((busy_flag & 0x80) != 0);
}

void LCD_WRI(unsigned char LCD_COM)
{
//	LCD_E = 0;
	LCD_RS = 0;
	LCD_RW = 1;
    P0 = 0xff;
	LCD_busy();
	LCD_E = 0;
//	delay();
	LCD_RW = 0;
	P0 = LCD_COM;
//	delay();

	LCD_E = 1;LCD_E = 0;
}

void LCD_WRD(unsigned char LCD_DATA)
{ 	LCD_RS = 0;
	LCD_RW = 1;
	P0 = 0xff;
	LCD_busy();
//	delay();
	LCD_RS = 1;
	LCD_RW = 0;
	P0 = LCD_DATA;
//	delay();

	LCD_E = 1;LCD_E = 0;
;
}
void LCD_ini()
{
	LCD_WRI(0x30); //��������-����ָ�
	LCD_WRI(0x0f); //��ʾ��,����,������ʾ��
	LCD_WRI(0x01); //����
	LCD_WRI(0x06); //������趨,��������ƶ�,AC+1
	LCD_WRI(0x0c); //������趨,��������ƶ�,AC+1
}