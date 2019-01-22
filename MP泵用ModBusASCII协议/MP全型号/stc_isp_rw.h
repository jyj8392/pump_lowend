sfr	 ISP_DATA	=   0xe2;
sfr  ISP_ADDRH	=   0xe3;  
sfr	 ISP_ADDRL	=   0xe4;
sfr	 ISP_CMD	=   0xe5; 
sfr	 ISP_TRIG	=   0xe6;   
sfr	 ISP_CONTR  =   0xe7; 

#define Read_Byte_Cmd 		0x01
#define Write_Byte_Cmd		0x02
#define Erase_Section_Cmd	0x03
#define WAIT_TIME		0x00

unsigned char EEPROM_byte_read(unsigned int byte_addr);
unsigned char EEPROM_sector_erase(unsigned int sector_addr);
unsigned char EEPROM_byte_program(unsigned int byte_addr, unsigned char original_data);
