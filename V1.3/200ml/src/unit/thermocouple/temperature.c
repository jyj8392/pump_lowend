#include "include.h"

extern  u16 ADC_RCVTab[120];			//AD采样数据缓冲区

u16 Tem1[10],Tem2[10],Tem3[10],Tem4[10],Tem5[10],Tem6[10],Tem7[10],Tem8[10],Tem9[10];

 //电阻箱调节
 u16 const DianZ[]={527,812,1093,1375,1655,1932,2209,2485,2761,3036,3308,3585,3862};


u16  const T_TABLT1[]={
0,39,77,116,154,193,231,269,307,345,                                  //;0~-9c
383,421,458,496,534,571,608,646,683,720,                              //;-10~-19c
757,794,830,867,903,940,976,1013,1049,1085,                           //;-20~-29c
1121,1157,1192,1228,1263,1299,1334,1370,1405,1440,                    //;-30~-39c
1475,1510,1544,1579,1614,1648,1682,1717,1751,1785,                    //;-40~-49c
1819,1853,1886,1920,1953,1987,2020,2053,2087,2120,                    //;-50~-59c
2152,2185,2218,2250,2283,2315,2348,2380,2412,2444,                    //;-60~-69c
2475,2507,2539,2570,2602,2633,2664,2695,2726,2757,                    //;-70~-79c
2788,2818,2849,2879,2909,2939,2970,2999,3029,3059,                    //;-80~-89c
3089,3118,3147,3177,3206,3235,3264,3293,3321,3350,                    //;-90~-99c
3378,3407,3435,3463,3491,3519,3547,3574,3602,3629,                     //;-100~-109c	   
3657,3684,3712,3740,3767,3794,3821,3849,3876,3902,					   //-110~-119   smart ++
3929,3956,3983,4009,4035,4062,4088,4114,4140,4166,					   //-120~-129 
4192,4217,4243,4268,4294,4319,4344,4369,4394,4419,					   //-130~-139
4444,4469,4493,4518,4542,4567,4591,4615,4639,4663,					   //-140~-149
4687,4710,4734,4758,4781,4804,4827,4851,4874,4896,
4919,4942,4965,4987,5009,5062,5054,5076,5098,5120,
5142,5163,5185,5206,5228,5249,5270,5291,5312,5333,
5354,5375,5395,5416,5436,5457,5477,5497,5517,5537,
5557,5576,5596,5615,5635,5654,5673,5692,5711,5730,						//-190~-199
/***********************************************************/
5750,5769,5788,5807,5826,5845,5864,5883,5902,5921,						//-200~-209
5941,5960,5979,5998,6017,6036,6055,6074,6093,6112,						//-210~-219
6131,6150,6169,6188,6207,6245,6264,6283,6302,6321,						//-220~-229
6340,6359,6378,6397,6416,6435,6454,6473,6492,6511,						//-230~-239
6530,6549,6568,6587,6606,6625,6644,6663,6682,6701,						//-240~-249
6720,6739,6758,6777,6796,6815						//-250~-255
};	
//            ORG 2100H
//;T TYPE THERMOCOUPLE POSITIVE TEMPERATURE
//;THE DATA IN THE TABLT IS POSITIVE NUMBER
u16 const T_TABLT2[]={
0,39,78,117,156,195,234,273,312,351,                          //;0~9c
391,430,470,510,549,589,629,669,709,749,                      //;10~19c
789,830,870,911,951,992,1032,1073,1114,1155,                  //;20~29c
1196,1237,1279,1320,1361,1403,1444,1486,1528,1569,            //;30~39c
1611,1653,1695,1738,1780,1822,1865,1907,1950,1992,            //;40~49c
2035,2078,2121,2164,2207,2250,2294,2337,2380,2424,            //;50~59c
2467,2511,2555,2599,2643,2687,2731,2775,2819,2864,            //;60~69c
2908,2953,2997,3042,3087,3131,3176,3221,3266,3312,            //;70~79c
3357,3402,3447,3493,3538,3584,3630,3676,3721,3767,            //;80~89c
3813,3859,3906,3952,3998,4044,4091,4137,4184,4231,            //;90~99c
4277,4324,4371,4418,4465,4512,4559,4607,4654,4701,            //;100~109c
4749,4796,4844,4891,4939,4987,5035,5083,5131,5179,            //;110~119c
5227,5275,5324,5372,5420,5469,5517,5566,5615,5663,            //;120~129c
5712,5761,5810,5859,5908,5957,6007,6056,6105,6155,            //;130~139c
6204,6254,6303,6353,6403,6452,6502,6552,6602,6652,            //;140~149c
6702,6753,6803,6853,6903,6954,7004,7055,7106,7156,            //;150~159c
7207,7258,7309,7360,7411,7462,7513,7564,7615,7666,            //;160~169c
7718,7769,7821,7872,7924,7975,8027,8079,8131,8183,            //;170~179c
8235,8287,8339,8391,8443,8495,8548,8600,8652,8705,            //;180~189c
8757,8810,8863,8915,8968,9021,9074,9127,9180,9233,            //;190~199c
9286,9339,9392,9446,9499,9553,9606,9659,9713,9767};            //;200~209c

