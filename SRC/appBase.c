#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "def.h"
#include "2416addr.h"
#include "2416lib.h"
#include "option.h"
#include "LCD.h"
#include "mmu.h"
#include "Nand.h"
#include "..\inc\download.h"
#include "..\inc\systemconf.h"
#include "..\inc\systemmenu.h"
#include "..\inc\function.h"
#include "..\inc\jiema.h"
#include "..\inc\communication.h"
#include "..\inc\test.h"
#include "..\inc\Rtc.h"
#include "..\inc\encrypt.h"

#include "..\inc\appBase.h"
#include "..\inc\app.h"


//#define Output_frequence  50000   //PWM输出频率

extern INTERALMEMORY g_InteralMemory;

//extern GLOBALVAR g_GlobalVar;

//extern INT16U PreScrNum;


unsigned int InPulseCount[7]={0};		//记录6路高速输入的脉冲数

unsigned int OutPulse_PreSet[4]={0};	// 3路脉冲输出的  脉冲预设值
unsigned int OutPulse_Counter[4]={0};	// 3路脉冲输出的  当前输出脉冲数

int Pulse_Out1,Pulse_Out2,nn,mm,wait,wait1 ;

extern unsigned int K_A2,dK2,Inpulse_xiadao2,Inpulse_qieduan2,Inpulse_taidao2,Inpulse_dengdai2[2],Inpulse_all2,Outpulse_xiadao2,Outpulse_qieduan2,Outpulse_taidao2;////定长切相关全局变量
extern char Dingchang_on_off,Chongkou_on_off,Chongkoushu;

extern unsigned int K_A1,dK1,Inpulse_xiadao1,Inpulse_qieduan1,Inpulse_taidao1,Outpulse_xiadao1,Outpulse_qieduan1,Outpulse_taidao1;////冲口相关全局变量
extern unsigned int Inpulse_dengdai1[100],Inpulse_all1[13];
char Current_chongkou;

extern INT16U K_A1_index[500],K_A2_index[500],K_A3_index[180];
int pulseout_qieduan1,pulseout_qieduan2,pulseout_qieduan3;
INT16U KA1_back,KA2_back;
char chongkou_current,dingchang_current;
char qieduan_enable,need_qieduan,current_qieduan,cut_all;/////,cut_all防止定长切后冲口未完成而造成系统继续乱切
int Enable_qieduan[10];
extern INT16U chongkou_last_length;
INT16U delay_back;
char product_start=0;
extern int Dingchang1_2,Dingchang2_2,Dingchang3_2;////用于三个定长切在线传数据
extern char current_product;

extern unsigned int K_A3;
INT16U buchang_pulse,buchang_pulse,buchang_pulse1;

char PulseOut_qie,PulseOut_chong;

void Pulse_out_function_qie(char pulse_number)
{
	char a;
	for(a=0;a<pulse_number;a++)///切断
	{
		rGPBDAT &=(~(1<<1));
		Delay(350);
		rGPBDAT |=(1<<1);
		Delay(350);
	}
		
	
}


void Pulse_out_function(char pulse_number1,char pulse_number2)
{
	char a;
	for(a=0;((a<pulse_number1)||(a<pulse_number2));a++)
	{
		if((a<pulse_number1) && (g_InteralMemory.Bit[101] == 0))///冲口
		{
			rGPBDAT &=(~(1<<2));
			Delay(350);
			rGPBDAT |=(1<<2);
			Delay(350);
		}
		
		if((a<pulse_number2) && (g_InteralMemory.Bit[100] == 0))///切断
		{
			rGPBDAT &=(~(1<<1));
			Delay(350);
			rGPBDAT |=(1<<1);
			Delay(350);
		}
		
	}
	
}

