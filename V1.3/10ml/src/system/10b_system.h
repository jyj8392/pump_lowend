/*********************************************************************************************************
**                           �弶���ú�������
********************************************************************************************************/
void GPIO_Configuration(void);			//GPIO
void RCC_Configuration(void);			//RCC
void Tim1_Configuration(void);			//PWM
void USART_Configuration(void);			//USART
void TIM_Configuration(void);			//TIM
void NVIC_Configuration(void);			//NVIC
void ADC_Configuration(void);
void DMA_Configuration(void);
void SPI_Configuration(void);
void Rev_Enable(FunctionalState en);

/*********************************************************************************************************
**                         	�弶���ܺ�������
********************************************************************************************************/
void SendSPIData(char * str);
void SendSPI(u8 data);
void ReceiveSPIData(void);
u32 ReceiveSPILong(void);
#define _delay(pp)	OSTimeDly(pp);

/*********************************************************************************************************
**                          ϵͳ����������
********************************************************************************************************/
void TaskInit(void *pdata);					//TaskInit ��ʼ������
void TaskKey(void *pdata);					//TaskKey ��������
void TaskBZ(void *pdata);					//TaskBZ ��������
void TaskLCD(void *pdata);					//TaskLCD ��������
void TaskTS(void *pdata);					//TaskTS ��������
void TaskMain(void *pdata);					//TaskMain ��������

//==============================================================================================
// �����ջ�ռ���䣬��λ��4�ֽ�
#define	TaskStkInitLengh	400			//Define the TaskInit stack length �����û�����Ķ�ջ����
#define	TaskStkKeyLengh		400			//Define the TaskKey stack length �����û�����Ķ�ջ����
#define	TaskStkBZLengh		400			//Define the TaskBZ stack length �����û�����Ķ�ջ����
#define	TaskStkLCDLengh		400			//Define the TaskLCD stack length �����û�����Ķ�ջ����
#define	TaskStkTSLengh		400			//Define the TaskTS stack length �����û�����Ķ�ջ����
#define	TaskStkMainLengh	400			//Define the TaskMain stack length �����û�����Ķ�ջ����
//==============================================================================================
// �������ȼ�����
#define	PRI_Init			7
#define	PRI_Key				10
#define	PRI_BZ				18
#define	PRI_LCD				6
#define	PRI_TS				14
#define	PRI_Main			15

#define	PRI_Stick			20
#define	PRI_Freeze			21
#define	PRI_Thaw			22
//==============================================================================================
//extern OS_STK TaskStkBZ[TaskStkBZLengh];			//Define the TaskStkBZ stack
extern OS_EVENT *LcdMbox;		//�洢��ֵ����Ϣ����

/*********************************************************************************************************
**                           ��Ĥ����ӳ��
********************************************************************************************************/
#define ISKEY1()			(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12))	   
#define ISKEY2()			(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11))	 
#define ISKEY3()			(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10))		 
#define ISKEY4()			(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9))		
#define ISKEY5()			(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8))		
#define ISKEY6()			(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))		
#define ISKEY7()			(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8))		
#define ISKEY8()			(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7))		
#define ISKEY9()			(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6))		
#define ISKEY10()			(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15))		
#define ISKEY11()			(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14))		
#define ISKEY12()			(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13))	
#define ISKEY_precold()		(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12))		
#define ISKEY_enhauce()		(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11))	 
#define ISKEY_exhaust()		(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10))	 
#define ISKEY_try()			(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9))	 
#define ISKEY_stick()		(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8))	 
#define ISKEY_freeze()		(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15))	 
#define ISKEY_thaw()		(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14))	 
#define ISKEY_stop()		(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13))	 
#define ISKEY_up()			(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))	 
#define ISKEY_down()		(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11))	 

#define KEYVal_no1			1
#define KEYVal_no2			2
#define KEYVal_no3			3
#define KEYVal_no4			4
#define KEYVal_no5			5
#define KEYVal_no6			6
#define KEYVal_no7			7
#define KEYVal_no8			8
#define KEYVal_no9			9
#define KEYVal_no10			10
#define KEYVal_no11			11
#define KEYVal_no12			12
#define KEYVal_no(n)		n

#define KEYVal_precold		13
#define KEYVal_enhance		14
#define KEYVal_exhaust		15
#define KEYVal_try			16
#define KEYVal_stick		17
#define KEYVal_freeze		18
#define KEYVal_thaw			19
#define KEYVal_stop			20

#define KEYVal_up			21
#define KEYVal_down			22

