
 CS549   EQU   P3.2
 CLK549  EQU   P3.3
 DATA549 EQU   P3.5
; ADC     EQU   22H

?PR?TLC549 SEGMENT CODE   ; //        在程序存储区中定义段

PUBLIC TLC549        ;      //声明函数

RSEG ?PR?TLC549;     //函数可被连接器放置在任何地方


;TLC549
 TLC549: SETB CS549
         SETB DATA549
         CLR CLK549
         MOV R0,#7
         DJNZ R0,$
         NOP
         MOV A,#00H
         CLR C
         CLR CS549
         NOP
         NOP
         MOV C,DATA549
         RLC A
         MOV R1,#07H
DATAOUT: SETB CLK549
         NOP
         CLR CLK549
         NOP
         MOV C,DATA549
         RLC A
         DJNZ R1,DATAOUT
         SETB CLK549
         NOP
         CLR CLK549
         SETB CS549
       ;  MOV ADC,A
	     MOV R7,A
         RET
;---------------------------------------
         END