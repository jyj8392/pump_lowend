#include "include.h"
#include "oled.h"

#define ON				1
#define OFF				0
#define OLED_EN			GPIOD, GPIO_Pin_4
#define OLED_RW			GPIOD, GPIO_Pin_3
#define OLED_RES		GPIOD, GPIO_Pin_0
#define OLED_CS			GPIOD, GPIO_Pin_1
#define OLED_DC			GPIOD, GPIO_Pin_7
#define OLED_DB7		GPIOA, GPIO_Pin_12
#define OLED_DB6		GPIOC, GPIO_Pin_12
#define OLED_DB5		GPIOA, GPIO_Pin_11
#define OLED_DB4		GPIOA, GPIO_Pin_10
#define OLED_DB3		GPIOA, GPIO_Pin_9
#define OLED_DB2		GPIOC, GPIO_Pin_9
#define OLED_DB1		GPIOA, GPIO_Pin_8
#define OLED_DB0		GPIOC, GPIO_Pin_8

#define SetOLEDDataBit(db, sw)	(sw == ON? GPIO_SetBits(db): GPIO_ResetBits(db))
#define SetOLEDCTL(ctl, sw)		(sw == ON? GPIO_SetBits(ctl): GPIO_ResetBits(ctl))

#define SetOLEDData(x)	{																	\
	if ((x & 0x80) == 0x80) {SetOLEDDataBit(OLED_DB7, ON);} else {SetOLEDDataBit(OLED_DB7, OFF);} \
	if ((x & 0x40) == 0x40) {SetOLEDDataBit(OLED_DB6, ON);} else {SetOLEDDataBit(OLED_DB6, OFF);} \
	if ((x & 0x20) == 0x20) {SetOLEDDataBit(OLED_DB5, ON);} else {SetOLEDDataBit(OLED_DB5, OFF);} \
	if ((x & 0x10) == 0x10) {SetOLEDDataBit(OLED_DB4, ON);} else {SetOLEDDataBit(OLED_DB4, OFF);} \
	if ((x & 0x08) == 0x08) {SetOLEDDataBit(OLED_DB3, ON);} else {SetOLEDDataBit(OLED_DB3, OFF);} \
	if ((x & 0x04) == 0x04) {SetOLEDDataBit(OLED_DB2, ON);} else {SetOLEDDataBit(OLED_DB2, OFF);} \
	if ((x & 0x02) == 0x02) {SetOLEDDataBit(OLED_DB1, ON);} else {SetOLEDDataBit(OLED_DB1, OFF);} \
	if ((x & 0x01) == 0x01) {SetOLEDDataBit(OLED_DB0, ON);} else {SetOLEDDataBit(OLED_DB0, OFF);} \
	} while(0)

void Write_Cmd(unsigned char cmd)
{       
	SetOLEDCTL(OLED_DC, OFF);//DC=0;
	SetOLEDCTL(OLED_CS, OFF);//CS=0;
	SetOLEDCTL(OLED_RW, OFF);//RW=0;
	SetOLEDData(cmd);
    SetOLEDCTL(OLED_EN, ON);//E=1;
    SetOLEDCTL(OLED_EN, OFF);//E=0;
//	SetOLEDCTL(OLED_RW, ON);//RW=1;
	SetOLEDCTL(OLED_CS, ON);//CS=1;
//	SetOLEDCTL(OLED_DC, ON);//DC=1;
}

void Write_Data(unsigned char dat)
{        
	SetOLEDCTL(OLED_DC, ON);//DC=1;
	SetOLEDCTL(OLED_CS, OFF);//CS=0;
	SetOLEDCTL(OLED_RW, OFF);//RW=0;
	SetOLEDData(dat);
    SetOLEDCTL(OLED_EN, ON);//E=1;
    SetOLEDCTL(OLED_EN, OFF);//E=0;
//	SetOLEDCTL(OLED_RW, ON);//RW=1;
	SetOLEDCTL(OLED_CS, ON);//CS=1;
//	SetOLEDCTL(OLED_DC, ON);//DC=1;
}


void Set_Column_Address(unsigned char a, unsigned char b)
{
	Write_Cmd(0x15);			// Set Column Address
	Write_Data(a);				//   Default => 0x00
	Write_Data(b);				//   Default => 0x77
}


void Set_Row_Address(unsigned char a, unsigned char b)
{
	Write_Cmd(0x75);			// Set Row Address
	Write_Data(a);				//   Default => 0x00
	Write_Data(b);				//   Default => 0x7F
}