#define x1	0.5
#define x2	0.5

/***************************************************************************************************************************/
void bubble_sort(u16 *x,u16 n)
  {
   u16 j, k, h, t;

     for (h=n-1; h>0; h=k) /*循环到没有比较范围*/
    {
       for (j=0, k=0; j<h; j++) /*每次预置k=0，循环扫描后更新k*/
         {
            if (*(x+j) > *(x+j+1)) /*大的放在后面，小的放到前面*/
            {
             t = *(x+j);
             *(x+j) = *(x+j+1);
             *(x+j+1) = t; /*完成交换*/
             k = j; /*保存最后下沉的位置。这样k后面的都是排序排好了的。*/
            }
         }
      }
  }

/******************************************************************************************************************/
u16 ADCData1(void)
{
   u8 i;
  
   u32 TEMP1=0,TEMP2=0;
static u16 adcdata1;
static u16 ThCp1[10]; static u8 N1;
 
   for(i=0;i<10;i++)
   {
     Tem1[i]=ADC_RCVTab[0+i*10];
   }  
   	bubble_sort(Tem1,10);	 
	for(i=3;i<8;i++)
	{
	 TEMP1+=Tem1[i]; 
	}
//-------------------------------
    if(N1>10) N1=0;
	ThCp1[N1++]=TEMP1/5;
	for(i=0;i<10;i++)  TEMP2+=ThCp1[i];

//-------------------------------
   // adcdata1=0.9*adcdata1+0.1*(TEMP1/5);
	  adcdata1=x2*adcdata1+x1*(TEMP2/10);

	return adcdata1;  
/*	u8 i;
	static u16 adcdata1 = 0;
		for (i = 0; i < 10; ++i) {
			Tem1[i] = ADC_RCVTab[0 + i * 12];
		}
		bubble_sort(Tem1, 10);
	adcdata1 = 0.9 * (float)adcdata1 + 0.1 * (float)Tem1[5];
	return adcdata1;*/
}
/***************************************************************************************************************/
 u16 ADCData2(void)
{
   u8 i;
  
   u32 TEMP1=0,TEMP2=0;
static u16 adcdata2;
static u16 ThCp2[10]; static u8 N2;
 
   for(i=0;i<10;i++)
   {
     Tem2[i]=ADC_RCVTab[1+i*10];
   }  
   	bubble_sort(Tem2,10);	 
	for(i=3;i<8;i++)
	{
	 TEMP1+=Tem2[i]; 
	}
//-------------------------------
    if(N2>10) N2=0;
	ThCp2[N2++]=TEMP1/5;
	for(i=0;i<10;i++)  TEMP2+=ThCp2[i];

//-------------------------------
   // adcdata2=0.9*adcdata2+0.1*(TEMP1/5);
	  adcdata2=x2*adcdata2+x1*(TEMP2/10);
	 
/*	u8 i;
	static u16 adcdata2 = 0;
		for (i = 0; i < 10; ++i) {
			Tem2[i] = ADC_RCVTab[1 + i * 10];
		}
		bubble_sort(Tem2, 10);
	adcdata2 = 0.9 * (float)adcdata2 + 0.1 * (float)Tem2[5];	*/
	return adcdata2;
}
/********************************************************************************************************/
   u16 ADCData3(void)
 {
   u8 i;
  
   u32 TEMP1=0,TEMP2=0;
   static u16 adcdata3;
   static u16 ThCp3[10]; static u8 N3;
 
   for(i=0;i<10;i++)
   {
     Tem3[i]=ADC_RCVTab[2+i*10];
   }  
   	bubble_sort(Tem3,10);	 
	for(i=3;i<8;i++)
	{
	 TEMP1+=Tem3[i]; 
	}
//-------------------------------
    if(N3>10) N3=0;
	ThCp3[N3++]=TEMP1/5;
	for(i=0;i<10;i++)  TEMP2+=ThCp3[i];

//-------------------------------
    //adcdata3=0.9*adcdata3+0.1*(TEMP1/5);
	  adcdata3=x2*adcdata3+x1*(TEMP2/10);
  
/*	u8 i;
	static u16 adcdata3 = 0;
		for (i = 0; i < 10; ++i) {
			Tem3[i] = ADC_RCVTab[2 + i * 10];
		}
		bubble_sort(Tem3, 10);
	adcdata3 = 0.9 * (float)adcdata3 + 0.1 * (float)Tem3[5];	*/
	  return adcdata3;
  }
