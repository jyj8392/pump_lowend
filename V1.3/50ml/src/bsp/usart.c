

  #include "stm32f10x.h"

/**********************************************
**�������ú���,����ʹ������������,���д���2ʹ�����жϽ���ģʽ
**
**********************************************/
u8 Uart1_Get_Flag;	//����1���յ����ݱ�־
u8 Uart1_Get_Data;	//����1���յ�����

u8 Uart2_Get_Flag;	//����2���յ����ݱ�־
u8 Uart2_Get_Data[10];	//����2���յ�����

void USART_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;
	
//	//ʹ�ܴ���1��PA��AFIO����
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
//            RCC_APB2Periph_AFIO |
//            RCC_APB2Periph_USART1 , 
//            ENABLE);

//    /* A9 USART1_Tx */
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�������-TX
//    GPIO_Init(GPIOA, &GPIO_InitStructure);

//    /* A10 USART1_Rx  */
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������-RX
//    GPIO_Init(GPIOA, &GPIO_InitStructure);


//    USART_InitStructure.USART_BaudRate = 115200;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	
//	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
//	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
//	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
//	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

//	USART_ClockInit(USART1, &USART_ClockInitStructure);
//    USART_Init(USART1, &USART_InitStructure);
//    /* Enable the USARTx */
//    USART_Cmd(USART1, ENABLE);
//	//����1ʹ�ý����ж�
//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);

//----------------------------------------------------------------------------------	
	//ʹ�ܴ���2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	// A2 ��T2X
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // A3 ��R2X 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

    USART_ClockInit(USART2, &USART_ClockInitStructure);
    USART_Init(USART2, &USART_InitStructure);
    
    USART_Cmd(USART2, ENABLE);
	//����2ʹ�ý����ж�
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
//----------------------------------------------------------------------------------	
	//ʹ�ܴ���3ʱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
//	
//	// B10 ��T3X
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);

//    // B11 ��R3X 
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
//	USART_InitStructure.USART_BaudRate = 115200;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	
//	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
//	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
//	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
//	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

//    USART_ClockInit(USART3, &USART_ClockInitStructure);
//    USART_Init(USART3, &USART_InitStructure);
//    
//    USART_Cmd(USART3, ENABLE);
//	//����3ʹ�ý����ж�
//	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
//----------------------------------------------------------------------------------	
	//ʹ�ܴ���4ʱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
//	
//	// C10 ��T4X
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init(GPIOC, &GPIO_InitStructure);

//    // C11 ��R4X 
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_Init(GPIOC, &GPIO_InitStructure);
//	
//	USART_InitStructure.USART_BaudRate = 9600;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	
//	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
//	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
//	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
//	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

//    USART_ClockInit(UART4, &USART_ClockInitStructure);
//    USART_Init(UART4, &USART_InitStructure);
//    
//    USART_Cmd(UART4, ENABLE);
//	//����4ʹ�ý����ж�
//	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
}

void USART1_Putc(unsigned char c)
{
    USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}

void USART1_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(USART1, *str++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}

void USART2_Putc(unsigned char c)
{
    USART_SendData(USART2, c);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
}

void USART2_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(USART2, *str++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }

}

void USART3_Putc(unsigned char c)
{
    USART_SendData(USART3, c);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
}

void USART3_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(USART3, *str++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    }

}

void USART3_HEX(u8 *str, u8 len)
{
    while(len--)
    {
        USART_SendData(USART3, *str++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    }
}

void UART4_Putc(unsigned char c)
{
    USART_SendData(UART4, c);
    while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET );
}

void UART4_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(UART4, *str++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
    }

}