void Set_Write_RAM()
{
	Write_Cmd(0x5C);			// Enable MCU to Write into RAM
}

void Set_Remap_Format(unsigned char d)
{
	Write_Cmd(0xA0);			// Set Re-Map / Dual COM Line Mode
	Write_Data(d);				//   Default => 0x40
	Write_Data(0x11);			//   Default => 0x01 (Disable Dual COM Mode)
}

void Fill_RAM(unsigned char Data)
{
unsigned char i,j;

	Set_Column_Address(0x1c,0x5b);
	Set_Row_Address(0x00,0x3f);
	Set_Write_RAM();

	for(i=0;i<64;i++)
	{
		for(j=0;j<64;j++)
		{
			Write_Data(Data);
			Write_Data(Data);
		}
	}
}

/************************************************
????,D7=1,???????
************************************************/
unsigned char data_4byte[4];
void change(unsigned char m)
{
	unsigned char k;
	unsigned char kk,kkk;
	kk=m;
	for(k=0;k<4;k++)   // ????????  2*4=8?
	{
		kkk=kk&0xc0;     //?K=0? ?D7,D6? ?K=1? ?D5,D4?

	/****??4???,16???,????????,??????4?***/

		switch(kkk)
		{
		case 0x00:
			data_4byte[k]=0x00;
			break;
		case 0x40:
			data_4byte[k]=0x0F;
			break;
		case 0x80:
			data_4byte[k]=0xF0;
			break;
		case 0xc0:
			data_4byte[k]=0xFF;
			break;
		default:
		break;
		}
		kk=kk<<2;                                //????
		Write_Data(data_4byte[k]);                /* 8 column  a nibble of command is a dot*/
	}
}

void ShowBlock(void)
{
    unsigned char i;
	Set_Remap_Format(0x15);
	Write_Cmd(0x15);
	Write_Data(0x1c);
	Write_Data(0x1c);
	Write_Data(0x75);
	Write_Data(0x01);
	Write_Data(0x3e);
	Write_Cmd(0x5c);
	for(i=0;i<64;i++)
	{
		Write_Data(0xf0);
		Write_Data(0x00);
	}
	
	Write_Cmd(0x15);
	Write_Data(0x5b);
	Write_Data(0x5b);
	Write_Cmd(0x5c);
	for(i=0;i<64;i++)
	{
		Write_Data(0x00);
		Write_Data(0x0f);
	}
	
	Write_Cmd(0x15);
	Write_Data(0x1c+26);
	Write_Data(0x1c+26);
	Write_Cmd(0x5c);
	for(i=0;i<64;i++)
	{
		Write_Data(0x00);
		Write_Data(0x0f);
	}
	
	Write_Cmd(0x15);
	Write_Data(0x1c+52);
	Write_Data(0x1c+52);
	Write_Cmd(0x5c);
	for(i=0;i<64;i++)
	{
		Write_Data(0x00);
		Write_Data(0x0f);
	}
	Set_Remap_Format(0x14);	  
//	for(i=0;i<64;i++){
//		Write_Cmd(0x15);
//		Write_Data(0x1c);
//		Write_Data(0x1c);

//		Write_Cmd(0x75);
//		Write_Data(0x00+i);
//		Write_Data(0x00+i);
//		Write_Cmd(0x5c);

//		Write_Data(0xf0);
//		Write_Data(0x00);
//	}	
//	for(i=0;i<64;i++){
//		Write_Cmd(0x15);
//		Write_Data(0x5b);
//		Write_Data(0x5b);

//		Write_Cmd(0x75);
//		Write_Data(0x00+i);
//		Write_Data(0x00+i);
//		Write_Cmd(0x5c);

//		Write_Data(0x00);
//		Write_Data(0x0f);
//	}	
	Write_Cmd(0x15);
	Write_Data(0x1c);
	Write_Data(0x5b);

	Write_Cmd(0x75);
	Write_Data(0x00);
	Write_Data(0x00);
	Write_Cmd(0x5c);

	for(i=0;i<64;i++)
	{
		Write_Data(0xff);
		Write_Data(0xff);
	}
	

	Write_Cmd(0x75);
	Write_Data(0x3f);
	Write_Data(0x3f);
	Write_Cmd(0x5c);

	for(i=0;i<64;i++)
	{
		Write_Data(0xff);
		Write_Data(0xff);
	}
}