void chong_qie_function(void)///包含冲口+定长,单独定长两种功能
{
	int i;
	
	
//	InPulseCount[2]++;///定时器内用于计算速度
	for(i=0; i<10; i++)
		Enable_qieduan[i]++;////冲最后一个口后清零此寄存器，然后累计到刀间距+最后一个口至切断处距离后切断
	
	
	buchang_pulse1=g_InteralMemory.Word[500]  * g_InteralMemory.KeepWord[500] / 0.064 / 25 / 100;
	///////////**********************************************/////////////////////////
	///冲口程序
	if(Chongkou_on_off > 0)
	{
		
		
		if((chongkou_current == Chongkoushu) && wait < Inpulse_dengdai1[12])
		{
			wait ++;
			qieduan_enable=0;////允许切断标志位
		}
		else
		{
			if(wait >= Inpulse_dengdai1[12])
			{
				wait = 0;
				chongkou_current = 0;
			}
			InPulseCount[3]++;///输入脉冲计数冲口用
			
			if((InPulseCount[3] <(Inpulse_dengdai1[chongkou_current] -buchang_pulse1) ))
			{
				g_InteralMemory.Word[509]=0;///等待
				g_InteralMemory.Bit[300] = 0;
			}
			///下刀
			else if((InPulseCount[3]>=(Inpulse_dengdai1[chongkou_current]-buchang_pulse1)) && (InPulseCount[3]<(Inpulse_xiadao1 + Inpulse_dengdai1[chongkou_current]-buchang_pulse1)) && (g_InteralMemory.Word[506] < Outpulse_xiadao1))
			{
				g_InteralMemory.Bit[101] = 0;//允许冲口脉冲输出
				g_InteralMemory.Bit[300] = 1;
				Pulse_Out2 += K_A1;
				if((Pulse_Out2 >= 10000) && (g_InteralMemory.Word[509]<onecircle_pulse))///word509 输出脉冲计数，达到onecircle_pulse个脉冲停止发脉冲
				{
					i=Pulse_Out2 / 10000 ;
					if((g_InteralMemory.Word[506]+i) <= Outpulse_xiadao1)
					{
						//Pulse_out_function_chong(i);
						PulseOut_chong=i;
						Pulse_Out2 = Pulse_Out2 - i*10000 ;
						g_InteralMemory.Word[509]+=i;
						g_InteralMemory.Word[506]+=i;
					}
					else
					{
						
						i= Outpulse_xiadao1 - g_InteralMemory.Word[506];
					//	Pulse_out_function_chong(i);
						PulseOut_chong=i;
						Pulse_Out2 = Pulse_Out2 - i*10000 ;
						g_InteralMemory.Word[509]+=i;
						g_InteralMemory.Word[506]+=i;
					}
					
				}
			}
			//切断
			else if((InPulseCount[3]>=(Inpulse_xiadao1 + Inpulse_dengdai1[chongkou_current]-buchang_pulse1)) && (InPulseCount[3]<(Inpulse_xiadao1+Inpulse_qieduan1+ Inpulse_dengdai1[chongkou_current]-buchang_pulse1)) && (g_InteralMemory.Word[507] <= Outpulse_qieduan1))
			{
				//mm++;
				//Pulse_Out2 = Pulse_Out2 + K_A1 - mm * dK1; K_A2_index[pulseout_qieduan2]
				g_InteralMemory.Bit[300] = 1;
				if((qieduan_enable == 0) && ((chongkou_current+1) == Chongkoushu))//冲最后一个口
				{
					qieduan_enable = 1;
					Enable_qieduan[need_qieduan]=0;///计数清零，定长切长度正式开始累积
					need_qieduan++;
					cut_all ++;//累计需要切得根数
					if(need_qieduan >= 10)
						need_qieduan = 0;
					g_InteralMemory.KeepBit[12] = 1;
				}
				Pulse_Out2 = Pulse_Out2 + K_A1_index[pulseout_qieduan1]; 
				
				
				if((Pulse_Out2 >= 10000) && (g_InteralMemory.Word[509]<onecircle_pulse))
				{
					i=Pulse_Out2 / 10000 ;
					if((g_InteralMemory.Word[507] + i) <= Outpulse_qieduan1)
					{
						//Pulse_out_function_chong(i);
						PulseOut_chong=i;
						Pulse_Out2 = Pulse_Out2 - i*10000 ;
						g_InteralMemory.Word[509]+=i;
						g_InteralMemory.Word[507]+=i;
					}
					else
					{
						
						i=Outpulse_qieduan1 - g_InteralMemory.Word[507];
						//Pulse_out_function_chong(i);
						PulseOut_chong=i;
						Pulse_Out2 = Pulse_Out2 - i*10000 ;
						g_InteralMemory.Word[509]+=i;
						g_InteralMemory.Word[507]+=i;
					}
					pulseout_qieduan1 = pulseout_qieduan1 +i;
				}
			}
			//抬刀
			else if(((InPulseCount[3]>=(Inpulse_xiadao1+Inpulse_qieduan1+ Inpulse_dengdai1[chongkou_current]-buchang_pulse1)) || (g_InteralMemory.Word[507] > Outpulse_qieduan1))
					&& (InPulseCount[3]<(Inpulse_xiadao1+Inpulse_qieduan1+Inpulse_taidao1+ Inpulse_dengdai1[chongkou_current]-buchang_pulse1))&& (g_InteralMemory.Word[508] < Outpulse_taidao1))
			{
				g_InteralMemory.Word[513]++;
				
				g_InteralMemory.Bit[300] = 1;
				if(pulseout_qieduan1 > 0)
					Pulse_Out2 = Pulse_Out2 + K_A1_index[pulseout_qieduan1];	
				else
				{
					if(mm == 0)
					{
						KA1_back=10000*(Outpulse_taidao1-g_InteralMemory.Word[508])/(Inpulse_taidao1 - g_InteralMemory.Word[513]);
						mm++;
					}
					Pulse_Out2 = Pulse_Out2 + KA1_back;
				}
				if((Pulse_Out2 >= 10000) && (g_InteralMemory.Word[509]<onecircle_pulse))
				{
					i=Pulse_Out2 / 10000 ;
					if((g_InteralMemory.Word[509]+i) <= onecircle_pulse)
					{
					//	Pulse_out_function_chong(i);
						PulseOut_chong=i;
						Pulse_Out2 = Pulse_Out2 - i*10000 ;
						g_InteralMemory.Word[509]+=i;
						g_InteralMemory.Word[508]+=i;
					}
					else
					{
						
						i=onecircle_pulse - g_InteralMemory.Word[509];
						//Pulse_out_function_chong(i);
						PulseOut_chong=i;
						Pulse_Out2 = Pulse_Out2 - i*10000 ;
						g_InteralMemory.Word[509]+=i;
						g_InteralMemory.Word[508]+=i;
					}
					if((pulseout_qieduan1-i) >= 0)
						pulseout_qieduan1=pulseout_qieduan1-i;
					else
						pulseout_qieduan1=0;
					
				}
			}
			
			else
			{
				g_InteralMemory.Bit[300] = 0;				
				if(g_InteralMemory.Word[509]<onecircle_pulse)///如果一圈没发满onecircle_pulse个脉冲则补发剩余脉冲
				{
					
					i=onecircle_pulse-g_InteralMemory.Word[509];//
					//Pulse_out_function_chong(i);
					PulseOut_chong=i;
				}
				
				g_InteralMemory.Word[504]=InPulseCount[3];///word504定长输入脉冲计数，测试用
				InPulseCount[3] = 0;
				mm=0;
				Pulse_Out2=0;
				pulseout_qieduan1=0;
				if((chongkou_current+1) <= Chongkoushu)
					chongkou_current++;
				
					
					
				g_InteralMemory.Word[506]=0;
				g_InteralMemory.Word[507]=0;
				g_InteralMemory.Word[508]=0;
				g_InteralMemory.Word[509]=0;
				g_InteralMemory.Word[510]=0;
				
				g_InteralMemory.Word[513]=0;
				/////////////////////////////////////////////////////////////参数在线导入
				if(g_InteralMemory.Bit[80] == 1)/////冲口-定长产品一完成
				{
					Inpulse_dengdai1[0] = g_InteralMemory.KeepWord[48]-g_InteralMemory.KeepWord[61];//段长1等待脉冲数=段长脉冲-下刀脉冲
					Inpulse_dengdai1[1] = g_InteralMemory.KeepWord[64];
					Inpulse_dengdai1[2] = g_InteralMemory.KeepWord[65];
					Inpulse_dengdai1[3] = g_InteralMemory.KeepWord[66];
					Inpulse_dengdai1[4] = g_InteralMemory.KeepWord[67];
					Inpulse_dengdai1[5] = g_InteralMemory.KeepWord[68];
					Inpulse_dengdai1[6] = g_InteralMemory.KeepWord[69];
					Inpulse_dengdai1[7] = g_InteralMemory.KeepWord[70];
					Inpulse_dengdai1[8] = g_InteralMemory.KeepWord[71];
					Inpulse_dengdai1[9] = g_InteralMemory.KeepWord[72];
					Inpulse_dengdai1[10] = g_InteralMemory.KeepWord[73];
					Inpulse_dengdai1[11] = g_InteralMemory.KeepWord[74];
					Inpulse_dengdai1[12] = g_InteralMemory.KeepWord[78];///////////////////////
				}
				else if(g_InteralMemory.Bit[90] == 1)/////冲口-定长产品2完成
				{
					Inpulse_dengdai1[0] = g_InteralMemory.KeepWord[389]-g_InteralMemory.KeepWord[402];//段长1等待脉冲数=段长脉冲-下刀脉冲
					Inpulse_dengdai1[1] = g_InteralMemory.KeepWord[406];
					Inpulse_dengdai1[2] = g_InteralMemory.KeepWord[407];
					Inpulse_dengdai1[3] = g_InteralMemory.KeepWord[408];
					Inpulse_dengdai1[4] = g_InteralMemory.KeepWord[409];
					Inpulse_dengdai1[5] = g_InteralMemory.KeepWord[410];
					Inpulse_dengdai1[6] = g_InteralMemory.KeepWord[411];
					Inpulse_dengdai1[7] = g_InteralMemory.KeepWord[412];
					Inpulse_dengdai1[8] = g_InteralMemory.KeepWord[413];
					Inpulse_dengdai1[9] = g_InteralMemory.KeepWord[414];
					Inpulse_dengdai1[10] = g_InteralMemory.KeepWord[415];
					Inpulse_dengdai1[11] = g_InteralMemory.KeepWord[416];
					Inpulse_dengdai1[12] = g_InteralMemory.KeepWord[419];///////////////////////
				}
	
			}
			
			
			
			
		}
	}	
	
	//////////////////////////////////////////////////////////////
	if((g_InteralMemory.KeepBit[12] == 1) && (Chongkou_on_off > 0) && (g_InteralMemory.KeepBit[10] == 0) && (cut_all > 0))
	{
		/**************Bug Fixed FJW****************/
		if(g_InteralMemory.KeepWord[2] == 0)
			g_InteralMemory.KeepWord[2] = 510;
		/**************Bug Fixed FJW****************/
		g_InteralMemory.KeepWord[79] = g_InteralMemory.KeepWord[42] / 10.0 * Encoder_Pulse / PI / g_InteralMemory.KeepWord[2] * 10.0 ;////刀间距对应编码器脉冲数,允许在线调整
		
		buchang_pulse=g_InteralMemory.Word[500]  * g_InteralMemory.KeepWord[169] / 0.064 / 25 / 100;
		if(Enable_qieduan[current_qieduan] >= (g_InteralMemory.KeepWord[79]  - Inpulse_xiadao2 + buchang_pulse))
		{
			g_InteralMemory.KeepBit[10] = 1;
			dingchang_current=0;
			Inpulse_dengdai2[dingchang_current]=chongkou_last_length;//需要等待一段时间，主要用于给上一段长最后的补偿脉冲留出时间发完，以免影响后续脉冲计数
			
		}
	}
	if(Chongkou_on_off == 0)
	{
		g_InteralMemory.KeepBit[10] = 1;///不冲口时要允许定长切
		cut_all=1;
	}
		
	if((Dingchang_on_off > 0) && (g_InteralMemory.KeepBit[10] == 1) && (cut_all > 0))///定长切程序KB10 冲口与定长切互联开始定长切标志位
	{
	
	
		InPulseCount[1]++;///输入脉冲计数定长切用
		if((InPulseCount[1] <Inpulse_dengdai2[dingchang_current]) )
		{
			g_InteralMemory.Word[505] = 0;////等待
			g_InteralMemory.Bit[301] = 0;
		}
		///下刀
		else if((InPulseCount[1]>=Inpulse_dengdai2[dingchang_current]) && (InPulseCount[1]<=(Inpulse_xiadao2 + Inpulse_dengdai2[dingchang_current])) && (g_InteralMemory.Word[501] < Outpulse_xiadao2))
		{
			g_InteralMemory.Bit[100] = 0;//允许定长切脉冲输出
			g_InteralMemory.Bit[49] =1;//禁止强切
			g_InteralMemory.Bit[301] = 1;
			Pulse_Out1 += K_A2;
			if((Pulse_Out1 >= 10000) && (g_InteralMemory.Word[505]<onecircle_pulse))///word505 输出脉冲计数，达到onecircle_pulse个脉冲停止发脉冲
			{
				
				i=Pulse_Out1 / 10000 ;
				//Pulse_out_function_qie(i);
				PulseOut_qie=i;
				Pulse_Out1 = Pulse_Out1 - i*10000 ;
				g_InteralMemory.Word[505]+=i;
				g_InteralMemory.Word[501]+=i;
				
			}
			
		}
		//切断
		else if((InPulseCount[1]>(Inpulse_xiadao2 + Inpulse_dengdai2[dingchang_current])) && (InPulseCount[1]<=(Inpulse_xiadao2+Inpulse_qieduan2+ Inpulse_dengdai2[dingchang_current])) && (g_InteralMemory.Word[502] <= Outpulse_qieduan2))
		{
			g_InteralMemory.Bit[49] =1;//禁止强切
		//	nn++;	
			g_InteralMemory.Bit[301] = 1;
			Pulse_Out1 = Pulse_Out1 + K_A2_index[pulseout_qieduan2];
			if((Pulse_Out1 >= 10000) && (g_InteralMemory.Word[505]<onecircle_pulse))
			{
				
				i=Pulse_Out1 / 10000 ;
			//	Pulse_out_function_qie(i);
				PulseOut_qie=i;
				Pulse_Out1 = Pulse_Out1 - i*10000 ;
				pulseout_qieduan2=pulseout_qieduan2+i;
				g_InteralMemory.Word[505]+=i;
				g_InteralMemory.Word[502]+=i;
			}
			
		}
		//抬刀
		else if(((InPulseCount[1]>(Inpulse_xiadao2+Inpulse_qieduan2+ Inpulse_dengdai2[dingchang_current])) || (g_InteralMemory.Word[502] > Outpulse_qieduan2))
				&& (InPulseCount[1]<=Inpulse_xiadao2+Inpulse_qieduan2+Inpulse_taidao2+ Inpulse_dengdai2[dingchang_current]) && (g_InteralMemory.Word[503] < Outpulse_taidao2))
		{
			g_InteralMemory.Bit[49] =1;//禁止强切
			g_InteralMemory.Bit[301] = 1;
		/*	if(delay_back == 0)
			{
				for(i=0; i<1000; i++);
				delay_back=1;
			}*/
			Pulse_Out1 = Pulse_Out1 + K_A2;
			
		
			if((Pulse_Out1 >= 10000) && (g_InteralMemory.Word[505]<onecircle_pulse))
			{
				i=Pulse_Out1 / 10000 ;
				if((g_InteralMemory.Word[505] + i) <=onecircle_pulse)
				{
					//Pulse_out_function_qie(i);
					PulseOut_qie=i;
					Pulse_Out1 = Pulse_Out1 - i*10000 ;
					g_InteralMemory.Word[505]+=i;
					g_InteralMemory.Word[503]+=i;
					
				}
				else 
				{
					
					i=onecircle_pulse - g_InteralMemory.Word[505];
					//Pulse_out_function_qie(i);
					PulseOut_qie=i;
					Pulse_Out1 = Pulse_Out1 - i*10000 ;
					g_InteralMemory.Word[505]+=i;
					g_InteralMemory.Word[503]+=i;
				}
				
			}
			
			
		}
		
		else// if((InPulseCount[1]>Inpulse_xiadao2+Inpulse_qieduan2+Inpulse_taidao2+ Inpulse_dengdai2[dingchang_current]) || (g_InteralMemory.Word[503] >= Outpulse_taidao2))
		{
			
			g_InteralMemory.Bit[301] = 0;
			if(g_InteralMemory.Word[505]<onecircle_pulse)///如果一圈没发满onecircle_pulse个脉冲则补发剩余脉冲
			{
				i=onecircle_pulse-g_InteralMemory.Word[505];//
				PulseOut_qie=i;
				//Pulse_out_function_qie(i);
			}
			InPulseCount[1] = 0;
		//	nn=0;
			Pulse_Out1=0;
			pulseout_qieduan2=0;
			dingchang_current = 1;///第一次等待的长度与以后的不一样
			g_InteralMemory.KeepWord[10]++;//产量计数
						
			g_InteralMemory.Word[501]=0;
			g_InteralMemory.Word[502]=0;
			g_InteralMemory.Word[503]=0;
			g_InteralMemory.Word[505]=0;
			g_InteralMemory.Word[512]=0;
			
			g_InteralMemory.KeepBit[10]=0;///完成一次后停止,等待下一次开启
			current_qieduan++;
			if(current_qieduan >= 10)
				current_qieduan = 0;
			
			cut_all -- ;//每切完一根减少一根知道最后一根
			delay_back=0;
			g_InteralMemory.Bit[49] =0;//允许强切
			
			/////////////////////////////////////////////////////////////参数在线导入
			if(g_InteralMemory.Bit[80] == 1)/////冲口-定长产品一完成
			{
				chongkou_last_length = g_InteralMemory.KeepWord[60];
			}
			else if(g_InteralMemory.Bit[90] == 1)//////冲口-定长产品2完成
			{
				chongkou_last_length = g_InteralMemory.KeepWord[401];
			}
			else if(g_InteralMemory.Bit[38] == 1)/////三段定长连续加工
			{
				if(( g_InteralMemory.KeepWord[10] <  g_InteralMemory.KeepWord[83]) && (current_product == 0))
				{
					Inpulse_dengdai2[0] = Dingchang1_2;///////101  102  103
					Inpulse_dengdai2[1] = Dingchang1_2;
				}
				if(( g_InteralMemory.KeepWord[10] <  g_InteralMemory.KeepWord[87]) && (current_product == 1))
				{
					Inpulse_dengdai2[0] = Dingchang2_2;///////101  102  103
					Inpulse_dengdai2[1] = Dingchang2_2;
				}
				if(( g_InteralMemory.KeepWord[10] <  g_InteralMemory.KeepWord[91]) && (current_product == 2))
				{
					Inpulse_dengdai2[0] = Dingchang3_2;///////101  102  103
					Inpulse_dengdai2[1] = Dingchang3_2;
				}
			}
			else if(g_InteralMemory.Bit[39] == 1)/////第一段定长
			{
				Inpulse_dengdai2[0] = Dingchang1_2;///////101  102  103
				Inpulse_dengdai2[1] = Dingchang1_2;
			}
			else if(g_InteralMemory.Bit[40] == 1)////第二段定长
			{
				Inpulse_dengdai2[0] = Dingchang2_2;///////101  102  103
				Inpulse_dengdai2[1] = Dingchang2_2;
			}
			else if(g_InteralMemory.Bit[41] == 1)/////第三段定长
			{
				Inpulse_dengdai2[0] = Dingchang3_2;///////101  102  103
				Inpulse_dengdai2[1] = Dingchang3_2;
			}
				
		}
	}///定长切结束
	/////////////////////////
	/////////**********************************************///////////////////////



	Pulse_out_function(PulseOut_chong,PulseOut_qie);
	PulseOut_chong=0;
	PulseOut_qie=0;
	//////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////		
}

