#include "include.h"

/********************************************************************************************************/
//管脚定义
#define GPIO_LCD    GPIOE   
#define LCD_WR(x)    (x ? GPIO_SetBits(GPIOC, GPIO_Pin_2) : GPIO_ResetBits(GPIOC, GPIO_Pin_2)) 
#define LCD_RD(x)    (x ? GPIO_SetBits(GPIOC, GPIO_Pin_1) : GPIO_ResetBits(GPIOC, GPIO_Pin_1))
#define LCD_CS(x)    (x ? GPIO_SetBits(GPIOC, GPIO_Pin_3) : GPIO_ResetBits(GPIOC, GPIO_Pin_3))
#define LCD_A0(x)    (x ? GPIO_SetBits(GPIOA, GPIO_Pin_2) : GPIO_ResetBits(GPIOA, GPIO_Pin_2))
#define LCD_RES(x)   (x ? GPIO_SetBits(GPIOC, GPIO_Pin_0) : GPIO_ResetBits(GPIOC, GPIO_Pin_0))

/****************************************************************
【文 件 名 称】lcd12864.h
【功 能 描 述】lcd12864 头文件
【 作      者 】shifu
*****************************************************************/
//#ifndef __LCD12864_H
//#define __LCD12864_H
//****************************************************************
   //#include "stm32f10x_gpio.h"
//*****************************************************************

#define SYSTEM_SET		0x40
#define DISPLAY_OFF		0x58
#define DISPLAY_ON		0x59
#define SCROLL			0x44
#define CSRFORM			0x5D
#define HDOT_SCR		0x5A
#define OVLAY			0x5F
#define CSRW			0x46
#define MWRITE			0x42
#define MREAD			0x43

#define CSRDIR_RIGHT	0x4C
#define CSRDIR_LEFT     0x4D
#define CSRDIR_UP       0x4E
#define CSRDIR_DOWN     0x4F
						 
#define SYSTEM_SET  0x40
#define SCROLL      0x44
#define SLEEP_IN    0x53
#define DISP_ON     0x59
#define DISP_OFF    0x58
#define CGRAM_ADR   0x5c
#define CSR_RIGHT   0x4c
#define CSR_LEFT    0x4d
#define CSR_UP      0x4e
#define CSR_DOWN    0x4f
#define CSRW        0x46
#define CSRR        0x47
#define MWRITE      0x42
#define MREAD       0x43
void SED1335_set_text_cursor(unsigned int row, unsigned int col);
void SED1335_set_graphics_cursor(unsigned int row, unsigned int col);
// toggle pixels instead of on
unsigned char toggle;
 
void delay(u16 k)
{  
	u8 i;
	for(i=0;i<20;i++); 	
}

void LCD_Parallel_Out(uint8_t value) 
{ 

unsigned char tValue; 
tValue=~value; 
  GPIO_LCD->BSRR=(unsigned int)value<<7;   //使用高8位输出
  GPIO_LCD->BRR=(unsigned int)tValue<<7;   //使用高8位输出
    
//  GPIO_LCD->BSRR=(unsigned int)value;    //使用低8位输出
//  GPIO_LCD->BRR=(unsigned int)tValue;    //使用低8位输出
} 

void SED1335_send (char byte)
{  
//	u32	Dtmp;  

//	nRD(1);		    // nRD must always remain high
LCD_RD(1); 	//PIN0_SET(RD); 
   //RD_on;			 
	delay(2);
LCD_CS(0);  	//PIN0_CLR(CS);
   //CS_off;  
//	nCS(0);		    // chip select	

	delay(2);

//	nWR(0);		    // write
//LCD_WR(0);  	//PIN0_CLR(WR);
   //WR_off;
	delay(2);
	//if((byte&0x80)!=0) Dtmp = DB7;
	//else Dtmp = 0;
	//Dtmp += byte&0x7F; 
	//FIO2PIN = Dtmp;
	LCD_Parallel_Out(byte);
	delay(2);
   	LCD_WR(0);  	//PIN0_CLR(WR); 
	delay(2);
   
LCD_WR(1);  	//PIN0_SET(WR); 
   //WR_on;
//	nWR(1);
//	nCS(1);		    // chip unselect  

	delay(2);
LCD_CS(1);  	//PIN0_SET(CS);
   //CS_on;
}