void ShowNUM816(unsigned char x,unsigned char y,char coder, unsigned char back)
{
	unsigned char i, j;
	Set_Remap_Format(0x14);     //??2???,1???
	Write_Cmd(0x15);
	Write_Data(0x1c+x);
	Write_Data(0x1d+x);

	Write_Cmd(0x75);              //16?
	Write_Data(0x00+y);
	Write_Data(0x0f+y);
	Write_Cmd(0x5c);
	
	for (i = 0; i < sizeof(arr_Index816) / sizeof(struct num816_t); i++) {
		if (arr_Index816[i].index == coder) break;
	}
	
	for (j = 0; j < sizeof(arr_Index816->arr); j++) {
		change(back == OFF?arr_Index816[i].arr[j]: ~arr_Index816[i].arr[j]);
	}
}

void ShowNUM1624(unsigned char x,unsigned char y,char coder, unsigned char back)
{
	unsigned char i, j;
	Set_Remap_Format(0x14);     //??2???,1???
	Write_Cmd(0x15);
	Write_Data(0x1c+x);
	Write_Data(0x1f+x);

	Write_Cmd(0x75);              //16?
	Write_Data(0x00+y);
	Write_Data(0x16+y);
	Write_Cmd(0x5c);
	
	for (i = 0; i < sizeof(arr_Index1624) / sizeof(struct num1624_t); i++) {
		if (arr_Index1624[i].index == coder) break;
	}
	
	for (j = 0; j < sizeof(arr_Index1624->arr); j++) {
		change(back == OFF?arr_Index1624[i].arr[j]: ~arr_Index1624[i].arr[j]);
	}
}

void ShowSYM824(unsigned char x,unsigned char y,char coder, unsigned char back)
{
	unsigned char i, j;
	Set_Remap_Format(0x14);     //??2???,1???
	Write_Cmd(0x15);
	Write_Data(0x1c+x);
	Write_Data(0x1d+x);

	Write_Cmd(0x75);              //16?
	Write_Data(0x00+y);
	Write_Data(0x16+y);
	Write_Cmd(0x5c);
	
	for (i = 0; i < sizeof(arr_Index824) / sizeof(struct symbo824_t); i++) {
		if (arr_Index824[i].index == coder) break;
	}
	
	for (j = 0; j < sizeof(arr_Index824->arr); j++) {
		change(back == OFF?arr_Index824[i].arr[j]: ~arr_Index824[i].arr[j]);
	}
}

void ShowHZ1616(unsigned char x,unsigned char y,char str[], unsigned char back)
{
	unsigned char i, j;
	Set_Remap_Format(0x14);     //??2???,1???
	Write_Cmd(0x15);
	Write_Data(0x1c+x);
	Write_Data(0x1f+x);

	Write_Cmd(0x75);              //16?
	Write_Data(0x00+y);
	Write_Data(0x10+y);
	Write_Cmd(0x5c);
	
	for (i = 0; i < sizeof(arr_Index1616) / sizeof(struct hz_t); i++) {
		for (j = 0; j < sizeof(arr_Index1616[i].index); j++)
		if (arr_Index1616[i].index[j] != str[j]) break;
		if (j == sizeof(arr_Index1616[i].index)) break;
	}
	if (i < sizeof(arr_Index1616) / sizeof(struct hz_t)) {
		for (j = 0; j < 32; j++) {
			change(back == OFF?arr_Index1616[i].arr[j]: ~arr_Index1616[i].arr[j]);
		}
	}
}

void ShowASCII816Str(unsigned char x,unsigned char y,char str[], unsigned char back)
{
	unsigned char i = 0;
	while (str[i] != '\0') {
		ShowNUM816(x, y, str[i], back);
		x += 2;
		i += 1;
	}
}

void ShowHZ1616Str(unsigned char x,unsigned char y,char str[], unsigned char back)
{
	unsigned char i = 0;
	char tmp[2];
	while (str[i] != '\0') {
		if (i % 2 == 0) {
			tmp[0] = str[i];
			tmp[1] = str[i+1];
			ShowHZ1616(x, y, tmp, back);
			x += 4;
		}
		i += 1;
	}
}