void xiekou_chongqie_function(void)
{
	int i;
	
	
	for(i=0; i<10; i++)
		Enable_qieduan[i]++;////冲最后一个口后清零此寄存器，然后累计到刀间距+最后一个口至切断处距离后切断
	
	
	buchang_pulse1=g_InteralMemory.Word[500]  * g_InteralMemory.KeepWord[500] / 0.064 / 25 / 100;
	///////////**********************************************/////////////////////////
	///冲口程序
	if(Chongkou_on_off > 0)
	{
		InPulseCount[3]++;///输入脉冲计数冲口用
		
		if((InPulseCount[3] <(Inpulse_dengdai1[chongkou_current] - buchang_pulse1) ))
		{
			g_InteralMemory.Word[509]=0;///等待
			g_InteralMemory.Bit[300] = 0;
		}
		///下刀
		else if((InPulseCount[3]>=Inpulse_dengdai1[chongkou_current] - buchang_pulse1) && (InPulseCount[3]<(Inpulse_xiadao1 + Inpulse_dengdai1[chongkou_current] - buchang_pulse1)) && (g_InteralMemory.Word[506] < Outpulse_xiadao1))
		{
			g_InteralMemory.Bit[101] = 0;//允许冲口脉冲输出
			g_InteralMemory.Bit[300] = 1;
			Pulse_Out2 += K_A1;
			if((Pulse_Out2 >= 10000) && (g_InteralMemory.Word[509]<onecircle_pulse))///word509 输出脉冲计数，达到onecircle_pulse个脉冲停止发脉冲
			{
				i=Pulse_Out2 / 10000 ;
				if((g_InteralMemory.Word[506]+i) <= Outpulse_xiadao1)
				{
					//Pulse_out_function_chong(i);
					PulseOut_chong=i;
					Pulse_Out2 = Pulse_Out2 - i*10000 ;
					g_InteralMemory.Word[509]+=i;
					g_InteralMemory.Word[506]+=i;
				}
				else
				{
					
					i= Outpulse_xiadao1 - g_InteralMemory.Word[506];
					//Pulse_out_function_chong(i);
					PulseOut_chong=i;
					Pulse_Out2 = Pulse_Out2 - i*10000 ;
					g_InteralMemory.Word[509]+=i;
					g_InteralMemory.Word[506]+=i;
				}
				
			}
		}
		//切断
		else if((InPulseCount[3]>=(Inpulse_xiadao1 + Inpulse_dengdai1[chongkou_current] - buchang_pulse1)) && (InPulseCount[3]<(Inpulse_xiadao1+Inpulse_qieduan1+ Inpulse_dengdai1[chongkou_current] - buchang_pulse1)) && (g_InteralMemory.Word[507] <= Outpulse_qieduan1))
		{
			g_InteralMemory.Bit[300] = 1;
			if((qieduan_enable == 0) && (chongkou_current == Chongkoushu))//冲最后一个口第四个口
			{
				qieduan_enable = 1;
				Enable_qieduan[need_qieduan]=0;///计数清零，定长切长度正式开始累积
				need_qieduan++;
				cut_all ++;//累计需要切得根数
				if(need_qieduan >= 10)
					need_qieduan = 0;
				g_InteralMemory.KeepBit[12] = 1;
			}
			Pulse_Out2 = Pulse_Out2 + K_A1_index[pulseout_qieduan1]; 
			
			
			if((Pulse_Out2 >= 10000) && (g_InteralMemory.Word[509]<onecircle_pulse))
			{
				i=Pulse_Out2 / 10000 ;
				if((g_InteralMemory.Word[507] + i) <= Outpulse_qieduan1)
				{
					//Pulse_out_function_chong(i);
					PulseOut_chong=i;
					Pulse_Out2 = Pulse_Out2 - i*10000 ;
					g_InteralMemory.Word[509]+=i;
					g_InteralMemory.Word[507]+=i;
				}
				else
				{
					
					i=Outpulse_qieduan1 - g_InteralMemory.Word[507];
					//Pulse_out_function_chong(i);
					PulseOut_chong=i;
					Pulse_Out2 = Pulse_Out2 - i*10000 ;
					g_InteralMemory.Word[509]+=i;
					g_InteralMemory.Word[507]+=i;
				}
				pulseout_qieduan1 = pulseout_qieduan1 +i;
			}
		}
		//抬刀
		else if(((InPulseCount[3]>=(Inpulse_xiadao1+Inpulse_qieduan1+ Inpulse_dengdai1[chongkou_current] - buchang_pulse1)) || (g_InteralMemory.Word[507] > Outpulse_qieduan1))
				&& (InPulseCount[3]<(Inpulse_xiadao1+Inpulse_qieduan1+Inpulse_taidao1+ Inpulse_dengdai1[chongkou_current] - buchang_pulse1))&& (g_InteralMemory.Word[508] < Outpulse_taidao1))
		{
			g_InteralMemory.Bit[300] = 1;
			g_InteralMemory.Word[513]++;
			

			if(pulseout_qieduan1 > 0)
				Pulse_Out2 = Pulse_Out2 + K_A1_index[pulseout_qieduan1];	
			else
			{
				if(mm == 0)
				{
					KA1_back=10000*(Outpulse_taidao1-g_InteralMemory.Word[508])/(Inpulse_taidao1 - g_InteralMemory.Word[513]);
					mm++;
				}
				Pulse_Out2 = Pulse_Out2 + KA1_back;
			}
			if((Pulse_Out2 >= 10000) && (g_InteralMemory.Word[509]<onecircle_pulse))
			{
				i=Pulse_Out2 / 10000 ;
				if((g_InteralMemory.Word[509]+i) <= onecircle_pulse)
				{
					//Pulse_out_function_chong(i);
					PulseOut_chong=i;
					Pulse_Out2 = Pulse_Out2 - i*10000 ;
					g_InteralMemory.Word[509]+=i;
					g_InteralMemory.Word[508]+=i;
				}
				else
				{
					
					i=onecircle_pulse - g_InteralMemory.Word[509];
				//	Pulse_out_function_chong(i);
					PulseOut_chong=i;
					Pulse_Out2 = Pulse_Out2 - i*10000 ;
					g_InteralMemory.Word[509]+=i;
					g_InteralMemory.Word[508]+=i;
				}
				if((pulseout_qieduan1-i) >= 0)
					pulseout_qieduan1=pulseout_qieduan1-i;
				else
					pulseout_qieduan1=0;
				
			}
		}
		
		else
		{
			g_InteralMemory.Bit[300] = 0;				
			if(g_InteralMemory.Word[509]<onecircle_pulse)///如果一圈没发满onecircle_pulse个脉冲则补发剩余脉冲
			{
				
				i=onecircle_pulse-g_InteralMemory.Word[509];//
				//Pulse_out_function_chong(i);
				PulseOut_chong=i;
			}
			
			g_InteralMemory.Word[504]=InPulseCount[3];///word504定长输入脉冲计数，测试用
			InPulseCount[3] = 0;
			mm=0;
			Pulse_Out2=0;
			pulseout_qieduan1=0;
			if(chongkou_current< Chongkoushu)
				chongkou_current++;
			else
				chongkou_current=0;
			
			qieduan_enable = 0;	
				
			g_InteralMemory.Word[506]=0;
			g_InteralMemory.Word[507]=0;
			g_InteralMemory.Word[508]=0;
			g_InteralMemory.Word[509]=0;
			g_InteralMemory.Word[510]=0;
			
			g_InteralMemory.Word[513]=0;	
			
			////////////////////////////////////////////////斜口数据在线导入
			if(g_InteralMemory.Bit[81] == 1)
			{
			Inpulse_dengdai1[0] = g_InteralMemory.KeepWord[134];
			Inpulse_dengdai1[1] = g_InteralMemory.KeepWord[131];
			Inpulse_dengdai1[2] = g_InteralMemory.KeepWord[132];
			Inpulse_dengdai1[3] = g_InteralMemory.KeepWord[133];
			/*
			if(g_InteralMemory.KeepBit[15] == 0)
				Chongkoushu = 3;
			else if(g_InteralMemory.KeepBit[15] == 1)
				Chongkoushu = 0;	*/
			Chongkoushu = g_InteralMemory.KeepWord[168];
			}
			else if(g_InteralMemory.Bit[91] == 1)
			{
				Inpulse_dengdai1[0] = g_InteralMemory.KeepWord[482];
				Inpulse_dengdai1[1] = g_InteralMemory.KeepWord[479];
				Inpulse_dengdai1[2] = g_InteralMemory.KeepWord[480];
				Inpulse_dengdai1[3] = g_InteralMemory.KeepWord[481];
				
				Chongkoushu = g_InteralMemory.KeepWord[459];
			}
		}
		
	}
	
	
	//////////////////////////////////////////////////////////////
	if((g_InteralMemory.KeepBit[12] == 1) && (Chongkou_on_off > 0) && (g_InteralMemory.KeepBit[10] == 0) && (cut_all > 0))
	{
		/**************Bug Fixed FJW****************/
		if(g_InteralMemory.KeepWord[2] == 0)
			g_InteralMemory.KeepWord[2] = 510;
		/**************Bug Fixed FJW****************/
		
		g_InteralMemory.KeepWord[79] = g_InteralMemory.KeepWord[42] / 10.0 * Encoder_Pulse / PI / g_InteralMemory.KeepWord[2] * 10.0 ;////刀间距对应编码器脉冲数,允许在线调整
		buchang_pulse=g_InteralMemory.Word[500]  * g_InteralMemory.KeepWord[169] / 0.064 / 25 / 100;
		if(Enable_qieduan[current_qieduan] >= (g_InteralMemory.KeepWord[79]  - Inpulse_xiadao2-20 + buchang_pulse))
		{
			g_InteralMemory.KeepBit[10] = 1;
			dingchang_current=0;
			Inpulse_dengdai2[dingchang_current]=20;//需要等待一段时间，主要用于给上一段长最后的补偿脉冲留出时间发完，以免影响后续脉冲计数
			
		}
	}
		
	if((Dingchang_on_off > 0) && (g_InteralMemory.KeepBit[10] == 1) && (cut_all > 0))///定长切程序KB10 冲口与定长切互联开始定长切标志位
	{
	
	
		InPulseCount[1]++;///输入脉冲计数定长切用
		if((InPulseCount[1] <Inpulse_dengdai2[dingchang_current]) )
		{
			g_InteralMemory.Word[505] = 0;////等待
			g_InteralMemory.Bit[301] = 0;
		}
		///下刀
		else if((InPulseCount[1]>=Inpulse_dengdai2[dingchang_current]) && (InPulseCount[1]<=(Inpulse_xiadao2 + Inpulse_dengdai2[dingchang_current])) && (g_InteralMemory.Word[501] < Outpulse_xiadao2))
		{
			g_InteralMemory.Bit[100] = 0;//允许定长切脉冲输出
			g_InteralMemory.Bit[49] =1;//禁止强切
			g_InteralMemory.Bit[301] = 1;
			Pulse_Out1 += K_A2;
			if((Pulse_Out1 >= 10000) && (g_InteralMemory.Word[505]<onecircle_pulse))///word505 输出脉冲计数，达到onecircle_pulse个脉冲停止发脉冲
			{
				
				i=Pulse_Out1 / 10000 ;
				//Pulse_out_function_qie(i);
				PulseOut_qie=i;
				Pulse_Out1 = Pulse_Out1 - i*10000 ;
				g_InteralMemory.Word[505]+=i;
				g_InteralMemory.Word[501]+=i;
				
			}
		}
		//切断
		else if((InPulseCount[1]>(Inpulse_xiadao2 + Inpulse_dengdai2[dingchang_current])) && (InPulseCount[1]<=(Inpulse_xiadao2+Inpulse_qieduan2+ Inpulse_dengdai2[dingchang_current])) && (g_InteralMemory.Word[502] <= Outpulse_qieduan2))
		{
			g_InteralMemory.Bit[301] = 1;
			g_InteralMemory.Bit[49] =1;//禁止强切
		//	nn++;	
			Pulse_Out1 = Pulse_Out1 + K_A2_index[pulseout_qieduan2];
			if((Pulse_Out1 >= 10000) && (g_InteralMemory.Word[505]<onecircle_pulse))
			{
				
				i=Pulse_Out1 / 10000 ;
				//Pulse_out_function_qie(i);
				PulseOut_qie=i;
				Pulse_Out1 = Pulse_Out1 - i*10000 ;
				pulseout_qieduan2=pulseout_qieduan2+i;
				g_InteralMemory.Word[505]+=i;
				g_InteralMemory.Word[502]+=i;
			}
		}
		//抬刀
		else if(((InPulseCount[1]>(Inpulse_xiadao2+Inpulse_qieduan2+ Inpulse_dengdai2[dingchang_current])) || (g_InteralMemory.Word[502] > Outpulse_qieduan2))
				&& (InPulseCount[1]<=Inpulse_xiadao2+Inpulse_qieduan2+Inpulse_taidao2+ Inpulse_dengdai2[dingchang_current]) && (g_InteralMemory.Word[503] < Outpulse_taidao2))
		{	
			g_InteralMemory.Bit[301] = 1;
			g_InteralMemory.Bit[49] =1;//禁止强切
			
			Pulse_Out1 = Pulse_Out1 + K_A2;
			
			if((Pulse_Out1 >= 10000) && (g_InteralMemory.Word[505]<onecircle_pulse))
			{
				i=Pulse_Out1 / 10000 ;
				if((g_InteralMemory.Word[505] + i) <=onecircle_pulse)
				{
					//Pulse_out_function_qie(i);
					PulseOut_qie=i;
					Pulse_Out1 = Pulse_Out1 - i*10000 ;
					g_InteralMemory.Word[505]+=i;
					g_InteralMemory.Word[503]+=i;
					
				}
				else 
				{
					
					i=onecircle_pulse - g_InteralMemory.Word[505];
					//Pulse_out_function_qie(i);
					PulseOut_qie=i;
					Pulse_Out1 = Pulse_Out1 - i*10000 ;
					g_InteralMemory.Word[505]+=i;
					g_InteralMemory.Word[503]+=i;
				}
				
			}
			
		}
		
		else
		{
			
			g_InteralMemory.Bit[301] = 0;
			if(g_InteralMemory.Word[505]<onecircle_pulse)///如果一圈没发满onecircle_pulse个脉冲则补发剩余脉冲
			{
				
				i=onecircle_pulse-g_InteralMemory.Word[505];//
				//Pulse_out_function_qie(i);
				PulseOut_qie=i;
			}
			InPulseCount[1] = 0;
		//	nn=0;
			Pulse_Out1=0;
			pulseout_qieduan2=0;
			//dingchang_current = 1;///第一次等待的长度与以后的不一样
			g_InteralMemory.KeepWord[10]++;//产量计数
						
			g_InteralMemory.Word[501]=0;
			g_InteralMemory.Word[502]=0;
			g_InteralMemory.Word[503]=0;
			g_InteralMemory.Word[505]=0;
			g_InteralMemory.Word[512]=0;
			
			g_InteralMemory.KeepBit[10]=0;///完成一次后停止,等待下一次开启
			current_qieduan++;
			if(current_qieduan >= 10)
				current_qieduan = 0;
			
			cut_all -- ;//每切完一根减少一根知道最后一根
			delay_back=0;
			g_InteralMemory.Bit[49] =0;//允许强切
		}
	}///定长切结束
	/////////////////////////
	/////////**********************************************///////////////////////
	
	Pulse_out_function(PulseOut_chong,PulseOut_qie);
	PulseOut_chong=0;
	PulseOut_qie=0;
}


