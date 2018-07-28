

typedef struct
{
	unsigned char Hz_Index[3];		//Ë÷Òý
	unsigned char Code[72];		//×Ö¿â
}HZ_CODE24;

typedef struct
{
	unsigned char Hz_Index[3];		//Ë÷Òý
	unsigned char Code[48];		//×Ö¿â
}HZ_ASC1624;

//OS_EXTI  void writestring(unsigned short x_addr,unsigned short y_addr,unsigned short NumX,unsigned char NumY,unsigned char const *ptr,unsigned char FB);
//OS_EXTI  void SED1335_clear_graphics_layer(void);

extern  const HZ_CODE24 Hz_Code24[];
extern  const HZ_ASC1624 Hz_Asc1624[];

//extern unsigned char const BmpHand[];
//extern unsigned char const BmpHandClear[];
//
//extern unsigned char const Num24[];

//extern const unsigned char ASCII1624[];

extern void writestring(u16 x_addr,u16 y_addr,u16 NumX,u8 NumY,u8 const *ptr,u8 FB);
extern void Writestring24(u16 x_addr,u16 y_addr,u8  *ptr,u8 FB);

extern  void SED1335_clear_graphics_layer(void);
extern void SED1335_init (void);

extern void SED1335_set_graphics_cursor(unsigned int row, unsigned int col);