/*********************************************************************************************************
**                           ����LEDӳ��
********************************************************************************************************/
#define ON				1
#define OFF				0
#define LED1			GPIOE, GPIO_Pin_0
#define LED2			GPIOB, GPIO_Pin_9
#define LED3			GPIOB, GPIO_Pin_8
#define LED4			GPIOB, GPIO_Pin_7
#define LED5			GPIOB, GPIO_Pin_6
#define LED6			GPIOB, GPIO_Pin_5
#define LED7			GPIOD, GPIO_Pin_7
#define LED8			GPIOD, GPIO_Pin_6
#define LED9			GPIOD, GPIO_Pin_5
#define LED10			GPIOD, GPIO_Pin_4
#define LED11			GPIOD, GPIO_Pin_3
#define LED12			GPIOD, GPIO_Pin_2
#define LED13			GPIOD, GPIO_Pin_1
#define LED14			GPIOD, GPIO_Pin_0
#define LED15			GPIOC, GPIO_Pin_12
#define LED16			GPIOC, GPIO_Pin_11
#define LED17			GPIOC, GPIO_Pin_10
#define LED18			GPIOE, GPIO_Pin_2
#define LED19			GPIOE, GPIO_Pin_3
#define LED20			GPIOE, GPIO_Pin_4
#define LED21			GPIOE, GPIO_Pin_5
#define LED22			GPIOE, GPIO_Pin_6

#define Led(sw, n)		(sw == OFF? GPIO_SetBits(LED##n): GPIO_ResetBits(LED##n))
#define SetLed(sw, n)	Led(sw, n)

/*********************************************************************************************************
**                           ϵͳ���Ʊ���
********************************************************************************************************/

struct sys_ctl {
	u8 portSelected[12];	//ѡ�ж˿ں�
	u8 precoldState;		//Ԥ��״̬��־
	u8 enhanceState;		//����״̬��־
	u8 exhaustState;		//����״̬��־

	u16 bzOnTime;			//����������ʱ��
	u16 bzOffTime;			//�������ض�ʱ��
	u8 bzRepts;				//�������ظ�����
	
	u8 slot[12];
	u8 portExist[12];		//�˿ڴ���
	u8 portState[12];		//�˿ڿ���״̬
	u8 runTime_Min[12];		//����ʱ��(��)
	u8 runTime_Sec[12];		//����ʱ��(��)
	u8 freezePower[12];		//�䶳����
	
	float temperature[24];
	float pressure[8];
	
	u8 nextInterface;			//������
};

#define	PRECOLDTEMP			(13 - 1)

#define READY				0
#define TRY					1
#define STICK				2
#define FREEZE				3
#define THAW				4
#define ISSELECTED			1
#define INSERT				1



struct spiStruct {
	u8 frameStart;
	u16 port		:4;
	u16 state		:3;
	u16 exist		:1;
	u8 min;	
	u8 sec;		
	float temperature;
	float pressure;		//port,state,existΪ0ʱ��min,sec��ʾѹ�����
	u8 power;
	u16 select		:1;
	u16 precold		:1;
	u16 enhance		:1;
	u16 exhaust		:1;
	u8 RESERVED1	:4;
	u8 RESERVED2;
	u8 frameEnd;
};

struct spi_ctl {
#define SPISTARTBYTE	0xA5
#define SPIENDBYTE		0xA6
	u16 ovTimeCnt;
	u16 sendBusy;
	u16 recvBusy;
	u8 byteLen;
	u8 bitLen;
	u8 byte;
	u8 CRCValue[13];
	
	struct sys_ctl *revPtr;
	u8 *sendPtr;
	union state {			//ϵͳ״̬���밴����ͨ�ţ�����
		struct spiStruct _sysState;
		u8 __sysState[16];
	} sysState;
};
//extern struct spi_ctl spiCtl;

/*********************************************************************************************************
**                           Ӧ�ú�������
********************************************************************************************************/
#define SendSPIStr(str)	SendSPIData(str)
void SendKeyValue(u8 val, struct sys_ctl *sysctl);
bool ISPortSelected(struct sys_ctl *sysctl);
bool ISExistInSlot(u8 chn, struct sys_ctl *sysctl);
u8 GetCRC(u8 *data, u8 len);

/*********************************************************************************************************
**                          ���溯������
********************************************************************************************************/
void interface0(void);					//��������
void interfaceOK(void);
void addPort(u8 chn, struct sys_ctl *sysctl);
void clrPort(u8 chn, struct sys_ctl *sysctl);
void showPortState(struct sys_ctl *sysctl);

/*********************************************************************************************************
**                          ͨ��Э��˵��
**	����		x		 				����		  x1              	 x2		  x3     x4~7	x8~11	x12	   x13	  					  x14
**			bbbb bbbb 							bbbb bbb        b	  										   b    b	 b	  b    bbbb
**		�˿ں�   ���ܰ���					  �˿ں� ״̬     ����    ʱ��(��) ʱ��(��)  �¶�	ѹ��    ����   ѡ�� Ԥ�� ���� ���� Ԥ��   Ԥ��
**		1~   	 0 ��ѡ��					  1~12   0 ���� 0   ����
**		12		 1   ѡ��							 1 �Ե� 1 ������
**			 	 2   �Ե�							 2 �̶�
**				 3   �̶�							 3 �䶳
**				 4   �䶳							 4 ����
**				 5   ����
**				 6   ֹͣ					
**				 7	 Ԥ��
**				 8   ����
**				 9   ����
**				10 ���ʼ�
**				11 ���ʼ�
********************************************************************************************************/

extern OS_EVENT *KeyMbox;