void lianchong_qie_function(void)
{
	int i,j;
	
	
	
	for(i=0; i<10; i++)
		Enable_qieduan[i]++;////冲最后一个口后清零此寄存器，然后累计到刀间距+最后一个口至切断处距离后切断
	
	
	buchang_pulse1=g_InteralMemory.Word[500]  * g_InteralMemory.KeepWord[500] / 0.064 / 25 / 100;
	///////////**********************************************/////////////////////////
	///冲口程序
	if(Chongkou_on_off > 0)
	{
		
			InPulseCount[3]++;///输入脉冲计数冲口用
			
			if((InPulseCount[3] <(Inpulse_dengdai1[chongkou_current] - buchang_pulse1) ))
			{
				g_InteralMemory.Word[509]=0;///等待
				g_InteralMemory.Bit[300] = 0;
			}
			///下刀
			else if((InPulseCount[3]>=Inpulse_dengdai1[chongkou_current] - buchang_pulse1) && (InPulseCount[3]<(Inpulse_xiadao1 + Inpulse_dengdai1[chongkou_current] - buchang_pulse1)) && (g_InteralMemory.Word[506] < Outpulse_xiadao1))
			{
				g_InteralMemory.Bit[101] = 0;//允许冲口脉冲输出
				g_InteralMemory.Bit[300] = 1;
				Pulse_Out2 += K_A1;
				if((Pulse_Out2 >= 10000) && (g_InteralMemory.Word[509]<onecircle_pulse))///word509 输出脉冲计数，达到onecircle_pulse个脉冲停止发脉冲
				{
					i=Pulse_Out2 / 10000 ;
					if((g_InteralMemory.Word[506]+i) <= Outpulse_xiadao1)
					{
						//Pulse_out_function_chong(i);
						PulseOut_chong=i;
						Pulse_Out2 = Pulse_Out2 - i*10000 ;
						g_InteralMemory.Word[509]+=i;
						g_InteralMemory.Word[506]+=i;
					}
					else
					{
						
						i= Outpulse_xiadao1 - g_InteralMemory.Word[506];
						//Pulse_out_function_chong(i);
						PulseOut_chong=i;
						Pulse_Out2 = Pulse_Out2 - i*10000 ;
						g_InteralMemory.Word[509]+=i;
						g_InteralMemory.Word[506]+=i;
					}
					
				}
			}
			//切断
			else if((InPulseCount[3]>=(Inpulse_xiadao1 + Inpulse_dengdai1[chongkou_current] - buchang_pulse1)) && (InPulseCount[3]<(Inpulse_xiadao1+Inpulse_qieduan1+ Inpulse_dengdai1[chongkou_current] - buchang_pulse1)) && (g_InteralMemory.Word[507] <= Outpulse_qieduan1))
			{
				g_InteralMemory.Bit[300] = 1;
				if((qieduan_enable == 0) && ((chongkou_current+1) == Chongkoushu))//冲最后一个口
				{
					qieduan_enable = 1;
					Enable_qieduan[need_qieduan]=0;///计数清零，定长切长度正式开始累积
					need_qieduan++;
					cut_all ++;//累计需要切得根数
					if(need_qieduan >= 10)
						need_qieduan = 0;
					g_InteralMemory.KeepBit[12] = 1;
				}
				Pulse_Out2 = Pulse_Out2 + K_A1_index[pulseout_qieduan1]; 
				
				
				if((Pulse_Out2 >= 10000) && (g_InteralMemory.Word[509]<onecircle_pulse))
				{
					i=Pulse_Out2 / 10000 ;
					if((g_InteralMemory.Word[507] + i) <= Outpulse_qieduan1)
					{
						//Pulse_out_function_chong(i);
						PulseOut_chong=i;
						Pulse_Out2 = Pulse_Out2 - i*10000 ;
						g_InteralMemory.Word[509]+=i;
						g_InteralMemory.Word[507]+=i;
					}
					else
					{
						
						i=Outpulse_qieduan1 - g_InteralMemory.Word[507];
					//	Pulse_out_function_chong(i);
					PulseOut_chong=i;
						Pulse_Out2 = Pulse_Out2 - i*10000 ;
						g_InteralMemory.Word[509]+=i;
						g_InteralMemory.Word[507]+=i;
					}
					pulseout_qieduan1 = pulseout_qieduan1 +i;
				}
			}
			//抬刀
			else if(((InPulseCount[3]>=(Inpulse_xiadao1+Inpulse_qieduan1+ Inpulse_dengdai1[chongkou_current] - buchang_pulse1)) || (g_InteralMemory.Word[507] > Outpulse_qieduan1))
					&& (InPulseCount[3]<(Inpulse_xiadao1+Inpulse_qieduan1+Inpulse_taidao1+ Inpulse_dengdai1[chongkou_current] - buchang_pulse1))&& (g_InteralMemory.Word[508] < Outpulse_taidao1))
			{
				g_InteralMemory.Word[513]++;
				g_InteralMemory.Bit[300] = 1;

				if(pulseout_qieduan1 > 0)
					Pulse_Out2 = Pulse_Out2 + K_A1_index[pulseout_qieduan1];	
				else
				{
					if(mm == 0)
					{
						KA1_back=10000*(Outpulse_taidao1-g_InteralMemory.Word[508])/(Inpulse_taidao1 - g_InteralMemory.Word[513]);
						mm++;
					}
					Pulse_Out2 = Pulse_Out2 + KA1_back;
				}
				if((Pulse_Out2 >= 10000) && (g_InteralMemory.Word[509]<onecircle_pulse))
				{
					i=Pulse_Out2 / 10000 ;
					if((g_InteralMemory.Word[509]+i) <= onecircle_pulse)
					{
						//Pulse_out_function_chong(i);
						PulseOut_chong=i;
						Pulse_Out2 = Pulse_Out2 - i*10000 ;
						g_InteralMemory.Word[509]+=i;
						g_InteralMemory.Word[508]+=i;
					}
					else
					{
						
						i=onecircle_pulse - g_InteralMemory.Word[509];
					//	Pulse_out_function_chong(i);
					PulseOut_chong=i;
						Pulse_Out2 = Pulse_Out2 - i*10000 ;
						g_InteralMemory.Word[509]+=i;
						g_InteralMemory.Word[508]+=i;
					}
					if((pulseout_qieduan1-i) >= 0)
						pulseout_qieduan1=pulseout_qieduan1-i;
					else
						pulseout_qieduan1=0;
					
				}
			}
			
			else
			{
					g_InteralMemory.Bit[300] = 0;			
				if(g_InteralMemory.Word[509]<onecircle_pulse)///如果一圈没发满onecircle_pulse个脉冲则补发剩余脉冲
				{
					
					i=onecircle_pulse-g_InteralMemory.Word[509];//
					//Pulse_out_function_chong(i);
					PulseOut_chong=i;
				}
				
				g_InteralMemory.Word[504]=InPulseCount[3];///word504定长输入脉冲计数，测试用
				InPulseCount[3] = 0;
				mm=0;
				Pulse_Out2=0;
				pulseout_qieduan1=0;
				if((chongkou_current+1) < Chongkoushu)
					chongkou_current++;
				else
				{
					chongkou_current=0;
					qieduan_enable = 0;
				}
				
				
					
				g_InteralMemory.Word[506]=0;
				g_InteralMemory.Word[507]=0;
				g_InteralMemory.Word[508]=0;
				g_InteralMemory.Word[509]=0;
				g_InteralMemory.Word[510]=0;
				
				g_InteralMemory.Word[513]=0;	
				///////////////////////////////////////////////////////////////////连冲参数在线导入
				if(g_InteralMemory.Bit[82] == 1)/////连冲-定长产品一完成
				{
					/****Bug Fixed***/
					if(g_InteralMemory.KeepWord[137] == 0){
						g_InteralMemory.KeepWord[137] = 500;
					}
					/****Bug Fixed***/	
					
					Chongkoushu =  (g_InteralMemory.KeepWord[139]-g_InteralMemory.KeepWord[135]) /  g_InteralMemory.KeepWord[137] +1;
			
					Inpulse_dengdai1[0] =g_InteralMemory.KeepWord[153]  + g_InteralMemory.KeepWord[145]-g_InteralMemory.KeepWord[154] * (Chongkoushu-1)-g_InteralMemory.KeepWord[153]-Inpulse_xiadao1-Inpulse_qieduan1-Inpulse_taidao1;
					
					for(j=1;j<=Chongkoushu;j++)
						Inpulse_dengdai1[j] = g_InteralMemory.KeepWord[158];	
				}
				else if(g_InteralMemory.Bit[92] == 1)/////连冲-定长产品2
				{
					/****Bug Fixed***/
					if(g_InteralMemory.KeepWord[503] == 0){
						g_InteralMemory.KeepWord[503] = 500;
					}
					/****Bug Fixed***/	
					
					Chongkoushu =  (g_InteralMemory.KeepWord[505]-g_InteralMemory.KeepWord[501]) /  g_InteralMemory.KeepWord[503] +1;
		
					Inpulse_dengdai1[0] =g_InteralMemory.KeepWord[520]  + g_InteralMemory.KeepWord[512]-g_InteralMemory.KeepWord[521] * (Chongkoushu-1)-g_InteralMemory.KeepWord[520]-Inpulse_xiadao1-Inpulse_qieduan1-Inpulse_taidao1;
					
					for(j=1;j<=Chongkoushu;j++)
						Inpulse_dengdai1[j] = g_InteralMemory.KeepWord[525];
		
				}
			}
		
	}	
	//////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////
	if((g_InteralMemory.KeepBit[12] == 1) && (Chongkou_on_off > 0) && (g_InteralMemory.KeepBit[10] == 0) && (cut_all > 0))
	{
		/**************Bug Fixed FJW****************/
		if(g_InteralMemory.KeepWord[2] == 0)
			g_InteralMemory.KeepWord[2] = 510;
		/**************Bug Fixed FJW****************/
		g_InteralMemory.KeepWord[79] = g_InteralMemory.KeepWord[42] / 10.0 * Encoder_Pulse / PI / g_InteralMemory.KeepWord[2] * 10.0 ;////刀间距对应编码器脉冲数,允许在线调整
		buchang_pulse=g_InteralMemory.Word[500]  * g_InteralMemory.KeepWord[169] / 0.064 / 25 / 100;
		if(Enable_qieduan[current_qieduan] >= (g_InteralMemory.KeepWord[79]  - Inpulse_xiadao2 +buchang_pulse))//////+g_InteralMemory.KeepWord[154]
		{
			g_InteralMemory.KeepBit[10] = 1;
			dingchang_current=0;
			Inpulse_dengdai2[dingchang_current]=chongkou_last_length;//需要等待一段时间，主要用于给上一段长最后的补偿脉冲留出时间发完，以免影响后续脉冲计数
			
		}
	}
		
	if((Dingchang_on_off > 0) && (g_InteralMemory.KeepBit[10] == 1) && (cut_all > 0))///定长切程序KB10 冲口与定长切互联开始定长切标志位
	{
	
	
		InPulseCount[1]++;///输入脉冲计数定长切用
		if((InPulseCount[1] <Inpulse_dengdai2[dingchang_current]) )
		{
			g_InteralMemory.Word[505] = 0;////等待
			g_InteralMemory.Bit[301] = 0;
		}
		///下刀
		else if((InPulseCount[1]>=Inpulse_dengdai2[dingchang_current]) && (InPulseCount[1]<=(Inpulse_xiadao2 + Inpulse_dengdai2[dingchang_current])) && (g_InteralMemory.Word[501] < Outpulse_xiadao2))
		{
			g_InteralMemory.Bit[100] = 0;//允许定长切脉冲输出
			g_InteralMemory.Bit[49] =1;//禁止强切
			g_InteralMemory.Bit[301] = 1;
			Pulse_Out1 += K_A2;
			if((Pulse_Out1 >= 10000) && (g_InteralMemory.Word[505]<onecircle_pulse))///word505 输出脉冲计数，达到onecircle_pulse个脉冲停止发脉冲
			{
				
				i=Pulse_Out1 / 10000 ;
				//Pulse_out_function_qie(i);
				PulseOut_qie=i;
				Pulse_Out1 = Pulse_Out1 - i*10000 ;
				g_InteralMemory.Word[505]+=i;
				g_InteralMemory.Word[501]+=i;
				
			}
		}
		//切断
		else if((InPulseCount[1]>(Inpulse_xiadao2 + Inpulse_dengdai2[dingchang_current])) && (InPulseCount[1]<=(Inpulse_xiadao2+Inpulse_qieduan2+ Inpulse_dengdai2[dingchang_current])) && (g_InteralMemory.Word[502] <= Outpulse_qieduan2))
		{
			g_InteralMemory.Bit[49] =1;//禁止强切
		//	nn++;	
		g_InteralMemory.Bit[301] = 1;
			Pulse_Out1 = Pulse_Out1 + K_A2_index[pulseout_qieduan2];
			if((Pulse_Out1 >= 10000) && (g_InteralMemory.Word[505]<onecircle_pulse))
			{
				
				i=Pulse_Out1 / 10000 ;
				//Pulse_out_function_qie(i);
				PulseOut_qie=i;
				Pulse_Out1 = Pulse_Out1 - i*10000 ;
				pulseout_qieduan2=pulseout_qieduan2+i;
				g_InteralMemory.Word[505]+=i;
				g_InteralMemory.Word[502]+=i;
			}
		}
		//抬刀
		else if(((InPulseCount[1]>(Inpulse_xiadao2+Inpulse_qieduan2+ Inpulse_dengdai2[dingchang_current])) || (g_InteralMemory.Word[502] > Outpulse_qieduan2))
				&& (InPulseCount[1]<=Inpulse_xiadao2+Inpulse_qieduan2+Inpulse_taidao2+ Inpulse_dengdai2[dingchang_current]) && (g_InteralMemory.Word[503] < Outpulse_taidao2))
		{	
			g_InteralMemory.Bit[49] =1;//禁止强切
			g_InteralMemory.Bit[301] = 1;
			Pulse_Out1 = Pulse_Out1 + K_A2;
			
			if((Pulse_Out1 >= 10000) && (g_InteralMemory.Word[505]<onecircle_pulse))
			{
				i=Pulse_Out1 / 10000 ;
				if((g_InteralMemory.Word[505] + i) <=onecircle_pulse)
				{
					//Pulse_out_function_qie(i);
					PulseOut_qie=i;
					Pulse_Out1 = Pulse_Out1 - i*10000 ;
					g_InteralMemory.Word[505]+=i;
					g_InteralMemory.Word[503]+=i;
					
				}
				else 
				{
					
					i=onecircle_pulse - g_InteralMemory.Word[505];
					//Pulse_out_function_qie(i);
					PulseOut_qie=i;
					Pulse_Out1 = Pulse_Out1 - i*10000 ;
					g_InteralMemory.Word[505]+=i;
					g_InteralMemory.Word[503]+=i;
				}
				
			}
			
		}
		
		else
		{
			
			g_InteralMemory.Bit[301] = 0;
			if(g_InteralMemory.Word[505]<onecircle_pulse)///如果一圈没发满onecircle_pulse个脉冲则补发剩余脉冲
			{
				
				i=onecircle_pulse-g_InteralMemory.Word[505];//
				//Pulse_out_function_qie(i);
				PulseOut_qie=i;
			}
			InPulseCount[1] = 0;
		//	nn=0;
			Pulse_Out1=0;
			pulseout_qieduan2=0;
		//	dingchang_current = 1;///第一次等待的长度与以后的不一样
			g_InteralMemory.KeepWord[10]++;//产量计数
						
			g_InteralMemory.Word[501]=0;
			g_InteralMemory.Word[502]=0;
			g_InteralMemory.Word[503]=0;
			g_InteralMemory.Word[505]=0;
			g_InteralMemory.Word[512]=0;
			
			g_InteralMemory.KeepBit[10]=0;///完成一次后停止,等待下一次开启
			current_qieduan++;
			if(current_qieduan >= 10)
				current_qieduan = 0;
			
			cut_all -- ;//每切完一根减少一根知道最后一根
			delay_back=0;
			g_InteralMemory.Bit[49] =0;//允许强切
			
			//////////////////////////////////////////////////////////////在线参数导入
			if(g_InteralMemory.Bit[82] == 1)/////连冲-定长产品一完成
			{
				chongkou_last_length = g_InteralMemory.KeepWord[145]-g_InteralMemory.KeepWord[154] * (Chongkoushu-1)-g_InteralMemory.KeepWord[153];
			
			}
			else if(g_InteralMemory.Bit[92] == 1)/////连冲-定长产品2
			{
				chongkou_last_length = g_InteralMemory.KeepWord[512]-g_InteralMemory.KeepWord[521] * (Chongkoushu-1)-g_InteralMemory.KeepWord[520] ;
			}
			Inpulse_dengdai2[0] = chongkou_last_length;
			Inpulse_dengdai2[1] = chongkou_last_length;
		}
	}///定长切结束
	/////////////////////////
	/////////**********************************************///////////////////////
	
		
	Pulse_out_function(PulseOut_chong,PulseOut_qie);
	PulseOut_chong=0;
	PulseOut_qie=0;
	
}
void qiangqie_function(void)
{
	int i;
		
	InPulseCount[4]++;///输入脉冲计数定长切用
	if((InPulseCount[4] <20) )
		g_InteralMemory.Word[519] = 0;////等待
	///下刀
	else if((InPulseCount[4]>=20) && (InPulseCount[4]<=(215 + 20)) && (g_InteralMemory.Word[516] < 338))
	{
		Pulse_Out1 += K_A3;
		if((Pulse_Out1 >= 10000) && (g_InteralMemory.Word[519]<onecircle_pulse))///word505 输出脉冲计数，达到onecircle_pulse个脉冲停止发脉冲
		{
			
			i=Pulse_Out1 / 10000 ;
			Pulse_out_function_qie(i);
			Pulse_Out1 = Pulse_Out1 - i*10000 ;
			g_InteralMemory.Word[519]+=i;
			g_InteralMemory.Word[516]+=i;
			
		}
	}
	//切断
	else if((InPulseCount[4]>(215 + 20)) && (InPulseCount[4]<=(215+166+ 20)) && (g_InteralMemory.Word[517] <= 162))
	{
		
	//	nn++;	
		if(product_start == 0)
		{
			product_start =1;
		//	InPulseCount[1]=0;
		//	InPulseCount[3]=0;
		}
		Pulse_Out1 = Pulse_Out1 + K_A3_index[pulseout_qieduan3];
		if((Pulse_Out1 >= 10000) && (g_InteralMemory.Word[519]<onecircle_pulse))
		{
			
			i=Pulse_Out1 / 10000 ;
			Pulse_out_function_qie(i);
			Pulse_Out1 = Pulse_Out1 - i*10000 ;
			pulseout_qieduan3=pulseout_qieduan3+i;
			g_InteralMemory.Word[519]+=i;
			g_InteralMemory.Word[517]+=i;
		}
	}
	//抬刀
	else if(((InPulseCount[4]>(215+166+ 20)) || (g_InteralMemory.Word[517] > 162))
			&& (InPulseCount[4]<=(215+166+318+ 20)) && (g_InteralMemory.Word[518] < 500))
	{
	
		Pulse_Out1 = Pulse_Out1 + K_A3;
		
	
		
		if((Pulse_Out1 >= 10000) && (g_InteralMemory.Word[519]<onecircle_pulse))
		{
			i=Pulse_Out1 / 10000 ;
			if((g_InteralMemory.Word[519] + i) <=onecircle_pulse)
			{
				Pulse_out_function_qie(i);
				Pulse_Out1 = Pulse_Out1 - i*10000 ;
				g_InteralMemory.Word[519]+=i;
				g_InteralMemory.Word[518]+=i;
				
			}
			else 
			{
				
				i=onecircle_pulse - g_InteralMemory.Word[505];
				Pulse_out_function_qie(i);
				Pulse_Out1 = Pulse_Out1 - i*10000 ;
				g_InteralMemory.Word[519]+=i;
				g_InteralMemory.Word[518]+=i;
			}
			
		}
		
	}
	
	else
	{
		
		
		if(g_InteralMemory.Word[519]<onecircle_pulse)///如果一圈没发满onecircle_pulse个脉冲则补发剩余脉冲
		{
			
			i=onecircle_pulse-g_InteralMemory.Word[519];//
			Pulse_out_function_qie(i);
		}
		InPulseCount[4] = 0;
	
		Pulse_Out1=0;
		pulseout_qieduan3=0;
		product_start=0;
					
		g_InteralMemory.Word[516]=0;
		g_InteralMemory.Word[517]=0;
		g_InteralMemory.Word[518]=0;
		g_InteralMemory.Word[519]=0;
	//	g_InteralMemory.Word[512]=0;
		
		//g_InteralMemory.KeepBit[10]=0;///完成一次后停止,等待下一次开启
		
		g_InteralMemory.Bit[24]=0;
	}
}
///////////////////////////////////////////////////////////////////
////	高速脉冲输入X1 中断服务程序    ///////////////
////	InPulseCount[1] 为X1  输入的脉冲数	//////////////////
///////////////////////////////////////////////////////////////////
void __irq PulseIn_1_Process(void)	
{

	if((rGPFDAT >> 1) & 0x1)
	{
		InPulseCount[2]++;///定时器内用于计算速度
		 if((g_InteralMemory.Bit[24]==1) && (g_InteralMemory.Bit[49]==0))////强切
				qiangqie_function();
		 else
		 {
			if((g_InteralMemory.Bit[8]==1) || (g_InteralMemory.Bit[9]==1) ||(g_InteralMemory.Bit[10]==1)||(g_InteralMemory.Bit[30]==1) 
				||(g_InteralMemory.Bit[31]==1)||(g_InteralMemory.Bit[32]==1)||(g_InteralMemory.Bit[38]==1))////冲口定长切;定长切
				chong_qie_function();
			else if((g_InteralMemory.Bit[42]==1) || (g_InteralMemory.Bit[86]==1))/////4段冲斜口
				xiekou_chongqie_function();
			else if((g_InteralMemory.Bit[43]==1) || (g_InteralMemory.Bit[88]==1))////连冲定长切
				lianchong_qie_function();
		 }
	   	
	}

	rEINTPEND=(1<<1);
	ClearPending(BIT_EINT1);
}
///////////////////////////////////////////////////////////////////
////	高速脉冲输入X2 中断服务程序    ///////////////
////	InPulseCount[2] 为X2  输入的脉冲数	//////////////////
///////////////////////////////////////////////////////////////////
void __irq PulseIn_2_Process(void)	///冲口电机原点检测
{


	if((rGPFDAT >> 2) & 0x1)
	{
		
		
	}

	rEINTPEND=(1<<2);
	ClearPending(BIT_EINT2);
}
///////////////////////////////////////////////////////////////////
////	高速脉冲输入X3 中断服务程序    ///////////////
////	InPulseCount[3] 为X3  输入的脉冲数	//////////////////
///////////////////////////////////////////////////////////////////
void __irq PulseIn_3_Process(void)	///切断电机原点检测
{
	int i;
	for(i=0; i<20; i++);

	if((rGPFDAT >> 3) & 0x1)
	{
		
	}

	rEINTPEND=(1<<3);
	ClearPending(BIT_EINT3);
}


