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
void TaskPUMP(void *pdata);					//TaskLCD 键盘任务
void TaskPC(void *pdata);					//TaskPC 键盘任务
void TaskMain(void *pdata);					//TaskMain 键盘任务

//==============================================================================================
// 任务堆栈空间分配，单位：4字节
#define	TaskStkInitLengh	400			//Define the TaskInit stack length 定义用户任务的堆栈长度
#define	TaskStkKeyLengh		400			//Define the TaskKey stack length 定义用户任务的堆栈长度
#define	TaskStkBZLengh		400			//Define the TaskBZ stack length 定义用户任务的堆栈长度
#define	TaskStkPUMPLengh	400			//Define the TaskLCD stack length 定义用户任务的堆栈长度
#define	TaskStkPCLengh		400			//Define the TaskTS stack length 定义用户任务的堆栈长度
#define	TaskStkMainLengh	400			//Define the TaskMain stack length 定义用户任务的堆栈长度
//==============================================================================================
// 任务优先级分配
#define	PRI_Init			7
#define	PRI_Key				10
#define	PRI_BZ				3
#define	PRI_PUMP			6
#define	PRI_PC				4
#define	PRI_Main			15

//==============================================================================================
//extern OS_STK TaskStkBZ[TaskStkBZLengh];			//Define the TaskStkBZ stack
extern OS_EVENT *LcdMbox;		//存储键值的消息邮箱

/*********************************************************************************************************
**                           蜂鸣器映射
********************************************************************************************************/
#define BZ			GPIOC,GPIO_Pin_13	   
#define SetBzON()	GPIO_SetBits(BZ);
#define SetBzOFF()	GPIO_ResetBits(BZ);
/*********************************************************************************************************
**                          步进电机映射
********************************************************************************************************/
#define MSTEP		GPIOC,GPIO_Pin_4	   
#define SetMSTEPON()	GPIO_ResetBits(MSTEP);
#define SetMSTEPOFF()	GPIO_SetBits(MSTEP);
/*********************************************************************************************************
**                           薄膜键盘映射
********************************************************************************************************/
#define IO1			GPIOC, GPIO_Pin_10	   
#define IO2			GPIOC, GPIO_Pin_11	 
#define IO3			GPIOC, GPIO_Pin_12		 
#define IO4			GPIOB, GPIO_Pin_5		
#define IO5			GPIOB, GPIO_Pin_6		
#define IO6			GPIOB, GPIO_Pin_7		
#define IO7			GPIOB, GPIO_Pin_8		
#define IO8			GPIOB, GPIO_Pin_9

#define PLCRUN		GPIOA, GPIO_Pin_7

#define ScanCol1()	do{GPIO_ResetBits(IO1);GPIO_SetBits(IO2);GPIO_SetBits(IO3);GPIO_SetBits(IO4);}while(0)						
#define ScanCol2()	do{GPIO_SetBits(IO1);GPIO_ResetBits(IO2);GPIO_SetBits(IO3);GPIO_SetBits(IO4);}while(0)
#define ScanCol3()	do{GPIO_SetBits(IO1);GPIO_SetBits(IO2);GPIO_ResetBits(IO3);GPIO_SetBits(IO4);}while(0)
#define ScanCol4()	do{GPIO_SetBits(IO1);GPIO_SetBits(IO2);GPIO_SetBits(IO3);GPIO_ResetBits(IO4);}while(0)							

#define ISKEYNUM1	(!GPIO_ReadInputDataBit(IO5))
#define ISKEYNUM2	(!GPIO_ReadInputDataBit(IO5))
#define ISKEYNUM3	(!GPIO_ReadInputDataBit(IO5))
#define ISKEYNUM4	(!GPIO_ReadInputDataBit(IO6))
#define ISKEYNUM5	(!GPIO_ReadInputDataBit(IO6))
#define ISKEYNUM6	(!GPIO_ReadInputDataBit(IO6))
#define ISKEYNUM7	(!GPIO_ReadInputDataBit(IO6))
#define ISKEYNUM8	(!GPIO_ReadInputDataBit(IO7))
#define ISKEYNUM9	(!GPIO_ReadInputDataBit(IO7))
#define ISKEYNUM0	(!GPIO_ReadInputDataBit(IO5))
#define ISKEYL		(!GPIO_ReadInputDataBit(IO8))
#define ISKEYP		(!GPIO_ReadInputDataBit(IO8))
#define ISKEYSTART	(!GPIO_ReadInputDataBit(IO8))
#define ISKEYSTOP	(!GPIO_ReadInputDataBit(IO8))
#define ISKEYUP		(!GPIO_ReadInputDataBit(IO7))
#define ISKEYDOWN	(!GPIO_ReadInputDataBit(IO7))

#define ISPLCRUN	(GPIO_ReadInputDataBit(PLCRUN))