// A0->HIGH (for command) 
void SED1335_send_command(u8 command)
{
//	A0(1);
LCD_A0(1);  	//PIN0_SET(A0);
    //A0_off;
	delay(2);
	SED1335_send(command);
}
// A0->LOW (for data)
void SED1335_send_data(u8 data)
{
//	A0(0);
 LCD_A0(0); 	//PIN0_CLR(A0);
//	A0_on;
	delay(2);
	SED1335_send(data);					
}



unsigned char  SYSTAB[]={0x30,0x87,0x07,0x27,0x42,0xf0,0x28,0x00}; // control parameter
unsigned char  SCRTAB[]={0x00,0x00,0xf0,0x00,0x40,0xf0,0x00,0x80,0x00,0x00};
//unsigned char  SCRTAB[]={0x00,0x00,0xf0,0x00,0x00,0xf0,0x00,0x00,0x00,0x00};
void SED1335_init (void)
{
	u32 i;
 
	 LCD_RES(1);	 //PIN0_SET(RET);



	SED1335_send_command(SYSTEM_SET);
	for(i=0;i<8;i++)
	{
		SED1335_send_data(SYSTAB[i]);			// P1
	}	

	SED1335_send_command(SCROLL);
	for(i=0;i<10;i++)
	{
		SED1335_send_data(SCRTAB[i]);			// SAD 1L
	}

	
	SED1335_send_command(0x5a);
	SED1335_send_data(0x00);//00			// OV, DM, MX

	SED1335_send_command(0x5b);
	SED1335_send_data(0xC);//00			// OV, DM, MX

	SED1335_send_command(0x59);
	SED1335_send_data(0x04);//00			// OV, DM, MX
	
	SED1335_send_command(0x5C);
	SED1335_send_data(0x55);//00			// OV, DM, MX
	SED1335_send_data(0x55);//00			// OV, DM, MX

    



}

void SED1335_display_on()
{
    SED1335_send_command(DISPLAY_ON);
}

void SED1335_display_off()
{
    SED1335_send_command(DISPLAY_OFF);
}

void SED1335_clear_text_layer(void)
{
	unsigned int i;

	SED1335_send_command(CSRW);
	SED1335_send_data(0x00);
	SED1335_send_data(0x00);

	SED1335_send_command(CSRDIR_RIGHT);

	SED1335_send_command(MWRITE);

	for (i=0; i<1200; i++)
	{
		SED1335_send_data(' ');	// space
	}
}
void SED1335_clear_graphics_layer(void)
{
	unsigned int i;

	SED1335_send_command(CSRW);
//	SED1335_send_data(0xB0);
//	SED1335_send_data(0x04);  
	SED1335_send_data(0x00);
	SED1335_send_data(0x00);

	SED1335_send_command(CSRDIR_RIGHT);

	SED1335_send_command(MWRITE);
										
	for (i=0; i<9600; i++)
	{
		SED1335_send_data(0);	// space
	}
}

void SED1335_set_text_cursor(unsigned int row, unsigned int col)
{
		unsigned int addr;
		addr = 40 * row + col;
		
		SED1335_send_command(CSRW);
		SED1335_send_data(addr & 0x00FF);
		SED1335_send_data(addr >> 8);
}

void SED1335_set_graphics_cursor(unsigned int row, unsigned int col)
{
	unsigned  int addr;
//	addr = 1200 + col + 40 * row;
	addr = col + 40 * row;
	SED1335_send_command(CSRW);
	SED1335_send_data(addr & 0x00FF);
	SED1335_send_data(addr >> 8);
}



void SED1335_write_char (unsigned char c)
{
	SED1335_send_command(MWRITE);
	SED1335_send_data(c);
}

void SED1335_write_char_at (unsigned char c, unsigned int row, unsigned int col)
{	
	SED1335_set_text_cursor(row, col);

	SED1335_send_command(MWRITE);
	SED1335_send_data(c);
}

void SED1335_write_str (const char* string)
{
	SED1335_send_command(MWRITE);

	while (*string != 0)
	{
		SED1335_send_data(*string);
		string++;
	}
}

void SED1335_write_str_at (const char* string, unsigned int row, unsigned int col)
{
	SED1335_set_text_cursor(row, col);
	SED1335_write_str(string);
} 

void SED1335_write_dec (unsigned int num)
{
	if ( (num / 100) > 0 )		// hundreds
	{
		SED1335_write_char((num/100)+'0');
	}
	if ( ((num / 10) > 0) || ((num / 100) > 0) )		// tens
	{
		SED1335_write_char((num%100)/10+'0');
	}

	// ones
	SED1335_write_char(num%10+'0');
}