void ShowASCII1624Str(unsigned char x,unsigned char y,char str[], unsigned char back)
{
	unsigned char i = 0;
	while (str[i] != '\0') {
		if ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) {
			ShowNUM1624(x, y, str[i], back);
			x += 4;
		} else {
			ShowSYM824(x, y, str[i], back);
			x += 2;
		}
		
		i += 1;
	}
}

void OLED_Initial (void)
{
	
    SetOLEDCTL(OLED_RES, OFF);//REST=0;
	SetOLEDCTL(OLED_RES, ON);//REST=1;
	Write_Cmd(0xFD);
 	Write_Data(0x12);  //??MCU??

	Write_Cmd(0xAE);  //???
	
	Set_Column_Address(0x1c,0x5b);	   //256?
	Set_Row_Address(0x00,0x3f);		 //64?

	Write_Cmd(0xA0);
// 	Write_Data(0x14);
 	Write_Data(0x06);
	Write_Data(0x11);  //????

	Write_Cmd(0xA1);
	Write_Data(0x00); //???
	
	Write_Cmd(0xA2);
	Write_Data(0x00); //????
	
        Write_Cmd(0xA6);	  //????
        Write_Cmd(0xA9);	  //????
		
	Write_Cmd(0xAB);
	Write_Data(0x01); //??????
	
	Write_Cmd(0xB1); //Set Phase Length
	Write_Data(0x52);
//	Write_Data(0xE2);

	Write_Cmd(0xB3);  ///???????
	Write_Data(0x91);
//	Write_Data(0x11);

	Write_Cmd(0xB4); ///????Display Enhancement a
	Write_Data(0xA0);
	Write_Data(0xfd);   //0xb5
	
	Write_Cmd(0xB5); ///set gpio
	Write_Data(0x00);
	
	Write_Cmd(0xB6);
	Write_Data(0x08);

	Write_Cmd(0xb8);  //??
	Write_Data(0x0c);
	Write_Data(0x18);
	Write_Data(0x24);
	Write_Data(0x30);
	Write_Data(0x3c);
	Write_Data(0x48);
	Write_Data(0x54);
	Write_Data(0x60);
 	Write_Data(0x6c);
	Write_Data(0x78);
	Write_Data(0x84);
	Write_Data(0x90);
	Write_Data(0x9c);
	Write_Data(0xa8);
	Write_Data(0xb4);
	
	Write_Cmd(0x00);
	
	Write_Cmd(0xBB);
	Write_Data(0x1F);

 	Write_Cmd(0xBE);
	Write_Data(0x07);//0.86VCC
	
	Write_Cmd(0xC1); ///Set Contrast Current
	Write_Data(0x9f); //9f
//	Write_Data(0xff); //9f
	
	Write_Cmd(0xC7);  //Master Contrast	Current Control
	Write_Data(0x0F);
	
	Write_Cmd(0xCA); ///??MUX???
	Write_Data(0x3F);
	
	Write_Cmd(0xD1); //Display Enhancement B
	Write_Data(0x82);
//	Write_Data(0xa2);
//	Write_Data(0x00);//??
	Write_Data(0x20);//??
	










//	Write_Cmd(0x15);			// Set Column Address
//	Write_Data(0x00);				//   Default => 0x00
//	Write_Data(0x77);				//   Default => 0x77
//	Write_Cmd(0x75);			// Set Column Address
//	Write_Data(0x00);				//   Default => 0x00
//	Write_Data(0x7F);				//   Default => 0x77
//	Write_Cmd(0x5C);			// Set Column Address
//int i,j;
//for (j = 0; j < 128; j++) {
//	for(i = 0; i < 120; i++) {
//	Write_Data(0xf0);				//   Default => 0x00
//	Write_Data(0xf0);				//   Default => 0x77
//	}
//} 





 	Write_Cmd(0xA6);//????
//	Write_Cmd(0xA9);//?????
//	Write_Cmd(0x5c);
	Fill_RAM(0x00);
        Write_Cmd(0xAF);  //???
//		ShowNUM612(0, 0, '1');
//		ShowStr(32, 0, "下限");
//		ShowHZ1616(32, 32, "限");
//		ShowHZ1616(32, 48, "限");
//		write_oled8(12, 2, 18);
//		write_oled8(14, 2, 25);
//		write_oled8(16, 2, 40);

//ShowNUM1624(10,25,'0', 0);
}

void OLED_SetContrast(unsigned char step)
{
	Write_Cmd(0xC1); ///Set Contrast Current
	Write_Data(0x0C + 0x1B * step); //9f
}
