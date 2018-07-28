#include "include.h"

#define ON				1
#define OFF				0
#define LCD_EN			GPIOB, GPIO_Pin_15
#define LCD_RW			GPIOB, GPIO_Pin_11
#define LCD_RS			GPIOB, GPIO_Pin_10
#define LCD_PSB			GPIOA, GPIO_Pin_12
#define	LCD_DB0			GPIOC, GPIO_Pin_6
#define	LCD_DB1			GPIOC, GPIO_Pin_7
#define	LCD_DB2			GPIOC, GPIO_Pin_8
#define	LCD_DB3			GPIOC, GPIO_Pin_9
#define	LCD_DB4			GPIOA, GPIO_Pin_8
#define	LCD_DB5			GPIOA, GPIO_Pin_9
#define	LCD_DB6			GPIOA, GPIO_Pin_10
#define	LCD_DB7			GPIOA, GPIO_Pin_11

#define SetLCDDataBit(db, sw)	(sw == ON? GPIO_SetBits(db): GPIO_ResetBits(db))
#define GetLCDDataBit(db)	GPIO_ReadInputDataBit(db)
#define SetLCDCTL(ctl, sw)		(sw == ON? GPIO_SetBits(ctl): GPIO_ResetBits(ctl))

#define SetLCDData(x)	do {																	\
	if ((x & 0x80) == 0x80) {SetLCDDataBit(LCD_DB7, ON);} else {SetLCDDataBit(LCD_DB7, OFF);} \
	if ((x & 0x40) == 0x40) {SetLCDDataBit(LCD_DB6, ON);} else {SetLCDDataBit(LCD_DB6, OFF);} \
	if ((x & 0x20) == 0x20) {SetLCDDataBit(LCD_DB5, ON);} else {SetLCDDataBit(LCD_DB5, OFF);} \
	if ((x & 0x10) == 0x10) {SetLCDDataBit(LCD_DB4, ON);} else {SetLCDDataBit(LCD_DB4, OFF);} \
	if ((x & 0x08) == 0x08) {SetLCDDataBit(LCD_DB3, ON);} else {SetLCDDataBit(LCD_DB3, OFF);} \
	if ((x & 0x04) == 0x04) {SetLCDDataBit(LCD_DB2, ON);} else {SetLCDDataBit(LCD_DB2, OFF);} \
	if ((x & 0x02) == 0x02) {SetLCDDataBit(LCD_DB1, ON);} else {SetLCDDataBit(LCD_DB1, OFF);} \
	if ((x & 0x01) == 0x01) {SetLCDDataBit(LCD_DB0, ON);} else {SetLCDDataBit(LCD_DB0, OFF);} \
	} while(0)

#define GetLCDData(x)	do {																	\
	x |= (GetLCDDataBit(LCD_DB7) << 7);	\
	x |= (GetLCDDataBit(LCD_DB6) << 6);	\
	x |= (GetLCDDataBit(LCD_DB5) << 5);	\
	x |= (GetLCDDataBit(LCD_DB4) << 4);	\
	x |= (GetLCDDataBit(LCD_DB3) << 3);	\
	x |= (GetLCDDataBit(LCD_DB2) << 2);	\
	x |= (GetLCDDataBit(LCD_DB1) << 1);	\
	x |= (GetLCDDataBit(LCD_DB0) << 0);	\
	} while(0)

#define GetBFBit()	GetLCDDataBit(LCD_DB7)

void LCD_delay(void)
{
	int i;
	for (i = 0; i < 1000; i++);
}
	
void Wait_for_Transmit(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	unsigned char isBusy = 0;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	SetLCDCTL(LCD_RS, OFF);//RS=0;
	SetLCDCTL(LCD_RW, ON);//RW=1;
	SetLCDCTL(LCD_EN, ON);//EN=1;
//	LCD_delay();
//	do {
//		GetLCDData(isBusy);
//	} while((isBusy & 0x80) != 0);
	while (GetBFBit() == 1);
	
    SetLCDCTL(LCD_EN, OFF);//EN=0;
//	LCD_delay();
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_6);
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	GPIO_SetBits(GPIOC, GPIO_Pin_8);
	GPIO_SetBits(GPIOC, GPIO_Pin_9);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
	GPIO_SetBits(GPIOA, GPIO_Pin_9);
	GPIO_SetBits(GPIOA, GPIO_Pin_10);
	GPIO_SetBits(GPIOA, GPIO_Pin_11);
}	
	
void Write_Cmd(unsigned char cmd)
{    
	Wait_for_Transmit();
	SetLCDCTL(LCD_RS, OFF);//RS=0;
	SetLCDCTL(LCD_RW, OFF);//RW=0;
	SetLCDData(cmd);
    SetLCDCTL(LCD_EN, ON);//EN=1;
    SetLCDCTL(LCD_EN, OFF);//E=0;
}

void Write_Data(unsigned char dat)
{
	Wait_for_Transmit();
	SetLCDCTL(LCD_RS, ON);//RS=1;
	SetLCDCTL(LCD_RW, OFF);//RW=0;
	SetLCDData(dat);
    SetLCDCTL(LCD_EN, ON);//EN=1;
    SetLCDCTL(LCD_EN, OFF);//E=0;
}

void Clear_Screen(void)
{
	int i;
	Write_Cmd(0x01);
	for (i = 0; i < 20; ++i) LCD_delay();
}

void Set_Address(unsigned char row, unsigned char column)
{
	switch (row) {
	case 0: Write_Cmd(0x80 + column); break;
	case 1: Write_Cmd(0x90 + column); break;
	case 2: Write_Cmd(0x88 + column); break;
	case 3: Write_Cmd(0x98 + column); break;
	}
}

void Set_Reverse_Type(unsigned char row, unsigned char column, unsigned char rev)
{
//	Set_Address(row, column);
	Write_Cmd(0x0c);
//	Write_Cmd(0x0c | rev);
}


void ShowStr(unsigned char row, unsigned char column,
	char str[], unsigned char back)
{
	unsigned char i = 0;
	
	Set_Address(row, column);
	Set_Reverse_Type(row, column, back);
	while (str[i] != '\0')
		Write_Data((unsigned char)str[i++]);	
}

void LCD_Initial(void)
{
	int i;
	SetLCDCTL(LCD_PSB, ON);//PSB=1 使用并口
	for (i = 0; i < 100; ++i) LCD_delay();
	
	Write_Cmd(0x30);
	Write_Cmd(0x0f);
	Write_Cmd(0x01);
	Write_Cmd(0x06);
	Write_Cmd(0x0c);
	for (i = 0; i < 100; ++i) LCD_delay();
}
