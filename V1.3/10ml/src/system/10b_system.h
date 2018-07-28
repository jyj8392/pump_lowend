/*********************************************************************************************************
**                           板级配置函数声明
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
**                         	板级功能函数声明
********************************************************************************************************/
void SendSPIData(char * str);
void SendSPI(u8 data);
void ReceiveSPIData(void);
u32 ReceiveSPILong(void);
#define _delay(pp)	OSTimeDly(pp);

/*********************************************************************************************************
**                          系统任务函数声明
********************************************************************************************************/
void TaskInit(void *pdata);					//TaskInit 初始化任务
void TaskKey(void *pdata);					//TaskKey 键盘任务
void TaskBZ(void *pdata);					//TaskBZ 键盘任务
void TaskLCD(void *pdata);					//TaskLCD 键盘任务
void TaskTS(void *pdata);					//TaskTS 键盘任务
void TaskMain(void *pdata);					//TaskMain 键盘任务

//==============================================================================================
// 任务堆栈空间分配，单位：4字节
#define	TaskStkInitLengh	400			//Define the TaskInit stack length 定义用户任务的堆栈长度
#define	TaskStkKeyLengh		400			//Define the TaskKey stack length 定义用户任务的堆栈长度
#define	TaskStkBZLengh		400			//Define the TaskBZ stack length 定义用户任务的堆栈长度
#define	TaskStkLCDLengh		400			//Define the TaskLCD stack length 定义用户任务的堆栈长度
#define	TaskStkTSLengh		400			//Define the TaskTS stack length 定义用户任务的堆栈长度
#define	TaskStkMainLengh	400			//Define the TaskMain stack length 定义用户任务的堆栈长度
//==============================================================================================
// 任务优先级分配
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
extern OS_EVENT *LcdMbox;		//存储键值的消息邮箱

/*********************************************************************************************************
**                           薄膜键盘映射
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
**                           按键LED映射
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
**                           系统控制变量
********************************************************************************************************/

struct sys_ctl {
	u8 portSelected[12];	//选中端口号
	u8 precoldState;		//预冷状态标志
	u8 enhanceState;		//给力状态标志
	u8 exhaustState;		//排气状态标志

	u16 bzOnTime;			//蜂鸣器开启时间
	u16 bzOffTime;			//蜂鸣器关断时间
	u8 bzRepts;				//蜂鸣器重复次数
	
	u8 slot[12];
	u8 portExist[12];		//端口存在
	u8 portState[12];		//端口开启状态
	u8 runTime_Min[12];		//运行时间(分)
	u8 runTime_Sec[12];		//运行时间(秒)
	u8 freezePower[12];		//冷冻功率
	
	float temperature[24];
	float pressure[8];
	
	u8 nextInterface;			//界面编号
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
	float pressure;		//port,state,exist为0时，min,sec表示压力序号
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
	union state {			//系统状态（与按键板通信）上行
		struct spiStruct _sysState;
		u8 __sysState[16];
	} sysState;
};
//extern struct spi_ctl spiCtl;

/*********************************************************************************************************
**                           应用函数声明
********************************************************************************************************/
#define SendSPIStr(str)	SendSPIData(str)
void SendKeyValue(u8 val, struct sys_ctl *sysctl);
bool ISPortSelected(struct sys_ctl *sysctl);
bool ISExistInSlot(u8 chn, struct sys_ctl *sysctl);
u8 GetCRC(u8 *data, u8 len);

/*********************************************************************************************************
**                          界面函数声明
********************************************************************************************************/
void interface0(void);					//开机界面
void interfaceOK(void);
void addPort(u8 chn, struct sys_ctl *sysctl);
void clrPort(u8 chn, struct sys_ctl *sysctl);
void showPortState(struct sys_ctl *sysctl);

/*********************************************************************************************************
**                          通信协议说明
**	发送		x		 				接收		  x1              	 x2		  x3     x4~7	x8~11	x12	   x13	  					  x14
**			bbbb bbbb 							bbbb bbb        b	  										   b    b	 b	  b    bbbb
**		端口号   功能按键					  端口号 状态     存在    时间(分) 时间(秒)  温度	压力    功率   选中 预冷 给力 排气 预留   预留
**		1~   	 0 不选择					  1~12   0 就绪 0   存在
**		12		 1   选择							 1 试刀 1 不存在
**			 	 2   试刀							 2 固定
**				 3   固定							 3 冷冻
**				 4   冷冻							 4 复温
**				 5   复温
**				 6   停止					
**				 7	 预冷
**				 8   给力
**				 9   排气
**				10 功率加
**				11 功率减
********************************************************************************************************/

extern OS_EVENT *KeyMbox;