/********************************************************************************************************/
   u16 ADCData4(void)
  {
     u8 i;
  
     u32 TEMP1=0,TEMP2=0;
     static u16 adcdata4;
     static u16 ThCp4[10]; static u8 N4;
 
     for(i=0;i<10;i++)
     {
       Tem4[i]=ADC_RCVTab[3+i*10];
     }  
     bubble_sort(Tem4,10);	 
	 for(i=3;i<8;i++)
	 {
	   TEMP1+=Tem4[i]; 
	 }
//-------------------------------
    if(N4>10) N4=0;
	ThCp4[N4++]=TEMP1/5;
	for(i=0;i<10;i++)  TEMP2+=ThCp4[i];

//-------------------------------
   // adcdata1=0.9*adcdata1+0.1*(TEMP1/5);
	  adcdata4=x2*adcdata4+x1*(TEMP2/10);
	   
/*	u8 i;
	static u16 adcdata4 = 0;
		for (i = 0; i < 10; ++i) {
			Tem4[i] = ADC_RCVTab[3 + i * 10];
		}
		bubble_sort(Tem4, 10);
	adcdata4 = 0.9 * (float)adcdata4 + 0.1 * (float)Tem4[5];	 */
	  return adcdata4;
   }
/********************************************************************************************************/
 u16 ADCData5(void)
{
   u8 i;
  
   u32 TEMP1=0,TEMP2=0;
static u16 adcdata5;
static u16 ThCp5[10]; static u8 N5;
 
   for(i=0;i<10;i++)
   {
     Tem5[i]=ADC_RCVTab[4+i*10];
   }  
   	bubble_sort(Tem5,10);	 
	for(i=3;i<8;i++)
	{
	 TEMP1+=Tem5[i]; 
	}
//-------------------------------
    if(N5>10) N5=0;
	ThCp5[N5++]=TEMP1/5;
	for(i=0;i<10;i++)  TEMP2+=ThCp5[i];

//-------------------------------
   // adcdata1=0.9*adcdata1+0.1*(TEMP1/5);
	  adcdata5=x2*adcdata5+x1*(TEMP2/10);
					
/*	u8 i;
	static u16 adcdata5 = 0;
		for (i = 0; i < 10; ++i) {
			Tem5[i] = ADC_RCVTab[4 + i * 10];
		}
		bubble_sort(Tem5, 10);
	adcdata5 = 0.9 * (float)adcdata5 + 0.1 * (float)Tem5[5];   */
	return adcdata5;
}
//-------------------------------------------------------------------------------------------------------------
/********************************************************************************************************/
 u16 ADCData6(void)
{
   u8 i;
  
   u32 TEMP1=0,TEMP2=0;
static u16 adcdata6;
static u16 ThCp6[10]; static u8 N6;
 
   for(i=0;i<10;i++)
   {
     Tem6[i]=ADC_RCVTab[5+i*10];
   }  
   	bubble_sort(Tem6,10);	 
	for(i=3;i<8;i++)
	{
	 TEMP1+=Tem6[i]; 
	}
//-------------------------------
    if(N6>10) N6=0;
	ThCp6[N6++]=TEMP1/5;
	for(i=0;i<10;i++)  TEMP2+=ThCp6[i];

//-------------------------------
   // adcdata2=0.9*adcdata2+0.1*(TEMP1/5);
	  adcdata6=x2*adcdata6+x1*(TEMP2/10);
/*	u8 i;
	static u16 adcdata6 = 0;
		for (i = 0; i < 10; ++i) {
			Tem6[i] = ADC_RCVTab[5 + i * 10];
		}
		bubble_sort(Tem6, 10);
	adcdata6 = 0.9 * (float)adcdata6 + 0.1 * (float)Tem6[5];  */
	return adcdata6;
}
//-------------------------------------------------------------------------------------------------------------
/********************************************************************************************************/
 u16 ADCData7(void)
{
   u8 i;
  
   u32 TEMP1=0,TEMP2=0;
static u16 adcdata7;
static u16 ThCp7[10]; static u8 N7;
 
   for(i=0;i<10;i++)
   {
     Tem7[i]=ADC_RCVTab[6+i*10];
   }  
   	bubble_sort(Tem7,10);	 
	for(i=3;i<8;i++)
	{
	 TEMP1+=Tem7[i]; 
	}
//-------------------------------
    if(N7>10) N7=0;
	ThCp7[N7++]=TEMP1/5;
	for(i=0;i<10;i++)  TEMP2+=ThCp7[i];

//-------------------------------
   // adcdata2=0.9*adcdata2+0.1*(TEMP1/5);
	  adcdata7=x2*adcdata7+x1*(TEMP2/10);
/*	u8 i;
	static u16 adcdata7 = 0;
		for (i = 0; i < 10; ++i) {
			Tem7[i] = ADC_RCVTab[6 + i * 10];
		}
		bubble_sort(Tem7, 10);
	adcdata7 = 0.9 * (float)adcdata7 + 0.1 * (float)Tem7[5];  */
	return adcdata7;
}
//-------------------------------------------------------------------------------------------------------------
/********************************************************************************************************/
 u16 ADCData8(void)
{
   u8 i;
  
   u32 TEMP1=0,TEMP2=0;
static u16 adcdata8;
static u16 ThCp8[10]; static u8 N8;
 
   for(i=0;i<10;i++)
   {
     Tem8[i]=ADC_RCVTab[7+i*10];
   }  
   	bubble_sort(Tem8,10);	 
	for(i=3;i<8;i++)
	{
	 TEMP1+=Tem8[i]; 
	}
//-------------------------------
    if(N8>10) N8=0;
	ThCp8[N8++]=TEMP1/5;
	for(i=0;i<10;i++)  TEMP2+=ThCp8[i];

//-------------------------------
   // adcdata2=0.9*adcdata2+0.1*(TEMP1/5);
	  adcdata8=x2*adcdata8+x1*(TEMP2/10);
/*	u8 i;
	static u16 adcdata8 = 0;
		for (i = 0; i < 10; ++i) {
			Tem8[i] = ADC_RCVTab[7 + i * 10];
		}
		bubble_sort(Tem8, 10);
	adcdata8 = 0.9 * (float)adcdata8 + 0.1 * (float)Tem8[5]; */
	return adcdata8;
}
//-------------------------------------------------------------------------------------------------------------
  u16 ADCData9(void)
{
   u8 i;
  
   u32 TEMP1=0,TEMP2=0;
static u16 adcdata9;
static u16 ThCp9[10]; static u8 N9;
 
   for(i=0;i<10;i++)
   {
     Tem9[i]=ADC_RCVTab[8+i*10];
   }  
   	bubble_sort(Tem9,10);	 
	for(i=3;i<8;i++)
	{
	 TEMP1+=Tem9[i]; 
	}
//-------------------------------
    if(N9>10) N9=0;
	ThCp9[N9++]=TEMP1/5;
	for(i=0;i<10;i++)  TEMP2+=ThCp9[i];

//-------------------------------
   // adcdata1=0.9*adcdata1+0.1*(TEMP1/5);
	  adcdata9=x2*adcdata9+x1*(TEMP2/10);

	return adcdata9;
}
//---------------------------------------------------------------------------------------------------------------
u16 ADCDataPt1(void)
 {
   u8 i;
   u32 TEMP5=0;
   u16 ADCData;
 static  u16 ADCDataPt;

   for(i=0;i<10;i++)
   {
     Tem5[i]=ADC_RCVTab[9+i*10];
   }
   	bubble_sort(Tem5,10);
//	max=Tem5[20]; min=Tem5[0];
	for(i=3;i<8;i++)
	{
	 TEMP5+=Tem5[i]; 
	}

    ADCDataPt=0.8*ADCDataPt+0.2*(TEMP5/5); 


	ADCData=ADCDataPt;
	return ADCData;
    //return 2120;
 }
 