//////////////////////////////////////////////////////////////////////
////	高速脉冲输入X4-X6  中断服务程序     //////////////
////	InPulseCount[4] 为X4  输入的脉冲数	//////////////////
////	InPulseCount[5] 为X5  输入的脉冲数	//////////////////
////	InPulseCount[6] 为X6  输入的脉冲数	//////////////////
/////////////////////////////////////////////////////////////////////
void __irq PulseIn_4_7_Process(void)
{
	int i;
	for(i=0; i<20; i++);

		// Eint4	// 高速输入X4
	if ( !( rEINTMASK >>4  & 0x1 )  && (rEINTPEND & (1<<4)) )//取消屏蔽标志+挂起有效
	{
		if((rGPFDAT >> 4) & 0x1)
		{
			//InPulseCount[4]++;
		}

		rEINTPEND=(1<<4);		
	}
	// Eint5	// 高速输入X5
	else if ( !( rEINTMASK >>5  & 0x1 )  && (rEINTPEND & (1<<5)) )//取消屏蔽标志+挂起有效	
	
	{
		if((rGPFDAT >> 5) & 0x1)
		{
			InPulseCount[5]++;
		}

		rEINTPEND=(1<<5);		
	}
	// Eint6	// 高速输入X6
	else if ( !( rEINTMASK >>6  & 0x1 )  && (rEINTPEND & (1<<6)) )//取消屏蔽标志+挂起有效	
	
	{
		if((rGPFDAT >> 6) & 0x1)
		{
			InPulseCount[6]++;  //InPulseCount[n] 只定义了7个 InPulseCount[0]-----InPulseCount[6]
		}

		rEINTPEND=(1<<6);		
	}
	// Eint7	// 高速输入X7
	else if ( !( rEINTMASK >>7  & 0x1 )  && (rEINTPEND & (1<<7)) )//取消屏蔽标志+挂起有效	
	
	{
		if(g_InteralMemory.Bit[1] == 1)///定长切电机光电传感器
		{

			
			g_InteralMemory.Bit[1] = 0;
			g_InteralMemory.Bit[4] = 0;
			
			PulseOut_1_Stop();
			Y_Out_Init();
			g_InteralMemory.KeepWord[4] = OutPulse_Counter[1];
			
		}
		
		if(g_InteralMemory.Bit[25] == 1)///定长切电机光电传感器
		{

			
			g_InteralMemory.Bit[25] = 0;
			g_InteralMemory.Bit[4] = 0;
			
			PulseOut_1_Stop();
			Y_Out_Init();
			
		}
		
		g_InteralMemory.Bit[100] = 1 ;//定长切禁止脉冲输出
		
		rEINTPEND=(1<<7);		
	}
 
	ClearPending(BIT_EINT4_7);
}

