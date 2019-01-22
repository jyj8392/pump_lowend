


?PR?_DELAY_NS SEGMENT CODE   ; //        在程序存储区中定义段

PUBLIC _DELAY_NS        ;      //声明函数

RSEG ?PR?_DELAY_NS;     //函数可被连接器放置在任何地方


_DELAY_NS:
         MOV A,R7
		 JZ DELAY_END
DELAY1S: MOV R2,#20
D1SL1:   MOV R1,#95
D1SL2:   MOV R0,#00H
         DJNZ R0,$
         DJNZ R1,D1SL2
         DJNZ R2,D1SL1
;----------------------
         DJNZ R7,_DELAY_NS
DELAY_END: 
         RET
		 END