void SED1335_write_hex (unsigned char c)
{
	unsigned char tmp;
	tmp = c;

	SED1335_write_char('h');

	// high 
	tmp = c >> 4;	
	if (tmp >= 10)
	{
		SED1335_write_char(tmp + 55);
	}
	else
	{
		SED1335_write_char(tmp + '0');
	}

	// low
	tmp = c & 0x0F;
	if (tmp >= 10)
	{
		SED1335_write_char(tmp + 55);
	}
	else
	{
		SED1335_write_char(tmp + '0');
	}	
}
/***** Graphics Functions *****/
 
#define SED1335_write_graphics(data)		\
    SED1335_send_command(MWRITE);			\
    SED1335_send_data(data)


#define SED1335_write_graphics_at(data,row,col)		\
    SED1335_set_graphics_cursor(row, col);			\
    SED1335_write_graphics(data)

/*
void SED1335_pixel_on (unsigned int x, unsigned int y)
{
    unsigned int row;
    unsigned int col;
	unsigned int data, graphics;
	
	row = y;
    col = x >> 3;   // div by 8
    
	//graphics = SED1335_read_graphics_at(row, col);
	// replaced with inline due to stack limit
	SED1335_set_graphics_cursor(row, col);
	SED1335_send_command(MREAD);
	graphics = SED1335_read();
	
	
    if (toggle)
    {
                 //   mask     ^   data
        data = (0x80 >> (x%8)) ^ graphics;
    }
    else
    {   
                 //   mask     |   data
        data = (0x80 >> (x%8)) | graphics;
    }

    SED1335_write_graphics_at(data, row, col);
}
*/
/*
void SED1335_pixel_off (unsigned int x, unsigned int y)
{
    unsigned int row;
    unsigned int col;
	unsigned int data, graphics;
	
	row = y;
	col = x >> 3;       // div by 8

	graphics = SED1335_read_graphics_at(row, col);

             //   mask     |   data

    data = ((0x80 >> (x&0x07)) ^ 0xFF) & graphics;
	
    SED1335_write_graphics_at(data, row, col);
}
*/
void SED1335_bitmap (unsigned char* map, unsigned int top_left_y, unsigned int top_left_x)
{
    unsigned int i, j;
    unsigned int height, width;
    
    width  = (((unsigned int)(map[0]))<<8) + map[1];      // in pixel
    width  = width >> 3;                                    // in bytes
    
    height = (((unsigned int)(map[2]))<<8) + map[3];
    
    
    for (i=0; i<height; i++)
    {                               
        unsigned int row, col;
        row = top_left_y + i;
        col = top_left_x >> 3;      // div by 8
        
        SED1335_set_graphics_cursor(row, col); 
        
        for (j=0; j<width; j++)
        {
            SED1335_write_graphics(map[i*width+j+4]);
        }
     }
} // end SED1335_bitmap
/*
void SED1335_invert_rows (unsigned char from, unsigned char to)
{
    unsigned int row, col;
	unsigned char data;
    
    // invert row1 through row2
	for (row = from; row <= to; row++)
	{	    
	    for (col=0; col<320; col=col+8)
	    {
	        data = SED1335_read_graphics_at(row,col>>3);        // read
	        data = ~data;                                       // invert
	        
	        SED1335_write_graphics_at(data, row, col>>3);       // write
	    }
	} // end for       
}
 */
void SED1335_toggle_on()
{
	toggle = 1;
}

void SED1335_toggle_off()
{
	toggle = 0;
}

void writestring(u16 x_addr,u16 y_addr,u16 NumX,u8 NumY,u8 const *ptr,u8 FB)
{   
	u16 i_hz,j; 
    unsigned int row, col;
  	for(i_hz=0;i_hz<NumY;i_hz++)  //计数值16
  	{					   
        row = y_addr + i_hz;
        col = x_addr;      // div by 8
        
        SED1335_set_graphics_cursor(row, col); 

		for(j=0;j<(NumX>>3);j++)
		{
			if(FB==0)
			{
			    SED1335_send_command(MWRITE);
			    SED1335_send_data(*ptr);
			}
			else
			{
			    SED1335_send_command(MWRITE);
			    SED1335_send_data(~*ptr);
			}
			ptr++;
		}
	}	
	_delay(1);
}

