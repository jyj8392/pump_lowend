#include <main.h>
#include <intrins.h>
#include "STC_ISP_RW.h"


//�� ISP,IAP ���� 
void ISP_IAP_enable(void)
{
	EA	=	0;	// ���ж� 
	ISP_CONTR	=	ISP_CONTR & 0x18;       // 0001,1000
	ISP_CONTR	=	ISP_CONTR | WAIT_TIME;
	ISP_CONTR	=	ISP_CONTR | 0x80;       // 1000,0000 
}

// �ر� ISP,IAP ���� 
void ISP_IAP_disable(void)
{										
	ISP_CONTR	=	ISP_CONTR & 0x7f;	// 0111,1111 
	ISP_TRIG	=	0x00;
	EA			=   1;               	 // ���ж� 
}

// �ֽڶ�
unsigned char EEPROM_byte_read(unsigned int byte_addr)
{
	ISP_ADDRH	=	(unsigned char)(byte_addr >> 8);
	ISP_ADDRL	=	(unsigned char)(byte_addr & 0x00ff);

	ISP_CMD		=	ISP_CMD	&	0xf8;       // 1111,1000 
	ISP_CMD		=	ISP_CMD	|	Read_Byte_Cmd;        // 0000,0001 

	ISP_IAP_enable();

	ISP_TRIG	=	0x46;
	ISP_TRIG	=	0xb9;
	_nop_();

	ISP_IAP_disable();
	return (ISP_DATA);
}

// ��������
unsigned char EEPROM_sector_erase(unsigned int sector_addr)
{
	unsigned int get_sector_addr	=	0;
	get_sector_addr			=	(sector_addr & 0xfe00); /// 1111,1110,0000,0000; ȡ������ַ
	ISP_ADDRH        		=	(unsigned char)(get_sector_addr >> 8);
	ISP_ADDRL		        =	0x00;

	ISP_CMD	=	ISP_CMD	&	0xf8;	// 1111,1000
	ISP_CMD	=	ISP_CMD	|	Erase_Section_Cmd;	// 0000,0011

	ISP_IAP_enable();
	ISP_TRIG	=	0x46;        // ����ISP_IAP���� 
	ISP_TRIG	=	0xb9;        // ����ISP_IAP���� 
	 _nop_();

	ISP_IAP_disable();
	return 1;
}

//�ֽڱ�� 
unsigned char EEPROM_byte_program(unsigned int byte_addr, unsigned char original_data)
{
	ISP_ADDRH	=	(unsigned char)(byte_addr >> 8);
	ISP_ADDRL	=	(unsigned char)(byte_addr & 0x00ff);

	ISP_CMD		=	ISP_CMD	&	0xf8;        // 1111,1000 
	ISP_CMD		=	ISP_CMD	|	Write_Byte_Cmd;		// 0000,0010 
	ISP_DATA	=	original_data;

	ISP_IAP_enable();
	ISP_TRIG	=	0x46;        // ����ISP_IAP���� 
	ISP_TRIG	=	0xb9;        // ����ISP_IAP���� 
	_nop_();

	ISP_IAP_disable();
	return	1;
}	  	
