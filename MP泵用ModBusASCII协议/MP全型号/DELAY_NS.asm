


?PR?_DELAY_NS SEGMENT CODE   ; //        �ڳ���洢���ж����

PUBLIC _DELAY_NS        ;      //��������

RSEG ?PR?_DELAY_NS;     //�����ɱ��������������κεط�


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