#define KEYNUM1		1
#define KEYNUM2		2
#define KEYNUM3		3
#define KEYNUM4		4
#define KEYNUM5		5
#define KEYNUM6		6
#define KEYNUM7		7
#define KEYNUM8		8
#define KEYNUM9		9
#define KEYNUM0		10
#define KEYUP		11
#define KEYDOWN		12
#define KEYL		13
#define KEYP		14
#define KEYSTOP		15
#define KEYSTART	16
/*********************************************************************************************************
**                           按键LED映射
********************************************************************************************************/
//#define ON				1
//#define OFF				0
//#define LED1			GPIOE, GPIO_Pin_0
//#define LED2			GPIOB, GPIO_Pin_9
//#define LED3			GPIOB, GPIO_Pin_8
//#define LED4			GPIOB, GPIO_Pin_7
//#define LED5			GPIOB, GPIO_Pin_6
//#define LED6			GPIOB, GPIO_Pin_5
//#define LED7			GPIOD, GPIO_Pin_7
//#define LED8			GPIOD, GPIO_Pin_6
//#define LED9			GPIOD, GPIO_Pin_5
//#define LED10			GPIOD, GPIO_Pin_4
//#define LED11			GPIOD, GPIO_Pin_3
//#define LED12			GPIOD, GPIO_Pin_2
//#define LED13			GPIOD, GPIO_Pin_1
//#define LED14			GPIOD, GPIO_Pin_0
//#define LED15			GPIOC, GPIO_Pin_12
//#define LED16			GPIOC, GPIO_Pin_11
//#define LED17			GPIOC, GPIO_Pin_10
//#define LED18			GPIOE, GPIO_Pin_2
//#define LED19			GPIOE, GPIO_Pin_3
//#define LED20			GPIOE, GPIO_Pin_4
//#define LED21			GPIOE, GPIO_Pin_5
//#define LED22			GPIOE, GPIO_Pin_6

//#define Led(sw, n)		(sw == OFF? GPIO_SetBits(LED##n): GPIO_ResetBits(LED##n))
//#define SetLed(sw, n)	Led(sw, n)

///*********************************************************************************************************
//**                           泵校准系数
//********************************************************************************************************/
struct pumpflow_t {
	u16 index;
	u16 cnt;
};
extern struct pumpflow_t const PUMP_FlowPara[10];
struct pumppress_t {
	u16 pg;
	u16 pz;
};
/*********************************************************************************************************
**                           系统控制变量
********************************************************************************************************/
#define WORKING				0
#define SETTINGFLOW			1
#define SETTINGPRESSURE		2
#define PURGING				3
#define FLOWPARAADJ			4
#define PARARESETING		5

#define PURGETIME			(3 * 60)

struct sys_ctl {
	u16 bzOnTime;			//蜂鸣器开启时间
	u16 bzOffTime;			//蜂鸣器关断时间
	u8 bzRepts;				//蜂鸣器重复次数
	
		
	u8 interface;			//界面编号
	
	u8 controlMode;
	u8 pumpMode;
	u8 flowMode;
	u16 pressMax;
	u16 pressMin;
	u16 purgeFlow;
	u8 backLumin;
	u8 keyBeep;
	
	u16 setFlow;
	s16 pressOffset;
	s16 nowPress;
	u8 sendPressInterval;
	
	u8 pressProtect;
	
};

struct pumpctl_t {
	struct sys_ctl *sysctl;
	
	u8 nowState;
	double nowInterval;
	u16 dstInterval;
	
	double startFlow;
	u32 startTime;
	double dstFlow;
	u32 dstTime;
	double nowFlow;
	u32 nowTime;
	u8 isPIDStart;
	
	double calCnt;
	double accCnt;
	double dCndMax;
	u32 accTime;
	
	u32 maxTime;
	
	struct pumpflow_t flowPara[10];
};
extern struct pumpctl_t PumpCtl;

struct PID {
	double SetPoint;    //  ???? Desired Value
	double Kp;          //  ???? Kp
	double Ki;          //  ???? Ki
	double Kd;          //  ???? Kd
	u32 DeadZone;

	double LastError;          //  Error[-1]
	double PrevError;          //  Error[-2]
	double SumError;           //  Sums of Errors
};
extern struct PID Cnt_PID;



struct uart2data_t {
	u8 STX;
	u8 ID;
	u8 AI;
	u8 PFC;
	u32 VALUE;
	u16 CRCCode;
	u8 ETX;
	
	u8 revLen;
	char revData[16];
};
extern struct uart2data_t frameData;
/************串口通信*************/
#define ACK		0x23
#define NACK	0x24
#define WAIT	0x25

/************设备信息*************/
#define EQUID		12
	#define MAXFLOW		100
	#define MAXPRESSURE	5
#define EQUSERNO	"012345"
#define SOFTSTART	0

/************压力参数*************/
#define PRESS_DEBUG		0

#define PRESSPARA_S1_X4	0
#define PRESSPARA_S1_X3	0
#define PRESSPARA_S1_X2	0
#define PRESSPARA_S1_X1	0.0007
#define PRESSPARA_S1_X0	0.05519
#define PRESSPARA_S_SPLITE	200
#define PRESSPARA_S2_X2	0
#define PRESSPARA_S2_X1	0.0014
#define PRESSPARA_S2_X0	(-0.0571)

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
void USART2_Puts(char * str);
void USART2_Putc(unsigned char c);

void LCD_Initial(void);
void Clear_Screen(void);
void ShowStr(unsigned char row, unsigned char column,
	char str[], unsigned char back);
unsigned int GetPressure(void);
unsigned int GetFLOWPLC(void);
void GetStep(void);
void SetStep(void);

void PumpSetFlow(u16 setFlow);
void PumpStart(u16 setFlow);
void PumpPuge(u16 setFlow);
void PumpStop(void);
u16 GetFlowPara(double flow);
u16 SetFlowPara(u16 flow, u16 curflow);

void SetPulse(void);
void ShowBlock(void);
void ShowASCII816Str(unsigned char x,unsigned char y,char str[], unsigned char back);
void ShowHZ1616Str(unsigned char x,unsigned char y,char str[], unsigned char back);
void ShowASCII1624Str(unsigned char x,unsigned char y,char str[], unsigned char back);
void ScanKey(void);

void PID_Init(double kp, double ki, double kd, u32 deadzone, double setpoint);
double PID_Calc(struct PID *pp, double NextPoint);

short int getFFT(short int *a);

void Read_Flash(struct sys_ctl *sysctl);
void Write_Flash(struct sys_ctl *sysctl);

/*********************************************************************************************************
**                          界面函数声明
********************************************************************************************************/



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
