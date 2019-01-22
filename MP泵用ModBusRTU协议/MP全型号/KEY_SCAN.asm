




 
?PR?KEY_SCAN SEGMENT CODE   ; //        在程序存储区中定义段

PUBLIC KEY_SCAN        ;      //声明函数

RSEG ?PR?KEY_SCAN;     //函数可被连接器放置在任何地方



KEY_SCAN:
//	 ;以下是4X4键盘扫描程序
     
KEYL1:  MOV R3,#0F7H
        MOV R7,#00H
//;********************
//;按键消抖程序
        MOV A,#0F0H
        MOV P1,A
        MOV A,P1
        CJNE A,#0F0H,KEYDOU
       ; LJMP KEYEND
	   LJMP KEYL1
KEYDOU:  MOV R6,#00H
         DJNZ R6,$
//         ;***********
         MOV A,P1
         CJNE A,#0F0H,KEYDOUIN
       ; LJMP KEYEND
	   LJMP KEYL1
KEYDOUIN: NOP
//;********************
KEYL2:  MOV A,R3
        MOV P1,A
        MOV A,P1
        MOV R4,A
        SETB C
        MOV R5,#04H
KEYL3:  RLC A
        JNC KEYIN
        INC R7
        DJNZ R5,KEYL3
        MOV A,R3
        SETB C
        RRC A
        MOV R3,A
        JC KEYL2
      //  LJMP KEYEND
	    LJMP KEYL1
KEYIN:  SETB P2.7   //;BUFFER
        MOV R6,#60
KEYD2:  MOV R5,#248
        DJNZ R5,$
        DJNZ R6,KEYD2
        CLR P2.7
KEYD3:  MOV A,P1
        XRL A,R4
        JZ KEYD3
        LCALL DELAY1
KEYEND:  NOP
        
         RET
;******************************************
DELAY1: MOV R0,#60
D1L2:   MOV R6,#06
D1L1:   MOV R5,#248
        DJNZ R5,$
        DJNZ R6,D1L1
        DJNZ R0,D1L2
        RET
;******************************************************
		 END