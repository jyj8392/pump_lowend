
 TXD_M     EQU   P2.0
 RXD_M     EQU   P2.1

?PR?_OUTPUT SEGMENT CODE   ; //        �ڳ���洢���ж����

PUBLIC _OUTPUT        ;      //��������

RSEG ?PR?_OUTPUT;     //�����ɱ��������������κεط�

;******************************************
;���������ӳ���
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