//////////////////////////////////////////////////////////////////////
////	高速脉冲输入X8-X23  中断服务程序     //////////////
/////////////////////////////////////////////////////////////////////
void __irq PulseIn_8_23_Process(void)   // 高速输入X8-x10   
{	 
	if ( !( rEINTMASK >>8  & 0x1 )  && (rEINTPEND & (1<<8)) ) //取消屏蔽+挂起有效  冲口光电传感器
	{	 
		//////
		if(g_InteralMemory.Bit[1] == 1)
		{
			g_InteralMemory.Bit[4] = 0;
			g_InteralMemory.Bit[1] = 0;
			PulseOut_2_Stop();
			Y_Out_Init();
			g_InteralMemory.KeepWord[44] = OutPulse_Counter[2];
		}
		
		if(g_InteralMemory.Bit[26] == 1)
		{
			g_InteralMemory.Bit[4] = 0;
			g_InteralMemory.Bit[26] = 0;
			PulseOut_2_Stop();
			Y_Out_Init();
	
		}
		g_InteralMemory.Bit[101] = 1 ;//冲口禁止脉冲输出
		rEINTPEND=(1<<8);
		 
	}
	else if ( !( rEINTMASK >>9  & 0x1 )  && (rEINTPEND & (1<<9)) ) //取消屏蔽+挂起有效   定长切伺服零位信号
	{ 
			//////////变频器报警输入
		Set_Y_Value(7,0);
		if(g_InteralMemory.KeepBit[200] == 0)
			g_InteralMemory.Word[PING_CTRL] = 11;////数据计算完成后自动返回产品参数设置界面
		else
			g_InteralMemory.Word[PING_CTRL] = 45;
		InPulse1_Stop();//脉冲输入禁止
		
		rEINTPEND=(1<<9);
	}
	else if ( !( rEINTMASK >>10  & 0x1 )  && (rEINTPEND & (1<<10)) ) //取消屏蔽+挂起有效 冲口伺服零位信号 
	{
		////////
	////////冲口伺服异常报警
		Set_Y_Value(7,0);
		
		if(g_InteralMemory.KeepBit[200] == 0)
			g_InteralMemory.Word[PING_CTRL] = 12;////数据计算完成后自动返回产品参数设置界面
		else
			g_InteralMemory.Word[PING_CTRL] = 46;
		InPulse1_Stop();//脉冲输入禁止
		rEINTPEND=(1<<10);
	}
	 
	 
	ClearPending(BIT_EINT8_23);

}