//----------------------------------------------------------------------------------------------------------------
u16 TemperaturePt100(void)
{
   u8 i,TemPtIndex=0,IndexPt0,IndexPt1;
   u16 max,min;
   u32 TEMP5=0;
   u16 ADCData,TemPt1,Tem;
 
	ADCData=ADCDataPt1();
	for(;ADCData>DianZ[TemPtIndex];) TemPtIndex++;	
		
			if(TemPtIndex>0)
			{
			  	IndexPt0=TemPtIndex-1;
				IndexPt1=TemPtIndex;
			}
			else
			{
				IndexPt0=0;
				IndexPt1=0;
			}							
		   TemPt1=(ADCData-DianZ[IndexPt0])*50;
		   TemPt1=TemPt1/(DianZ[IndexPt1]-DianZ[IndexPt0]);
		   Tem=TemPt1+IndexPt0*50;
	return Tem;
	
}
//---------------------------------------------------------------------------------------------------------------------
 extern  s16 T1,T2,T3,T4,T5,T6, T7,T8,T9;
 s16  Thermocouple(u16 ThCoupData, u8 chn)
{
  u16 LengDVolt,VoltIndex;
  u16 TemPt;
  u16 Volt,DmptVot;
  u16 ThermocoupleData;
  u8  IndexAD=0,Index0,Index1;
  s16 temper=0;

  double a;
  double b;
  double c;
  double d;

  TemPt=TemperaturePt100();
  a = -2.654512675574497E+06; b = 3.513012601166371E+04; c = 1.673147856015859E+02; d = -2.795572618184489E-01;
  TemPt = (s16)((a + b * (double)TemPt) / (1 + c * (double)TemPt + d * (double)TemPt*(double)TemPt));
  VoltIndex=TemPt/10;
  LengDVolt=((TemPt%10)*(T_TABLT2[VoltIndex+1]-T_TABLT2[VoltIndex]))/10+T_TABLT2[VoltIndex];
  T6 = TemPt;
  switch (chn) {
  case 1:
	  break;
  case 2:
	  break;
  case 3:
	  break;
  case 4:
	  a = -8.213518489535460E+03; b = 3.443011830883973; c = 5.95927889886126E-05; d = -8.305176330140967E-08;
	  break;
  case 5:
	  a = -8.249007854147805E+03; b = 3.440808723039479; c = 6.650041193735524E-05; d = -7.828710876905532E-08;
	  break;
  case 6:
	  a = -8.241652603829643E+03; b = 3.440254953419218; c = 5.571257166616530E-05; d = -8.292518265035715E-08;
	  break;
  case 7:
	  a = -8.288633381962442E+03; b = 3.466047376925705; c = 7.504222083973321E-05; d = -7.542291619547687E-08;
	  break;
  case 8:
	  break;
  case 9:
	  a = -8.107983292435980E+03; b = 3.388512334978625; c = 8.060667468600875E-06; d = -5.546355754617819E-08;
	  break;
  }
  temper = (s16)((a + b * (double)ThCoupData) / (1 + c * (double)ThCoupData + d * (double)ThCoupData*(double)ThCoupData));
//  return ThCoupData;
  if (temper + LengDVolt > 0) {
		DmptVot = temper + LengDVolt;
		if(DmptVot>T_TABLT2[190]) DmptVot=T_TABLT2[190];
		for(;DmptVot>T_TABLT2[IndexAD];) IndexAD++;
		if(IndexAD>0) {
			Index0=IndexAD-1;
			Index1=IndexAD;
		} else {
			Index0=0;
			Index1=0;
		}
		temper=(double)(DmptVot-T_TABLT2[Index0])/(double)(T_TABLT2[Index1]-T_TABLT2[Index0])*10+Index0*10;
  } else {
		DmptVot = abs(temper + LengDVolt);
		if(DmptVot>T_TABLT1[255]) DmptVot=T_TABLT1[255];
		for(;DmptVot>T_TABLT1[IndexAD];) IndexAD++;
		if(IndexAD>0) {
			Index0=IndexAD-1;
			Index1=IndexAD;
		} else {
			Index0=0;
			Index1=0;
		}
		temper=(double)(DmptVot-T_TABLT1[Index0])/(double)(T_TABLT1[Index1]-T_TABLT1[Index0])*10+Index0*10;
		temper=-temper;
  }
  return temper;
}

 extern  short int getFFT(short int *a);
s16 getTemp(s16 *nowtemp, s16 lasttemp)
{
	float proportion = 0;
	u16 dT = abs(nowtemp[63] - lasttemp);
	if (dT < 20) {
		proportion = (float)dT / 20.0;
		return (s16)(getFFT(nowtemp) * proportion + lasttemp * (1 - proportion));
	}

	return nowtemp[63];
}
