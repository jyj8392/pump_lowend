
 TXD_M     EQU   P2.0
 RXD_M     EQU   P2.1

?PR?_OUTPUT SEGMENT CODE   ; //        在程序存储区中定义段

PUBLIC _OUTPUT        ;      //声明函数

RSEG ?PR?_OUTPUT;     //函数可被连接器放置在任何地方

;******************************************
;发送数据子程序
_OUTPUT: MOV A,R7
         CLR C
         MOV TXD_M,C
         LCALL DEL833
         MOV R3,#8
  OUT:   RLC  A
         MOV TXD_M,C
         LCALL DEL833
         DJNZ R3,OUT
         SETB C
         MOV TXD_M,C
         LCALL DEL833
         RET
;*******************************
;***************************
 DEL833: MOV R6,#205
         DJNZ R6,$
         RET
;****************************
         END