///////////////////////////////////////////////////////////////
////	高速脉冲输入X1  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse1_Start(void)
{
	rEINTPEND=(1<<1);
	ClearPending(BIT_EINT1);
	
	rEINTMASK &= ~(1<<1);
	rINTMSK1 &= ~BIT_EINT1;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X1  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse1_Stop(void)
{
	rEINTMASK |=(1<<1);
	rINTMSK1 |=BIT_EINT1;
	
	rEINTPEND=(1<<1);
	ClearPending(BIT_EINT1);
}

///////////////////////////////////////////////////////////////
////	高速脉冲输入X2  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse2_Start(void)
{
	rEINTPEND=(1<<2);
	ClearPending(BIT_EINT2);
	
	rEINTMASK &= ~(1<<2);
	rINTMSK1 &= ~BIT_EINT2;
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X2  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse2_Stop(void)
{
	rEINTMASK |=(1<<2);
	rINTMSK1 |=BIT_EINT2;
	
	rEINTPEND=(1<<2);
	ClearPending(BIT_EINT2);
}

///////////////////////////////////////////////////////////////
////	高速脉冲输入X3  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse3_Start(void)
{
	rEINTPEND=(1<<3);
	ClearPending(BIT_EINT3);

	rEINTMASK &= ~(1<<3);
	rINTMSK1 &= ~BIT_EINT3;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X3  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse3_Stop(void)
{
	rEINTMASK |=(1<<3);
	rINTMSK1 |=BIT_EINT3;
	
	rEINTPEND=(1<<3);
	ClearPending(BIT_EINT3);
}

///////////////////////////////////////////////////////////////
////	高速脉冲输入X4  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse4_Start(void)
{
	rEINTPEND=(1<<4);
	ClearPending(BIT_EINT4_7);
	
	rEINTMASK &= ~(1<<4);
	rINTMSK1 &= ~BIT_EINT4_7;
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X4  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse4_Stop(void)
{
	rEINTMASK |=(1<<4);
	//rINTMSK1 |=BIT_EINT4_7;
	
	rEINTPEND=(1<<4);
	ClearPending(BIT_EINT4_7);
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X5  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse5_Start(void)
{
	rEINTPEND=(1<<5);
	ClearPending(BIT_EINT4_7);
	
	rEINTMASK &= ~(1<<5);
	rINTMSK1 &= ~BIT_EINT4_7;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X5  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse5_Stop(void)
{
	rEINTMASK |=(1<<5);
	//rINTMSK1 |=BIT_EINT4_7;
	
	
	rEINTPEND=(1<<5);
	ClearPending(BIT_EINT4_7);
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X6  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse6_Start(void)
{
	rEINTPEND=(1<<6);
	ClearPending(BIT_EINT4_7);
	
	rEINTMASK &= ~(1<<6);
	rINTMSK1 &= ~BIT_EINT4_7;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X6  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse6_Stop(void)
{
	rEINTMASK |=(1<<6);
	//rINTMSK1 |=BIT_EINT4_7;
	
	rEINTPEND=(1<<6);
	ClearPending(BIT_EINT4_7);
}


///////////////////////////////////////////////////////////////
////	高速脉冲输入X7  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse7_Start(void)
{
	rEINTPEND=(1<<7);
	ClearPending(BIT_EINT4_7);
	
	rEINTMASK &= ~(1<<7);
	rINTMSK1 &= ~BIT_EINT4_7;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X7  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse7_Stop(void)
{
	rEINTMASK |=(1<<7);
	//rINTMSK1 |=BIT_EINT4_7;
	rEINTPEND=(1<<7);
	ClearPending(BIT_EINT4_7);
	
}

///////////////////////////////////////////////////////////////
////	高速脉冲输入X8  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse8_Start(void)
{  
	rEINTPEND=(1<<8);
	ClearPending(BIT_EINT8_23);
	 
	rEINTMASK &= ~(1<<8);
	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X8  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse8_Stop(void)
{
	rEINTMASK |=(1<<8);
	//rINTMSK1 |=BIT_EINT8_23;
	
	rEINTPEND=(1<<8);
	ClearPending(BIT_EINT8_23);
	
}


///////////////////////////////////////////////////////////////
////	高速脉冲输入X9  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse9_Start(void)
{
	rEINTPEND=(1<<9);
	ClearPending(BIT_EINT8_23);
	
	rEINTMASK &= ~(1<<9);
	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X9  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse9_Stop(void)
{
	rEINTMASK |=(1<<9);
	//rINTMSK1 |=BIT_EINT8_23;
	
	rEINTPEND=(1<<9);
	ClearPending(BIT_EINT8_23);
	
}
 
///////////////////////////////////////////////////////////////
////	高速脉冲输入X10  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse10_Start(void)
{
	rEINTPEND=(1<<10);
	ClearPending(BIT_EINT8_23);
	
	rEINTMASK &= ~(1<<10);
	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X10  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse10_Stop(void)
{
	rEINTMASK |=(1<<10);
	//rINTMSK1 |=BIT_EINT8_23;
	
	rEINTPEND=(1<<10);
	ClearPending(BIT_EINT8_23);
	
}

 



/////////////////////////////////////////////////
//////	高速脉冲输入初始化     //////////////
//////	IN 1~6   6路高速输入	    //////////////
////////////////////////////////////////////////
void Pulse_In_Init(void)	
{
	DWORD tmp;
	
	//set GPF1~6 as EINT	x1-x6
	tmp = rGPFCON & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 12));
	rGPFCON = tmp | (0x2<<2) | (0x2<<4) | (0x2<<6) | (0x2<<8) | (0x2<<10) | (0x2<<12);	
	//GPF1~6 up down disable x1-x6
	rGPFUDP &=  (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 12));

	//GPF1~6  x1-x6 上升沿触发
	rEXTINT0 = (rEXTINT0 & (~(0x7<<4))) | (0x4<<4);		// Eint1	Rising edge triggered
	rEXTINT0 = (rEXTINT0 & (~(0x7<<8))) | (0x4<<8);		// Eint2	Rising edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<12))) | (0x4<<12);	// Eint3	Rising edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<16))) | (0x4<<16);	// Eint4	Rising edge triggered
	rEXTINT0 = (rEXTINT0 & (~(0x7<<20))) | (0x4<<20);	// Eint5	Rising edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<24))) | (0x4<<24);	// Eint6	Rising edge triggered	

	pISR_EINT1= (U32)PulseIn_1_Process;		// X1
	pISR_EINT2= (U32)PulseIn_2_Process;		// X2
	pISR_EINT3= (U32)PulseIn_3_Process;		// X3
	pISR_EINT4_7= (U32)PulseIn_4_7_Process;	// X4_6


	rEINTPEND = 0xffffff;

	rSRCPND1 |= BIT_EINT1|BIT_EINT2|BIT_EINT3|BIT_EINT4_7; //to clear the previous pending states
	rINTPND1 |= BIT_EINT1|BIT_EINT2|BIT_EINT3|BIT_EINT4_7;

	//rEINTMASK &= ~((1<<2)|(1<<3)|(1<<4)|(1<<6));
	//rINTMSK1 &= ~(BIT_EINT2|BIT_EINT3|BIT_EINT4_7);
	 
	InPulse1_Stop();
	InPulse2_Stop();
	InPulse3_Stop();
	InPulse4_Stop();
	InPulse5_Stop();
	InPulse6_Stop();
	
	/**********************//**********************//**********************//**********************/
	//      注意！！！上面的外部中断寄存器只配置x1-x6  之后的x7-x15要用中断形式，需提前配置       //
	/**********************//**********************//**********************//**********************/
	
	InPulse7_Stop();
	InPulse8_Stop();
	InPulse9_Stop();
	InPulse10_Stop();
	
	/*------------------x7-x15中断示例------------------*/
	
	
	//	GPF1~7 up down disable----x1~x7
	
	rGPFUDP &=  (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 12)) & (~(0x3<< 14)); 
	
	//	set GPF1~7 as EINT -----x1~x7
	
	tmp = rGPFCON & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 12))& (~(0x3<< 14));
	rGPFCON = tmp | (0x2<<2) | (0x2<<4) | (0x2<<6) | (0x2<<8) | (0x2<<10) | (0x2<<12)| (0x2<<14);		
   
   
    
    //	GPG 0 1 2---up down disable---X8 X9 X10
	
    tmp = rGPGUDP & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4));
	rGPGUDP  = tmp | (0x0<<0)| (0x0<<2)| (0x0<<4) ;
		
	//	set GPG 0 1 2 as EINT ---X8 X9 X10
	tmp = rGPGCON & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4));
	rGPGCON = tmp | (0x2<<0)| (0x2<<2)| (0x2<<4) ;  
	
	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<28))) | (0x2<<28);	// Eint7	Falling edge triggered	
	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<0))) | (0x2<<0);	    // Eint8	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<4))) | (0x2<<4);	    // Eint9	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<8))) | (0x2<<8);	    // Eint10	Falling edge triggered	
	
	pISR_EINT8_23= (U32)PulseIn_8_23_Process;	// X8 X9 X10
	
	
	
	
	 
	
	
}





///////////////////////////////////////////////////////////////////////////
////	Y1 脉冲发送中断服务程序////////
///////////////////////////////////////////////////////////////////////////
void __irq PulseOut_1_Process(void)
{
	OutPulse_Counter[1]++;
	
	
	//if(OutPulse_Counter[1] >= OutPulse_PreSet[1])
	if  (((  OutPulse_PreSet[1] > 0   )&&(OutPulse_Counter[1] >= OutPulse_PreSet[1])) || (g_InteralMemory.Word[505]>=1000))//输出脉冲大于1000停止发脉冲
	{
		if(g_InteralMemory.Bit[4]==0)
			PulseOut_1_Stop();
		
	}
	
	

	if((g_InteralMemory.Bit[24]==1) && (g_InteralMemory.Bit[49]==0))///强切
	{
		g_InteralMemory.Word[519]++;
			///下刀
		if((InPulseCount[4]>=20) && (InPulseCount[4]<=(215 + 20)) )
		{
			g_InteralMemory.Word[516]++;//监控输出脉冲数
		}
			//切断
		else if((InPulseCount[4]>(215 + 20)) && (InPulseCount[4]<=(215+166+ 20)))
		{
			g_InteralMemory.Word[517]++;//监控输出脉冲数
		}
		else if(((InPulseCount[4]>(215+166+ 20)) || (g_InteralMemory.Word[502] > 162))
			&& (InPulseCount[4]<=(215+166+318+ 20)) )
		{
			g_InteralMemory.Word[518]++;//监控输出脉冲数
		}
	}
	else
	{
		g_InteralMemory.Word[505]++;///输出脉冲计数
			///下刀
		if((InPulseCount[1]>=Inpulse_dengdai2[dingchang_current]) && (InPulseCount[1]<=(Inpulse_xiadao2 + Inpulse_dengdai2[dingchang_current])) )
		{
				g_InteralMemory.Word[501]++;//监控输出脉冲数			
		}
		//切断
		else if((InPulseCount[1]>(Inpulse_xiadao2 + Inpulse_dengdai2[dingchang_current])) && (InPulseCount[1]<=(Inpulse_xiadao2+Inpulse_qieduan2+ Inpulse_dengdai2[dingchang_current])) )
		{
				g_InteralMemory.Word[502]++;//监控输出脉冲数		
		}
		//抬刀
		else if(((InPulseCount[1]>(Inpulse_xiadao2+Inpulse_qieduan2+ Inpulse_dengdai2[dingchang_current])) || (g_InteralMemory.Word[502] > Outpulse_qieduan2))
						&& (InPulseCount[1]<=Inpulse_xiadao2+Inpulse_qieduan2+Inpulse_taidao2+ Inpulse_dengdai2[dingchang_current]) )
		{
				g_InteralMemory.Word[503]++;//监控输出脉冲数	
		}
	}
	
	ClearPending((U32)BIT_TIMER1);

}



///////////////////////////////////////////////////////////////////////////
////	Y1 脉冲输出启动//////////////////////////////
////	 每发送一段脉冲 PulseOut_1_Start 只需启动一次//////
////	frequence: 脉冲频率///////////////////////////////
////	pulse_PreSet: 发送脉冲数 ///////////////////////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_1_Start(unsigned int frequence, int pulse_PreSet)
{
	DWORD tmp;
	
	if(0 == pulse_PreSet)
	{
		PulseOut_1_Stop();
		return;
	}
	else
	{
		OutPulse_PreSet[1] = pulse_PreSet;
		OutPulse_Counter[1] = 0;	// 确保每次启动PulseOut_1_Start ，发送脉冲数 pulse_PreSet
	}
	
	if(0 == frequence)
	{
		frequence = 1;
		PulseOut_1_Stop();
		return;
	}
	
	rTCNTB1= 3030303/frequence;	// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}
	
	rTCMPB1 = rTCNTB1/2;
	
	rSRCPND1 = rSRCPND1 | ((U32)0x1<<11);   //清空定时器1源请求
    rINTPND1 = rINTPND1 | ((U32)0x1<<11);   //清空定时器1中断请求
   
	
	rINTMSK1 &=~(BIT_TIMER1);
	tmp = rTCON & (~(0xf << 8));	// dead zone Disable
	rTCON = tmp | (2 << 8);		/* update TCVNTB0, stop					*/
	rTCON = tmp | (9 << 8);		/* interval mode,  start				*/	
}

///////////////////////////////////////////////////////////////////////////
////	Y1 脉冲发送停止////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_1_Stop(void)
{
	

	rINTMSK1 |= BIT_TIMER1;
	rTCON &= ~(1 << 8);			/* Timer1, stop							*/

	//OutPulse_Counter[1] = 0;

}


///////////////////////////////////////////////////////////////////////////
////	Y2 脉冲发送中断服务程序////////
///////////////////////////////////////////////////////////////////////////
void __irq PulseOut_2_Process(void)
{
	OutPulse_Counter[2]++;
	
	
	g_InteralMemory.Word[509]++;
	//if(OutPulse_Counter[2] >= OutPulse_PreSet[2])
	if  ((  OutPulse_PreSet[2] > 0   )&&(OutPulse_Counter[2] >= OutPulse_PreSet[2]) || (g_InteralMemory.Word[509]>=1000))
	{
		if(g_InteralMemory.Bit[4]==0)
			PulseOut_2_Stop();
	}
	
	///下刀
	if((InPulseCount[3]>=Inpulse_dengdai1[chongkou_current]) && (InPulseCount[3]<(Inpulse_xiadao1 + Inpulse_dengdai1[chongkou_current])) )
	{
		
		g_InteralMemory.Word[506]++;
	}
	//切断
	else if((InPulseCount[3]>=(Inpulse_xiadao1 + Inpulse_dengdai1[chongkou_current])) && (InPulseCount[3]<(Inpulse_xiadao1+Inpulse_qieduan1+ Inpulse_dengdai1[chongkou_current])) )
	{
		
		g_InteralMemory.Word[507]++;
		
	}
	//抬刀
	else if(((InPulseCount[3]>=(Inpulse_xiadao1+Inpulse_qieduan1+ Inpulse_dengdai1[chongkou_current])) || (g_InteralMemory.Word[507] > Outpulse_qieduan1))
						&& (InPulseCount[3]<(Inpulse_xiadao1+Inpulse_qieduan1+Inpulse_taidao1+ Inpulse_dengdai1[chongkou_current])))
	{
		
		g_InteralMemory.Word[508]++;
		
	}
	
			
			
			
	ClearPending((U32)BIT_TIMER2);

}