/*****************************************************************************************************/
//显示汉字子函数
void LCDString24(u16 x_addr,u16 y_addr,u16 NumX,u8 NumY,u16 hz ,u8 FB)
   {

	u16 i_hz,j; 
    unsigned int row, col;
	unsigned int k; 
	
	u8* buf;
	u16 h=0;  

       for(i_hz=0;i_hz<NumY;i_hz++)  //计数值16
  	        {					   
                        row = y_addr + i_hz;
                        col = x_addr;      // div by 8
        
                        SED1335_set_graphics_cursor(row, col); 

	                           for (k = 0;Hz_Code24[k].Hz_Index[0] != 0xff;k++)
			                      {

									   	if ( (u8)hz == Hz_Code24[k].Hz_Index[0] )
										{
										   if ( (hz>>8) == Hz_Code24[k].Hz_Index[1] )
										       {

												 

												      	for(j=0;j<(NumX>>3);j++)
		                                                {

															   buf=(u8*)(&(Hz_Code24[k].Code[h]));
		                                    	              if(FB==0)
			                                                   {
			                                                     SED1335_send_command(MWRITE);
			                                                     SED1335_send_data(*buf);
		                             	                       }
		                          	                       else
			                                                {
			                                                   SED1335_send_command(MWRITE);
			                                                   SED1335_send_data(~*buf);
			                                                 }
		                         	                             // ptr++;
																 h++;
		                                                
													     }
														 
														// return;											 											        
											      }
											   
										  }


	                                     
				                   }
	           }       
   }
//-----------------------------------------------------------------------------------------------------
//显示数字和字母子函数

void LCDASC1624(u16 x_addr,u16 y_addr,u16 NumX,u8 NumY,u16 hz ,u8 FB)
   {

	u16 i_hz,j; 
    unsigned int row, col;
	unsigned int k; 
	
	u8* buf;
	u16 h=0;  

       for(i_hz=0;i_hz<NumY;i_hz++)  //计数值16
  	        {					   
                        row = y_addr + i_hz;
                        col = x_addr;      // div by 8
        
                        SED1335_set_graphics_cursor(row, col); 

	                           for (k = 0;Hz_Asc1624[k].Hz_Index[0] != 0xff;k++)
			                      {

									   	if ( (u8)hz == Hz_Asc1624[k].Hz_Index[0] )
										{
										  // if ( (hz>>8) == Hz_Code24[k].Hz_Index[1] )
										       //{

												 

												      	for(j=0;j<(NumX>>3);j++)
		                                                {

															   buf=(u8*)(&(Hz_Asc1624[k].Code[h]));
		                                    	              if(FB==0)
			                                                   {
			                                                     SED1335_send_command(MWRITE);
			                                                     SED1335_send_data(*buf);
		                             	                       }
		                          	                       else
			                                                {
			                                                   SED1335_send_command(MWRITE);
			                                                   SED1335_send_data(~*buf);
			                                                 }
		                         	                             // ptr++;
																 h++;
		                                                
													     }
														 
														// return;											 											        
											   //   }
											   
										   }


	                                     
				                   }
	           }       
   }

//----------------------------------------------------------------------------------------
//汉字为 24X24 数字或字母为 16X24
// void Writestring24(INT16U x_addr,INT16U y_addr,INT8U const *ptr,INT8U FB)
void Writestring24(u16 x_addr,u16 y_addr,u8  *ptr,u8 FB)
 {
   u16 NumX=24;
   u8 NumY=24;	
   u16 hz;
  

      while((*ptr != '\0'))
        {

		   if ((*ptr <= 0x80) && (*ptr >= 0x20)) // ASCII	
		   {

		      if(*ptr == 0x2e)	  //'.'
			   {
			      hz=*ptr++;
			      NumX=8;
                  NumY=24;  
			      LCDASC1624(x_addr,y_addr,NumX,NumY,hz,FB);
			      x_addr+=1;
			   }
			   else
			  {
		          hz=*ptr++;
			      NumX=16;
                  NumY=24;  
			      LCDASC1624(x_addr,y_addr,NumX,NumY,hz,FB);
			      x_addr+=2;
			  }
		    }

 //-------------------------------    
                   //  if (*ptr >= 0xb0)//汉字
				   if(*ptr>=0xa1)
		              {
		                   hz=*ptr++;
			               hz+=(*ptr++)<<8;
		                  //--------------------------- 
						   NumX=24;
                           NumY=24;
		                  LCDString24(x_addr,y_addr,NumX,NumY,hz,FB);	    
		               	  x_addr+=3;

		              }
	   //----------------------------------------------
	     
	}
 }