///////////////////////////////////////////////////////////////////////////
////	Y2 脉冲输出启动//////////////////////////////
////	 每发送一段脉冲 PulseOut_2_Start 只需启动一次//////
////	frequence: 脉冲频率///////////////////////////////
////	pulse_PreSet: 发送脉冲数 ///////////////////////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_2_Start(unsigned int frequence, int pulse_PreSet)
{
	DWORD tmp;
	
	if(0 == pulse_PreSet)
	{
		PulseOut_2_Stop();
		return;
	}
	else
	{
		OutPulse_PreSet[2] = pulse_PreSet;
		OutPulse_Counter[2] = 0;	// 确保每次启动PulseOut_2_Start ，发送脉冲数 pulse_PreSet
	}
	
	if(0 == frequence)
	{
		frequence = 1;
		PulseOut_2_Stop();
		return;
	}
	
	rTCNTB2= 300300/frequence;	// // Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}
	rTCMPB2 = rTCNTB2/2;
	
	rSRCPND1 = rSRCPND1 | ((U32)0x1<<12);   //清空定时器2源请求
    rINTPND1 = rINTPND1 | ((U32)0x1<<12);    //清空定时器2中断请求
  
	
	rINTMSK1 &=~(BIT_TIMER2);
	tmp = rTCON & (~(0xf << 12));	// dead zone Disable
	rTCON = tmp | (2 << 12)	;/* update TCVNTB0, stop					*/
	rTCON = tmp | (9 << 12)	;/* interval mode,  start				*/	
}

///////////////////////////////////////////////////////////////////////////
////	Y2 脉冲发送停止////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_2_Stop(void)
{
	//DWORD tmp;

	rINTMSK1 |= BIT_TIMER2;
	rTCON &= ~(1 << 12);		/* Timer2, stop							*/

	//OutPulse_Counter[2] = 0;

	
}


/////////////高速Y3未配置////////////////////



/////////////////////////////////////////////////
//////	高速脉冲输出初始化     //////////////
//////	OUT 1~3   3路高速输出 //////////////
////////////////////////////////////////////////
void Pulse_Out_Init()	
{
	DWORD tmp;

	tmp = rGPBCON & (~(0x3<< 2)) & (~(0x3<< 4))& (~(0x3<< 6));
	rGPBCON = tmp | (0x2<<2) | (0x2<<4) | (0x2<<6);	//set GPB1 2 3 as TOUT	

	
	// Timer1 Initialize	----HS Out1
	pISR_TIMER1 = (int)PulseOut_1_Process;	// Timer ISR for HS Out1
	rTCFG0 &= ~(0xff << 0); 
	rTCFG0 |= (10 << 0); 	// Dead zone=0, Prescaler0=10(0xff)
	rTCFG1 &= ~(0xf << 4); 
	rTCFG1 |= (0x0 << 4); 	// Interrupt, Mux0=1/2
	//rTCNTB1 = 30;    //30.27273	// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}	100K
	//rTCMPB1 = 15;
	
	PulseOut_1_Stop();
	

	
	// Timer2 Initialize	----HS Out2
	pISR_TIMER2 = (int)PulseOut_2_Process;	// Timer ISR for HS Out2
	rTCFG0 &= ~(0xff << 8); 
	rTCFG0 |= (110 << 8); 	// Dead zone=0, Prescaler0=110(0xff)   注意注意注意！！！不能变 Timer 2 3 4 共用
	rTCFG1 &= ~(0xf << 8); 
	rTCFG1 |= (0x0 << 8); 	// Interrupt, Mux0=1/2
	
 	
	PulseOut_2_Stop(); 
	
	
	
/*	
	
	
	// Timer3 Initialize	----HS Out3
 	pISR_TIMER3 = (int)PulseOut_3_Process;	// Timer ISR for HS Out3
	rTCFG0 &= ~(0xff << 8); 
	rTCFG0 |= (110 << 8); 	// Dead zone=0, Prescaler0=110(0xff)   注意注意注意！！！不能变 Timer 2 3 4 共用
	rTCFG1 &= ~(0xf << 12); 
 	rTCFG1 |= (0x0 << 12); 	// Interrupt, Mux0=1/2
 	
 	PulseOut_3_Stop();
*/
}






//////////////////////////////////////////////////////////////////////
////	DA 输出中断服务程序///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void __irq DA0_10_Process(void) // 可通过示波器确定频率
{
	if(0xffff == (g_InteralMemory.Word[32] & 0xffff))
	{
		g_InteralMemory.Word[32] = 0;
		g_InteralMemory.Word[33]++;
	}
	else
		g_InteralMemory.Word[32]++;

	ClearPending((U32)BIT_TIMER3);
}


//////////////////////////////////////////////////////////////////////
////	DA 输出启动(只需启动一次)//////////////////////////
////	voltage: 输出电压(0.000~10.000V )  小数点后3 位//////////
//////////////////////////////////////////////////////////////////////
void DA0_10_Start(float voltage)
{
	DWORD tmp;

	if(voltage>10*1000)
		voltage = 10*1000;
	else if(voltage<0)
		voltage = 0;
	
	rTCNTB3= 300;	
	//rTCMPB3 = (rTCNTB3*g_InteralMemory.Word[30])/(10000*1.326);// 最大10V , 小数点后3 位10*1000
	rTCMPB3 = (rTCNTB3*voltage)/(10*1000*1.326);// 最大10V , 小数点后3 位10*1000
	if(rTCMPB3 == rTCNTB3)
		rTCMPB3 -= 1;
	
	tmp = rGPBCON & (~(0x3<< 6));
	rGPBCON = tmp | (0x2<<6);	//set GPB3 as TOUT	

	rINTMSK1 &=~(BIT_TIMER3);
	tmp = rTCON & (~(0xf << 16));	// dead zone Disable
	rTCON = tmp | (2 << 16);		/* update TCVNTB3, stop					*/
	rTCON = tmp | (9 << 16);		/* interval mode,  start				*/	
}

//////////////////////////////////////////////////////////////////////
////	DA 输出停止//////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void DA0_10_Stop(void)
{
	DWORD tmp;

	rTCNTB3= 300;	
	rINTMSK1 |= BIT_TIMER3;
	rTCON &= ~(1 << 16);			/* Timer3, stop							*/

	tmp = rGPBCON & (~(0x3<< 6));
	rGPBCON = tmp | (1<<6);	//set GPB3 as OUT	
	rGPBDAT &= ~(1<<3);
}


//////////////////////////////////////////////////////////////////////
////	DA 输出初始化///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void DA0_10_Init(void)
{
	DWORD tmp;

	tmp = rGPBCON & (~(0x3<< 6));
	rGPBCON = tmp | (0x2<<6);	//set GPB3 as TOUT	

	// Timer3 Initialize	----DA
	pISR_TIMER3 = (int)DA0_10_Process;	// Timer ISR for DA
	rTCFG0 &= ~(0xff << 8); 
	rTCFG0 |= (110 << 8); 	// Dead zone=0, Prescaler0=110(0xff)   Timer 2 3 4 共用
	rTCFG1 &= ~(0xf << 12); 
	rTCFG1 |= (0x0 << 12); 	// Interrupt, Mux0=1/2
	rTCNTB3 = 300;    			// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}	1K
	rTCMPB3 = 150;
	DA0_10_Stop();
}



/////////////////////////////////////////////////
//////	普通输入初始化     //////////////
//////	IN7~IN15 共9 路		//////////////
////////////////////////////////////////////////
void X_In_Init(void)	
{
	DWORD tmp;

	tmp = rGPFCON & (~(0x3<< 14));
	rGPFCON = tmp |(0x0<<14);			
	//////////////
	tmp = rGPGCON & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8))
					 & (~(0x3<< 10)) & (~(0x3<< 12)) & (~(0x3<< 14));
	
	rGPGCON = tmp | (0x0<<0) | (0x0<<2) | (0x0<<4) | (0x0<<6) | (0x0<<8) | (0x0<<10) | (0x0<<12) | (0x0<<14);			
	//////////	
}



/////////////////////////////////////////////////
//////	普通输出初始化     //////////////
//////	Y2~Y15 共14 路	 //////////////
////////////////////////////////////////////////
void Y_Out_Init(void)		
{
	DWORD tmp;

	tmp = rGPECON & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 12)) & (~(0x3<< 14));
	
	
	rGPECON = tmp | (0x1<<0) | (0x1<<2) | (0x1<<4) | (0x1<<6) | (0x1<<8) | (0x1<<10) | (0x1<<12) | (0x1<<14);	
	// 输入输出都反向
	rGPEDAT |= (1<<0);	// OUT4		
	rGPEDAT |= (1<<1);	// OUT5		
	rGPEDAT |= (1<<2);	// OUT6		
	rGPEDAT |= (1<<3);	// OUT7		
	rGPEDAT |= (1<<4);	// OUT8		
	rGPEDAT |= (1<<5);	// OUT9		
	rGPEDAT |= (1<<6);	// OUT10	
	rGPEDAT |= (1<<7);	// OUT11	

	
	tmp=rGPBCON & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6));
	rGPBCON = tmp | (0x1<<2)| (0x1<<4)| (0x1<<6);
	
	rGPBDAT |= (1<<1);
	rGPBDAT |= (1<<2);
	rGPBDAT |= (1<<3);
}





////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////	输入   IN7-IN15//////////////////////
////	返回: 端口状态
////////////////////////////////////////////////////////////////
unsigned char Get_X_Value(unsigned char X_num)
{
	unsigned char X_value=0xff;

	switch(X_num)
	{	
		case 7:
			X_value = (rGPFDAT >> 7) & 0x1;	// X7
			break;
		case 8:
			X_value = (rGPGDAT >> 0) & 0x1;	// X8	
			break;
		case 9:
			X_value = (rGPGDAT >> 1) & 0x1;	// X9
			break;
		case 10:
			X_value = (rGPGDAT >> 2) & 0x1;	// X10
			break;
		case 11:
			X_value = (rGPGDAT >> 3) & 0x1;	// X11
			break;
		case 12:
			X_value = (rGPGDAT >> 4) & 0x1;	// X12
			break;
		case 13:
			X_value = (rGPGDAT >> 5) & 0x1;	// X13
			break;
		case 14:
			X_value = (rGPGDAT >> 6) & 0x1;	// X14
			break;
		case 15:
			X_value = (rGPGDAT >> 7) & 0x1;	// X15
			break;
		
		
	}

	return !X_value;// xuzhiqin tixing xiugai
}


////////////////////////////////////////////////////////////////
////	输出   Y2~Y15    /////////////////////////
////	Y_num:2~15		Value:   0或1    		//////
////////////////////////////////////////////////////////////////
void Set_Y_Value(unsigned char Y_num, unsigned char Value)
{
	Value = !Value;// xuzhiqin tixing xiugai
	
	switch(Y_num)
	{
	case 4:
		(Value) ? (rGPEDAT |= (1<<0)) : (rGPEDAT &= ~(1<<0));	// Y4	
		break;
	case 5:
		(Value) ? (rGPEDAT |= (1<<1)) : (rGPEDAT &= ~(1<<1));	// Y5	
		break;
	case 6:
		(Value) ? (rGPEDAT |= (1<<2)) : (rGPEDAT &= ~(1<<2));	// Y6	
		break;
	case 7:
		(Value) ? (rGPEDAT |= (1<<3)) : (rGPEDAT &= ~(1<<3));	// Y7	
		break;
	case 8:
		(Value) ? (rGPEDAT |= (1<<4)) : (rGPEDAT &= ~(1<<4));	// Y8	
		break;
	case 9:
		(Value) ? (rGPEDAT |= (1<<5)) : (rGPEDAT &= ~(1<<5));	// Y9	
		break;
	case 10:
		(Value) ? (rGPEDAT |= (1<<6)) : (rGPEDAT &= ~(1<<6));	// Y10	
		break;
	case 11:
		(Value) ? (rGPEDAT |= (1<<7)) : (rGPEDAT &= ~(1<<7));	// Y11	
		break;
	}

	
}





