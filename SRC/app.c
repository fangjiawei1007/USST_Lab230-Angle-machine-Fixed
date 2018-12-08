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

#include "..\inc\app.h"
#include "..\inc\appBase.h"





extern INTERALMEMORY g_InteralMemory;

//extern GLOBALVAR g_GlobalVar;

//extern INT16U PreScrNum;


extern unsigned int InPulseCount[7];		//��¼6·�����������������

extern unsigned int OutPulse_PreSet[4];	// 3·���������  ����Ԥ��ֵ
extern unsigned int OutPulse_Counter[4];	// 3·���������  ��ǰ���������


extern int Pulse_Out1,nn,Pulse_Out2 ;


char Chongkoushu;
unsigned int K_A2,dK2,Inpulse_xiadao2,Inpulse_qieduan2,Inpulse_taidao2,Inpulse_dengdai2[2],Inpulse_all2,Outpulse_xiadao2,Outpulse_qieduan2,Outpulse_taidao2;////���������ȫ�ֱ���
char Dingchang_on_off,Chongkou_on_off;
unsigned int K_A1,dK1,Inpulse_xiadao1,Inpulse_qieduan1,Inpulse_taidao1,Outpulse_xiadao1,Outpulse_qieduan1,Outpulse_taidao1;////������ȫ�ֱ���
unsigned int Inpulse_dengdai1[100],Inpulse_all1[13];
unsigned int K_A3;
	   
INT16U K_A1_index[500],K_A2_index[500],K_A3_index[180];

extern char chongkou_current,dingchang_current;
extern char qieduan_enable,need_qieduan,current_qieduan,cut_all;
extern int pulseout_qieduan1,pulseout_qieduan2,pulseout_qieduan3;
INT16U chongkou_last_length;
extern int Enable_qieduan[10];
double Gunzhijing;
char current_product;

int Dingchang1_1,Dingchang2_1,Dingchang3_1;////��������������
int Dingchang1_2,Dingchang2_2,Dingchang3_2;////�����������������ߴ�����
///////////////////////////////////////////////////////////////
//////	��ʼ��     //////////////////////////////////////////
//////	ÿ���ϵ�App_Init ִֻ��һ��	 //////////////////
//////////////////////////////////////////////////////////////
void App_Init(void)
{
	int i;
	
	X_In_Init();
	Y_Out_Init();
	Pulse_In_Init();
	//Pulse_Out_Init();
	//	DA0_10_Init();

	// ���������ʼ��
	for(i=0; i<7; i++)
	{
		InPulseCount[i] = 0;
	}

	


}


///////////////////////////////////////////////////////////////
//////	Ӧ��������     //////////////////////////////////////////
//////	ϵͳÿ��ѭ�������һ��App_Operation	 //////////////////
//////////////////////////////////////////////////////////////
///////2014.3.25 finished  --- wangliang 
void Hujiaoji_Init(void)
{
	int i;
	double j,a,temp1,temp2;
	Timer3_Init();
	InPulse1_Stop();
	
	
	InPulse7_Start();//����������
	InPulse8_Start();//��ڴ�����
	InPulse9_Start();//��Ƶ����������
	InPulse10_Start();//�ŷ���������
	
	Set_Y_Value(7,1);//��Ƶ��FWD����
	
	if(g_InteralMemory.KeepBit[100] > 1)//100  101���ڵ㶯
		g_InteralMemory.KeepBit[100]=0;
	if(g_InteralMemory.KeepBit[101] > 1)
		g_InteralMemory.KeepBit[101]=0;
	
	
	if(g_InteralMemory.KeepBit[200] > 1)
		g_InteralMemory.KeepBit[200] = 0;
			
	//for(i=0;i<100;i++)
	//g_InteralMemory.KeepWord[i]=0;
	
	if(g_InteralMemory.KeepWord[0] > 60000)
		g_InteralMemory.KeepWord[0]=125;///�����ж�ƫ�ľ�
	if(g_InteralMemory.KeepWord[2] > 60000)	
		g_InteralMemory.KeepWord[2]=510;//��������ֱ��
	if(g_InteralMemory.KeepWord[41] > 60000)	
		g_InteralMemory.KeepWord[41]=90;//���ƫ�ľ�
	if(g_InteralMemory.KeepWord[42] > 60000)
		g_InteralMemory.KeepWord[42]=7500;//����
	
	if(g_InteralMemory.KeepWord[4] >1000)
		g_InteralMemory.KeepWord[4]=500;
	if(g_InteralMemory.KeepWord[44]>1000)
		g_InteralMemory.KeepWord[44]=500;
		
	if(g_InteralMemory.KeepWord[600]>10000)////����������
		g_InteralMemory.KeepWord[600]=2500;
	
	
	//g_InteralMemory.KeepWord[16]=1000;
	//g_InteralMemory.KeepWord[3]=50;
	
	
	//g_InteralMemory.KeepWord[200]=0;
	//g_InteralMemory.KeepWord[17]=1500;
	//g_InteralMemory.KeepWord[1]=3000;
	
	for(i=18;i<=40;i+=2)
	{
		if(g_InteralMemory.KeepWord[i] > 2000)
			g_InteralMemory.KeepWord[i]=1000;
	}
	

	g_InteralMemory.Word[501]=0;
	g_InteralMemory.Word[502]=0;
	g_InteralMemory.Word[503]=0;
	g_InteralMemory.Word[504]=0;
	g_InteralMemory.Word[505]=0;
	g_InteralMemory.Word[506]=0;
	g_InteralMemory.Word[507]=0;
	g_InteralMemory.Word[508]=0;
	g_InteralMemory.Word[509]=0;////��������������
	g_InteralMemory.Word[511]=0;
	InPulseCount[1] = 0;
	InPulseCount[3] = 0;
	InPulseCount[4] = 0;
	
	chongkou_current = 0;
	dingchang_current = 0;
	current_qieduan = 0;
	need_qieduan = 0;
	pulseout_qieduan1 = 0;
	pulseout_qieduan2 = 0;
	pulseout_qieduan3 = 0;
	qieduan_enable = 0;
	cut_all = 0;
	Dingchang_on_off = 0;
	Chongkou_on_off = 0;
	
	for(i=0; i<10; i++)
		Enable_qieduan[i]=0;
	
	
	g_InteralMemory.Bit[8] = 0;
	g_InteralMemory.Bit[9] = 0;
	g_InteralMemory.Bit[10] = 0;
	g_InteralMemory.Bit[30] = 0;
	g_InteralMemory.Bit[31] = 0;
	g_InteralMemory.Bit[32] = 0;
	g_InteralMemory.Bit[42] = 0;
	g_InteralMemory.Bit[43] = 0;
	
	
	////////////////////////////////////////////////////
	//��Ʒ������ʼ��
	
	if(g_InteralMemory.KeepBit[201] == 1)
		g_InteralMemory.Bit[11] = 1;	
	else if(g_InteralMemory.KeepBit[202] == 1)
		g_InteralMemory.Bit[12] = 1;
	else if((g_InteralMemory.KeepBit[203] == 1) && (g_InteralMemory.KeepBit[204] == 1) && (g_InteralMemory.KeepBit[205] == 1))
		g_InteralMemory.Bit[36] = 1;
	else if(g_InteralMemory.KeepBit[203] == 1)
		g_InteralMemory.Bit[33] = 1;
	else if(g_InteralMemory.KeepBit[204] == 1)
		g_InteralMemory.Bit[34] = 1;
	else if(g_InteralMemory.KeepBit[205] == 1)
		g_InteralMemory.Bit[35] = 1;
	else if(g_InteralMemory.KeepBit[206] == 1)
		g_InteralMemory.Bit[47] = 1;
	else if(g_InteralMemory.KeepBit[207] == 1)
		g_InteralMemory.Bit[85] = 1;
	else if(g_InteralMemory.KeepBit[208] == 1)
		g_InteralMemory.Bit[48] = 1;
	else if(g_InteralMemory.KeepBit[209] == 1)
		g_InteralMemory.Bit[87] = 1;
			
	if(g_InteralMemory.KeepWord[500] > 60000)
		g_InteralMemory.KeepWord[500] = 10;//λ��ƫ��
	if(g_InteralMemory.KeepWord[169] > 60000)
		g_InteralMemory.KeepWord[169] = 10;//λ��ƫ��
	if(g_InteralMemory.KeepWord[190] > 60000)
		g_InteralMemory.KeepWord[190] = 25;//�������
	if(g_InteralMemory.KeepWord[191] > 60000)
		g_InteralMemory.KeepWord[191] = 55;//
	if(g_InteralMemory.KeepWord[192] > 60000)
		g_InteralMemory.KeepWord[192] = 10;//
	if(g_InteralMemory.KeepWord[193] > 60000)
		g_InteralMemory.KeepWord[193] = 3000;//
	if(g_InteralMemory.KeepWord[194] > 60000)
		g_InteralMemory.KeepWord[194] = 30;//
	
	
	for(i=17;i<=39;i+=2)
	{
		if(g_InteralMemory.KeepWord[i] > 65000)
			g_InteralMemory.KeepWord[i]=0;
	}
	if(g_InteralMemory.KeepWord[1]>65000)
		g_InteralMemory.KeepWord[1]=1000;
	if(g_InteralMemory.KeepWord[16]>1000)
		g_InteralMemory.KeepWord[16]=1000;
	if(g_InteralMemory.KeepWord[3]>60000)
		g_InteralMemory.KeepWord[3]=50;
	for(i=350;i<=374;i+=2)
	{
		if(g_InteralMemory.KeepWord[i] > 65000)
			g_InteralMemory.KeepWord[i]=0;
	}
	for(i=351;i<=375;i+=2)
	{
		if(g_InteralMemory.KeepWord[i] > 65000)
			g_InteralMemory.KeepWord[i]=1000;
	}
	if(g_InteralMemory.KeepWord[376]>60000)
		g_InteralMemory.KeepWord[376]=50;
		
		
	if(g_InteralMemory.KeepWord[80]>60000)
		g_InteralMemory.KeepWord[80]=100;
	if(g_InteralMemory.KeepWord[84]>60000)
		g_InteralMemory.KeepWord[84]=100;
	if(g_InteralMemory.KeepWord[88]>60000)
		g_InteralMemory.KeepWord[88]=100;
	
	if(g_InteralMemory.KeepWord[82]>60000)
		g_InteralMemory.KeepWord[82]=1000;
	if(g_InteralMemory.KeepWord[86]>60000)
		g_InteralMemory.KeepWord[86]=1000;
	if(g_InteralMemory.KeepWord[90]>60000)
		g_InteralMemory.KeepWord[90]=1000;
	if(g_InteralMemory.KeepWord[170]>60000)
		g_InteralMemory.KeepWord[170]=50;
	
	
	
	for(i=104;i<=110;i+=2)
	{
		if(g_InteralMemory.KeepWord[i] > 65000)
			g_InteralMemory.KeepWord[i]=100;
	}
	for(i=105;i<=111;i+=2)
	{
		if(g_InteralMemory.KeepWord[i] > 65000)
			g_InteralMemory.KeepWord[i]=1000;
	}
	if(g_InteralMemory.KeepWord[112]>60000)
		g_InteralMemory.KeepWord[112]=5;
	if(g_InteralMemory.KeepWord[168]>60000)
		g_InteralMemory.KeepWord[168]=3;
	
	for(i=450;i<=456;i+=2)
	{
		if(g_InteralMemory.KeepWord[i] > 65000)
			g_InteralMemory.KeepWord[i]=100;
	}
	for(i=451;i<=457;i+=2)
	{
		if(g_InteralMemory.KeepWord[i] > 65000)
			g_InteralMemory.KeepWord[i]=1000;
	}
	if(g_InteralMemory.KeepWord[458]>60000)
		g_InteralMemory.KeepWord[458]=5;
	if(g_InteralMemory.KeepWord[459]>60000)
		g_InteralMemory.KeepWord[459]=3;
		
	
	
	
	for(i=135;i<=139;i+=2)
	{
		if(g_InteralMemory.KeepWord[i] > 65000)
			g_InteralMemory.KeepWord[i]=100;
	}
	for(i=136;i<=140;i+=2)
	{
		if(g_InteralMemory.KeepWord[i] > 65000)
			g_InteralMemory.KeepWord[i]=1000;
	}
	
	if(g_InteralMemory.KeepWord[141]>60000)
		g_InteralMemory.KeepWord[141]=5;
	
	for(i=501;i<=505;i+=2)
	{
		if(g_InteralMemory.KeepWord[i] > 65000)
			g_InteralMemory.KeepWord[i]=100;
	}
	for(i=502;i<=506;i+=2)
	{
		if(g_InteralMemory.KeepWord[i] > 65000)
			g_InteralMemory.KeepWord[i]=1000;
	}
	
	if(g_InteralMemory.KeepWord[507]>60000)
		g_InteralMemory.KeepWord[507]=5;
			
	//////////////////////////////ǿ��ͬ����ʼ��	   
	j = (12.5 - 6) / 12.5 ;
	a=acos(j);//�жϹ��̵�ת���ĽǶ�
	
	K_A3 = 10000 * 1000 * 51 / 5000.0 / 12.5 / j ;
	
	temp1 = a / 162;
	for(i=0;i<162;i++)
	{
		temp2 = a-i*temp1;
		if(temp2 >= 0.0)
			K_A3_index[i]=10000 * 1000 * 51 / 5000.0 / 12.5 / cos(temp2) ;
		else
			K_A3_index[i]=10000 * 1000 * 51 / 5000.0 / 12.5 / j ;//A��KֵK_A2
	}
	
	if(g_InteralMemory.KeepWord[260] > 3000 )
		g_InteralMemory.KeepWord[260] = 1000;
}



void App_Operation(void)
{

/*
g_InteralMemory.Bit[]
g_InteralMemory.KeepBit[]
g_InteralMemory.Word[]
g_InteralMemory.KeepWord[]
InPulseCount[7]
*/	
	
	int j,tmp;
	double i,a,Pianxinju_1,Pianxinju_2,Dingchang,Houdu,Daojianju,temp1,temp2;//Pianxinju_1Ϊ����Pianxinju_2Ϊ�жϻ�
	double Duanchang1,Duanchang2,Duanchang3,Duanchang4,Duanchang5,Duanchang6,Duanchang7,Duanchang8,Duanchang9,Duanchang10,Duanchang11,Duanchang12;
	double Dingchang1,Dingchang2,Dingchang3;
	
	
	if((g_InteralMemory.Bit[30] == 1) || (g_InteralMemory.Bit[31] == 1) || (g_InteralMemory.Bit[32] == 1))
	{
		Set_Y_Value(11,1);//��ڵ�������ֹ
	}
	else
	{
		Set_Y_Value(11,0);
	}
	
	Gunzhijing= (double)g_InteralMemory.KeepWord[2] / 10.0 ;
	
	///////////////////////////////////////////////////////////////////
	//�����л�
	if(g_InteralMemory.Bit[97] == 1)
	{
		g_InteralMemory.Bit[97] = 0;
		
		if(g_InteralMemory.KeepBit[200] == 1)
			g_InteralMemory.KeepBit[200] = 0;
		else
			g_InteralMemory.KeepBit[200] = 1;
	}
	/////��������Ϊ���籣����
	g_InteralMemory.KeepBit[201] = g_InteralMemory.Bit[8];
	g_InteralMemory.KeepBit[202] = g_InteralMemory.Bit[9];
	g_InteralMemory.KeepBit[203] = g_InteralMemory.Bit[30];
	g_InteralMemory.KeepBit[204] = g_InteralMemory.Bit[31];
	g_InteralMemory.KeepBit[205] = g_InteralMemory.Bit[32];
	g_InteralMemory.KeepBit[206] = g_InteralMemory.Bit[42];
	g_InteralMemory.KeepBit[207] = g_InteralMemory.Bit[86];
	g_InteralMemory.KeepBit[208] = g_InteralMemory.Bit[43];
	g_InteralMemory.KeepBit[209] = g_InteralMemory.Bit[88];
	
	///////////////////////////
	///���ٴ���
	if(g_InteralMemory.Word[500] > (g_InteralMemory.KeepWord[190] * 10))
	{
		if((g_InteralMemory.Bit[8] == 1) || (g_InteralMemory.Bit[9] == 1) || (g_InteralMemory.Bit[42] == 1) || (g_InteralMemory.Bit[86] == 1)
			 || (g_InteralMemory.Bit[43] == 1) || (g_InteralMemory.Bit[88] == 1))
		{
			g_InteralMemory.Bit[23] = 1;
			
			if(g_InteralMemory.KeepBit[200] == 0)
				g_InteralMemory.Word[PING_CTRL] = 14;
			else
				g_InteralMemory.Word[PING_CTRL] = 47;
		}
	}
	
	if(g_InteralMemory.Word[500] > (g_InteralMemory.KeepWord[192] * 10))
	{
		if((g_InteralMemory.Bit[38] == 1))
		{
			if((current_product == 0) && (g_InteralMemory.KeepWord[191] > (g_InteralMemory.KeepWord[80] * g_InteralMemory.KeepWord[82] / 1000) ))
			{
				g_InteralMemory.Bit[23] = 1;
			
				if(g_InteralMemory.KeepBit[200] == 0)
					g_InteralMemory.Word[PING_CTRL] = 14;
				else
					g_InteralMemory.Word[PING_CTRL] = 47;
			}
			else if((current_product == 1) && (g_InteralMemory.KeepWord[191] > (g_InteralMemory.KeepWord[84] * g_InteralMemory.KeepWord[86] / 1000) ))
			{
				g_InteralMemory.Bit[23] = 1;
			
				if(g_InteralMemory.KeepBit[200] == 0)
					g_InteralMemory.Word[PING_CTRL] = 14;
				else
					g_InteralMemory.Word[PING_CTRL] = 47;
			}
			else if((current_product == 2) && (g_InteralMemory.KeepWord[191] > (g_InteralMemory.KeepWord[88] * g_InteralMemory.KeepWord[90] / 1000) ))
			{
				g_InteralMemory.Bit[23] = 1;
			
				if(g_InteralMemory.KeepBit[200] == 0)
					g_InteralMemory.Word[PING_CTRL] = 14;
				else
					g_InteralMemory.Word[PING_CTRL] = 47;
			}
		}
		else if((g_InteralMemory.Bit[39] == 1) && (g_InteralMemory.KeepWord[191] > (g_InteralMemory.KeepWord[80] * g_InteralMemory.KeepWord[82] / 1000) ))
		{
			g_InteralMemory.Bit[23] = 1;
			
			if(g_InteralMemory.KeepBit[200] == 0)
				g_InteralMemory.Word[PING_CTRL] = 14;
			else
				g_InteralMemory.Word[PING_CTRL] = 47;
		}
		else if((g_InteralMemory.Bit[40] == 1) && (g_InteralMemory.KeepWord[191] > (g_InteralMemory.KeepWord[84] * g_InteralMemory.KeepWord[86] / 1000) ))
		{
			g_InteralMemory.Bit[23] = 1;
			
			if(g_InteralMemory.KeepBit[200] == 0)
				g_InteralMemory.Word[PING_CTRL] = 14;
			else
				g_InteralMemory.Word[PING_CTRL] = 47;
		}
		else if((g_InteralMemory.Bit[41] == 1) && (g_InteralMemory.KeepWord[191] > (g_InteralMemory.KeepWord[88] * g_InteralMemory.KeepWord[90] / 1000) ))
		{
			g_InteralMemory.Bit[23] = 1;
			
			if(g_InteralMemory.KeepBit[200] == 0)
				g_InteralMemory.Word[PING_CTRL] = 14;
			else
				g_InteralMemory.Word[PING_CTRL] = 47;
		}
	}
	
	if(g_InteralMemory.Word[500] > (g_InteralMemory.KeepWord[194] * 10))
	{
		if((g_InteralMemory.Bit[39] == 1) && (g_InteralMemory.KeepWord[193] > (g_InteralMemory.KeepWord[80] * g_InteralMemory.KeepWord[82] / 1000) ))
		{
			g_InteralMemory.Bit[23] = 1;
			
			if(g_InteralMemory.KeepBit[200] == 0)
				g_InteralMemory.Word[PING_CTRL] = 14;
			else
				g_InteralMemory.Word[PING_CTRL] = 47;
		}
		else if((g_InteralMemory.Bit[40] == 1) && (g_InteralMemory.KeepWord[193] > (g_InteralMemory.KeepWord[84] * g_InteralMemory.KeepWord[86] / 1000) ))
		{
			g_InteralMemory.Bit[23] = 1;
			
			if(g_InteralMemory.KeepBit[200] == 0)
				g_InteralMemory.Word[PING_CTRL] = 14;
			else
				g_InteralMemory.Word[PING_CTRL] = 47;
		}
		else if((g_InteralMemory.Bit[41] == 1) && (g_InteralMemory.KeepWord[193] > (g_InteralMemory.KeepWord[88] * g_InteralMemory.KeepWord[90] / 1000) ))
		{
			g_InteralMemory.Bit[23] = 1;
			
			if(g_InteralMemory.KeepBit[200] == 0)
				g_InteralMemory.Word[PING_CTRL] = 14;
			else
				g_InteralMemory.Word[PING_CTRL] = 47;
		}
	}
	
	
/////�趨�����ж�B�ο��㣬��͵�
	if(g_InteralMemory.Bit[0] == 1)
	{
		OutPulse_Counter[1] = 0;///�������������
		g_InteralMemory.Bit[0]=0;
	}
	if(g_InteralMemory.Bit[18] == 1)
	{
		OutPulse_Counter[2] = 0;///�������������
		g_InteralMemory.Bit[18]=0;
	}
//////////////
	if(g_InteralMemory.Bit[3] == 1)//��ԭ��--�����ж�
	{
		
		Pulse_Out_Init();
		PulseOut_1_Stop();
		PulseOut_1_Start(200,1500);
		g_InteralMemory.Bit[20]=0;////����ŷ���λ��ʼ��־λ
		g_InteralMemory.Bit[4] = 1;///��ԭ��㶯��־λ����ֹ������峬��1000�󲻷�����
		g_InteralMemory.Bit[3] = 0;
		
		g_InteralMemory.Bit[1] = 1;/////�趨�е�ԭ���־λx7�ж���
		
	}
	if(g_InteralMemory.Bit[17] == 1)//��ԭ��--���
	{
		
		Pulse_Out_Init();
		PulseOut_2_Stop();
		PulseOut_2_Start(200,1500);
		g_InteralMemory.Bit[21]=0;////����ŷ���λ��ʼ��־λ
		g_InteralMemory.Bit[4] = 1;///��ԭ��㶯��־λ����ֹ������峬��1000�󲻷�����
		g_InteralMemory.Bit[17] = 0;
		
		g_InteralMemory.Bit[1] = 1;/////�趨�е�ԭ���־λx8�ж���
		
	}
	
		////*******************************
	/////////////
	//////////////
	if(g_InteralMemory.Bit[21] == 1)//��ԭ��--�����ж�  ������ԭ�㲻ȷ����͵㵽ֹͣ���������
	{
		Pulse_Out_Init();
		PulseOut_1_Stop();
		PulseOut_1_Start(200,1500);
		
		g_InteralMemory.Bit[4] = 1;///��ԭ��㶯��־λ����ֹ������峬��1000�󲻷�����
		g_InteralMemory.Bit[21] = 0;
		
		g_InteralMemory.Bit[25] = 1;/////�趨�е�ԭ���־λx7�ж���
		
	}
	if(g_InteralMemory.Bit[20] == 1)//��ԭ��--���   ������ԭ�㲻ȷ����͵㵽ֹͣ���������
	{
		Pulse_Out_Init();
		PulseOut_2_Stop();
		PulseOut_2_Start(200,1500);
		
		g_InteralMemory.Bit[4] = 1;///��ԭ��㶯��־λ����ֹ������峬��1000�󲻷�����
		g_InteralMemory.Bit[20] = 0;
		
		g_InteralMemory.Bit[26] = 1;/////�趨�е�ԭ���־λx8�ж���
		
	}

////////////////////////
	////�㶯--�����ж�	
	if((g_InteralMemory.Bit[14]==1) && (g_InteralMemory.Bit[15]==0))
	{
		Pulse_Out_Init();
		PulseOut_1_Stop();
		PulseOut_1_Start(200,50000);
		g_InteralMemory.Bit[15]=1;
		
		g_InteralMemory.Bit[4] = 1;
	}
	if((g_InteralMemory.Bit[14]==0) && (g_InteralMemory.Bit[15]==1))
	{
		PulseOut_1_Stop();
		Y_Out_Init();
		g_InteralMemory.Bit[15]=0;
		g_InteralMemory.Bit[4] = 0;
		
	}
	////�㶯--���	
	if((g_InteralMemory.Bit[16]==1) && (g_InteralMemory.Bit[19]==0))
	{
		Pulse_Out_Init();
		PulseOut_2_Stop();
		PulseOut_2_Start(200,50000);
		g_InteralMemory.Bit[19]=1;
		
		g_InteralMemory.Bit[4] = 1;
	}
	if((g_InteralMemory.Bit[16]==0) && (g_InteralMemory.Bit[19]==1))
	{
		PulseOut_2_Stop();
		
		Y_Out_Init();
		g_InteralMemory.Bit[19]=0;
		g_InteralMemory.Bit[4] = 0;
		
	}
	/////////////////////////////////////////////
	if(g_InteralMemory.Bit[5] == 1)//��������
	{
		g_InteralMemory.KeepWord[10]=0;
		g_InteralMemory.Bit[5]=0;
	}
	
	////////////////////////////////////////////////////////////////
		
	if((g_InteralMemory.Bit[22] == 1) && (g_InteralMemory.Bit[27] == 0))//////������ť
	{
		Y_Out_Init();
		Timer3_Start();//��ʼ����
		InPulse1_Start();//��������
		Set_Y_Value(7,1);//��Ƶ��FWD����
		
		g_InteralMemory.Bit[23]=0;
		g_InteralMemory.Bit[27]=1;
		g_InteralMemory.Bit[28]=0;
		g_InteralMemory.Bit[29]=1;///״ָ̬ʾ��
	}
	
	if((g_InteralMemory.Bit[23] == 1) && (g_InteralMemory.Bit[28] == 0)&& (g_InteralMemory.Bit[300] == 0)&& (g_InteralMemory.Bit[301] == 0))
	{
	//	Timer3_Stop();////����
		InPulse1_Stop();//��������
		//Set_Y_Value(7,0);//��Ƶ��FWDֹͣ
		
		g_InteralMemory.Bit[22]=0;
		g_InteralMemory.Bit[27]=0;
		g_InteralMemory.Bit[28]=1;
		g_InteralMemory.Bit[29]=0;///״ָ̬ʾ��
	}
	
	
	
	
	
	
	
	
	
	
////////*****************************************************************///////////
////////*****************************************************************///////////
/////////������������д��,ֻ�е����ťʱ����һ������
///////////
	if(g_InteralMemory.Bit[2] == 1)///////////////��Ʒһ����д��
	{	
	
	///////////////////////////////////
	//////������
		if (g_InteralMemory.KeepWord[1] > 0)///��������0����Ϊ��Ҫ�����ж�
		{
		  Pianxinju_2 = (double)g_InteralMemory.KeepWord[0] / 10.0 ;/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
		//	Dingchang = (double)(g_InteralMemory.KeepWord[1] + g_InteralMemory.KeepWord[16]) / 10.0 ;///kw16���Ȳ���
		Dingchang = (double)(g_InteralMemory.KeepWord[1]) * g_InteralMemory.KeepWord[16] / 1000 ;///kw16���Ȳ���
			Gunzhijing= (double)g_InteralMemory.KeepWord[2] / 10.0 ;
			   Houdu  = (double)g_InteralMemory.KeepWord[3] / 10.0 ;
		 
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_2)   
			Houdu = Pianxinju_2 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 
			   
			i = (Pianxinju_2 - Houdu) / Pianxinju_2 ;
			a=acos(i);//�жϹ��̵�ת���ĽǶ�
			
			g_InteralMemory.KeepWord[5] = onecircle_pulse * a / 2.0 / PI ;//�жϹ�����Ҫ������������P_KW5
			
			g_InteralMemory.KeepWord[6] = onecircle_pulse - g_InteralMemory.KeepWord[5] - g_InteralMemory.KeepWord[4] ;//�µ����̷�����������
			
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
			g_InteralMemory.KeepWord[7] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / i;//A��KֵK_A2
			
			g_InteralMemory.KeepWord[11] = Dingchang * Encoder_Pulse / PI / Gunzhijing ;////����������������
			g_InteralMemory.KeepWord[12] = 10000 * g_InteralMemory.KeepWord[6] / g_InteralMemory.KeepWord[7];////�µ����̱�����������
				
			g_InteralMemory.KeepWord[13] = Pianxinju_2 * sin(a) * Encoder_Pulse / PI / Gunzhijing ;////�жϹ��̱�����������
			
			g_InteralMemory.KeepWord[14] = 10000 * g_InteralMemory.KeepWord[4] / g_InteralMemory.KeepWord[7];////̧�����̱�����������
		
			g_InteralMemory.KeepWord[15] = g_InteralMemory.KeepWord[11] - g_InteralMemory.KeepWord[12] - g_InteralMemory.KeepWord[13] - g_InteralMemory.KeepWord[14];////ԭ��ȴ����̱�����������
			////ע�⣺�жϹ��̱����������� �д�ʵ����תʱ������
		///////////////////////
		/*
			temp1 = a / g_InteralMemory.KeepWord[5];
			for(j=0;j<g_InteralMemory.KeepWord[5];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0.0)
					K_A2_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / cos(temp2) ;
				else
					K_A2_index[j]=g_InteralMemory.KeepWord[7];
			}*/
		
		}
	////////���
	/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
		if (g_InteralMemory.KeepWord[17] > 0)//��һ�γ��ȴ���0����Ϊ�ӹ����
		{
			Pianxinju_1 = (double)g_InteralMemory.KeepWord[41] / 10.0 ;
			Duanchang1  = (double)(g_InteralMemory.KeepWord[17] / 10.0) * g_InteralMemory.KeepWord[18] / 1000  ;
			Duanchang2  = (double)(g_InteralMemory.KeepWord[19] / 10.0) * g_InteralMemory.KeepWord[20] / 1000  ;			
			Duanchang3  = (double)(g_InteralMemory.KeepWord[21] / 10.0) * g_InteralMemory.KeepWord[22] / 1000  ;			
			Duanchang4  = (double)(g_InteralMemory.KeepWord[23] / 10.0) * g_InteralMemory.KeepWord[24] / 1000  ;
			Duanchang5  = (double)(g_InteralMemory.KeepWord[25] / 10.0) * g_InteralMemory.KeepWord[26] / 1000  ;			
			Duanchang6  = (double)(g_InteralMemory.KeepWord[27] / 10.0) * g_InteralMemory.KeepWord[28] / 1000  ;			
			Duanchang7  = (double)(g_InteralMemory.KeepWord[29] / 10.0) * g_InteralMemory.KeepWord[30] / 1000  ;			
			Duanchang8  = (double)(g_InteralMemory.KeepWord[31] / 10.0) * g_InteralMemory.KeepWord[32] / 1000  ;			
			Duanchang9  = (double)(g_InteralMemory.KeepWord[33] / 10.0) * g_InteralMemory.KeepWord[34] / 1000  ;			
			Duanchang10  = (double)(g_InteralMemory.KeepWord[35] / 10.0) * g_InteralMemory.KeepWord[36] / 1000  ;			
			Duanchang11  = (double)(g_InteralMemory.KeepWord[37] / 10.0) * g_InteralMemory.KeepWord[38] / 1000  ;			
			Duanchang12  = (double)(g_InteralMemory.KeepWord[39] / 10.0) * g_InteralMemory.KeepWord[40] / 1000  ;
						
			Daojianju   = (double)g_InteralMemory.KeepWord[42] / 10.0 ;
			
			
		/*	tmp = 0;
			for(j=17;j<=40;j++)
				tmp += g_InteralMemory.KeepWord[j];
			if(tmp > (g_InteralMemory.KeepWord[1] + g_InteralMemory.KeepWord[16]))///���ܳ���������ʱ����&&��Ҫ�����ж�
			{
				//if(g_InteralMemory.KeepWord[1] > 0)///��Ҫ�����ж�
				{
					g_InteralMemory.Word[PING_CTRL] = 7;//��������
					g_InteralMemory.Bit[2] = 0 ;
					return;
				}
			}
		*//////////////////////////////////////////
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_1)   
			Houdu = Pianxinju_1 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 

			i = (Pianxinju_1 - Houdu) / Pianxinju_1 ;
			a=acos(i);//��ڹ��̵�ת���ĽǶ�
			
			g_InteralMemory.KeepWord[43] = onecircle_pulse * a / 2.0 / PI ;//��ڹ�����Ҫ������������P_KW43//�µ�����
			g_InteralMemory.KeepWord[45] = onecircle_pulse - g_InteralMemory.KeepWord[43] - g_InteralMemory.KeepWord[44] ;//�µ����̷�����������
		
			g_InteralMemory.KeepWord[46] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / i * g_InteralMemory.KeepWord[260] / 1000 ;//A��KֵK_A1*10000
			//g_InteralMemory.KeepWord[47] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1;//B��KֵK_B*10000
			/*	
			temp1 = a / g_InteralMemory.KeepWord[43];
			for(j=0;j<=g_InteralMemory.KeepWord[43];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0)
					K_A1_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / cos(temp2) ;
				else
					K_A1_index[j]=g_InteralMemory.KeepWord[46];
			}*/
		
		
		
			
			g_InteralMemory.KeepWord[48] = Duanchang1 * Encoder_Pulse / PI / Gunzhijing ;////�γ�1������������
			g_InteralMemory.KeepWord[49] = Duanchang2 * Encoder_Pulse / PI / Gunzhijing ;////�γ�2������������
			g_InteralMemory.KeepWord[50] = Duanchang3 * Encoder_Pulse / PI / Gunzhijing ;////�γ�3������������
			g_InteralMemory.KeepWord[51] = Duanchang4 * Encoder_Pulse / PI / Gunzhijing ;////�γ�4������������
			g_InteralMemory.KeepWord[52] = Duanchang5 * Encoder_Pulse / PI / Gunzhijing ;////�γ�5������������
			g_InteralMemory.KeepWord[53] = Duanchang6 * Encoder_Pulse / PI / Gunzhijing ;////�γ�6������������
			g_InteralMemory.KeepWord[54] = Duanchang7 * Encoder_Pulse / PI / Gunzhijing ;////�γ�7������������
			g_InteralMemory.KeepWord[55] = Duanchang8 * Encoder_Pulse / PI / Gunzhijing ;////�γ�8������������
			g_InteralMemory.KeepWord[56] = Duanchang9 * Encoder_Pulse / PI / Gunzhijing ;////�γ�9������������
			g_InteralMemory.KeepWord[57] = Duanchang10 * Encoder_Pulse / PI / Gunzhijing ;////�γ�10������������
			g_InteralMemory.KeepWord[58] = Duanchang11 * Encoder_Pulse / PI / Gunzhijing ;////�γ�11������������
			g_InteralMemory.KeepWord[59] = Duanchang12 * Encoder_Pulse / PI / Gunzhijing ;////�γ�12������������
			
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9-Duanchang10-Duanchang11-Duanchang12;
			
			g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
		
			g_InteralMemory.KeepWord[61] = 10000 * g_InteralMemory.KeepWord[45] / g_InteralMemory.KeepWord[46];////����µ����̱�����������
			
			g_InteralMemory.KeepWord[62] = Pianxinju_1 * sin(a) * Encoder_Pulse / PI / Gunzhijing * 1000 / g_InteralMemory.KeepWord[260] ;////��ڹ��̱�����������
			
			g_InteralMemory.KeepWord[63] = g_InteralMemory.KeepWord[62] + 10000 * (g_InteralMemory.KeepWord[44]-g_InteralMemory.KeepWord[43]) / g_InteralMemory.KeepWord[46];////���̧�����̱�����������
			
			g_InteralMemory.KeepWord[11] = Dingchang * Encoder_Pulse / PI / Gunzhijing ;////����������������
			
			tmp = g_InteralMemory.KeepWord[61] + g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
			
			g_InteralMemory.KeepWord[64] = g_InteralMemory.KeepWord[49] - tmp ;///��1�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[65] = g_InteralMemory.KeepWord[50] - tmp ;///��2�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[66] = g_InteralMemory.KeepWord[51] - tmp ;///��3�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[67] = g_InteralMemory.KeepWord[52] - tmp ;///��4�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[68] = g_InteralMemory.KeepWord[53] - tmp ;///��5�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[69] = g_InteralMemory.KeepWord[54] - tmp ;///��6�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[70] = g_InteralMemory.KeepWord[55] - tmp ;///��7�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[71] = g_InteralMemory.KeepWord[56] - tmp ;///��8�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[72] = g_InteralMemory.KeepWord[57] - tmp ;///��9�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[73] = g_InteralMemory.KeepWord[58] - tmp ;///��10�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[74] = g_InteralMemory.KeepWord[59] - tmp ;///��11�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[75] = g_InteralMemory.KeepWord[60] +  g_InteralMemory.KeepWord[48] - tmp ;///��12�γ����ɺ�ĵȴ�������
			
			////ע�⣺�жϹ��̱����������� �д�ʵ����תʱ������
			
			///////////////////////
			g_InteralMemory.KeepWord[79] = Daojianju * Encoder_Pulse / PI / Gunzhijing ;////������Ӧ������������
			
			///�����������
			g_InteralMemory.KeepWord[77] = 0;
			for(j=17;j<=39;j+=2)
			{
				if(g_InteralMemory.KeepWord[j] == 0)
					break;
				else
					g_InteralMemory.KeepWord[77]++;
			}
		
			////���һ�γ�ں�ĵȴ�������
			if(g_InteralMemory.KeepWord[77] == 1)
			{
				tmp = Dingchang-Duanchang1;
				g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
				g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60] - tmp ;
			}
			else if(g_InteralMemory.KeepWord[77] == 2)
			{
				tmp = Dingchang-Duanchang1-Duanchang2;
				g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
				g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60] - tmp ;
			}
			else if(g_InteralMemory.KeepWord[77] == 3)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3;
				g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
				g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60] - tmp ;
			}
			else if(g_InteralMemory.KeepWord[77] == 4)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4;
				g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
				g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60] - tmp ;
			}
			else if(g_InteralMemory.KeepWord[77] == 5)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5;
				g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
				g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60] - tmp ;
			}
			else if(g_InteralMemory.KeepWord[77] == 6)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6;
				g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp =  g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
				g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60]  - tmp ;
			}
			else if(g_InteralMemory.KeepWord[77] == 7)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7;
				g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp =  g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
				g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60] - tmp ;
			}
			else if(g_InteralMemory.KeepWord[77] == 8)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8;
				g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
				g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60] - tmp ;
			}
			else if(g_InteralMemory.KeepWord[77] == 9)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9;
				g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
				g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60]  - tmp ;
			}
			else if(g_InteralMemory.KeepWord[77] == 10)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9-Duanchang10;
				g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp =  g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
				g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60]  - tmp ;
			}
			else if(g_InteralMemory.KeepWord[77] == 11)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9-Duanchang10-Duanchang11;
				g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp =  g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
				g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60]- tmp ;
			}
			else if(g_InteralMemory.KeepWord[77] == 12)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9-Duanchang10-Duanchang11-Duanchang12;
				g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp =  g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
				g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60]  - tmp ;
			}
		
		
		
		
		/////
		}///������ݴ������
		
		
	///////////////////////////////////
	/////
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		nn=0;
		Pulse_Out1=0;
		Pulse_Out2=0;
		
		g_InteralMemory.Word[501]=0;
		g_InteralMemory.Word[502]=0;
		g_InteralMemory.Word[503]=0;
		g_InteralMemory.Word[505]=0;
			
		//////////////
		g_InteralMemory.Bit[2] = 0 ;
		
		if(g_InteralMemory.KeepBit[200] == 0)
			g_InteralMemory.Word[PING_CTRL] = 2;////���ݼ�����ɺ��Զ����ز�Ʒ�������ý���
		else
			g_InteralMemory.Word[PING_CTRL] = 33;
	}
	
////////*****************************************************************///////////

	if(g_InteralMemory.Bit[6] == 1)///////////////��Ʒ������д��
	{
		
		///////////////////////////////////
	//////������
		if (g_InteralMemory.KeepWord[374] > 0)///��������0����Ϊ��Ҫ�����ж�
		{
		  Pianxinju_2 = (double)g_InteralMemory.KeepWord[0] / 10.0 ;/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
		
		Dingchang = (double)(g_InteralMemory.KeepWord[374]) * g_InteralMemory.KeepWord[375] / 1000 ;///���Ȳ���
			Gunzhijing= (double)g_InteralMemory.KeepWord[2] / 10.0 ;
			   Houdu  = (double)g_InteralMemory.KeepWord[376] / 10.0 ;
		 
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_2)   
			Houdu = Pianxinju_2 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 
			   
			i = (Pianxinju_2 - Houdu) / Pianxinju_2 ;
			a=acos(i);//�жϹ��̵�ת���ĽǶ�
			
			g_InteralMemory.KeepWord[378] = onecircle_pulse * a / 2.0 / PI ;//�жϹ�����Ҫ������������
			
			g_InteralMemory.KeepWord[379] = onecircle_pulse - g_InteralMemory.KeepWord[378] - g_InteralMemory.KeepWord[4] ;//�µ����̷�����������
			
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
			g_InteralMemory.KeepWord[380] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / i ;//A��KֵK_A2
			
			g_InteralMemory.KeepWord[381] = Dingchang * Encoder_Pulse / PI / Gunzhijing ;////����������������
			g_InteralMemory.KeepWord[382] = 10000 * g_InteralMemory.KeepWord[379] / g_InteralMemory.KeepWord[380];////�µ����̱�����������
				
			g_InteralMemory.KeepWord[383] = Pianxinju_2 * sin(a) * Encoder_Pulse / PI / Gunzhijing ;////�жϹ��̱�����������
			
			g_InteralMemory.KeepWord[384] = 10000 * g_InteralMemory.KeepWord[4] / g_InteralMemory.KeepWord[380];////̧�����̱�����������
		
			g_InteralMemory.KeepWord[385] = g_InteralMemory.KeepWord[381] - g_InteralMemory.KeepWord[382] - g_InteralMemory.KeepWord[383] - g_InteralMemory.KeepWord[384];////ԭ��ȴ����̱�����������
			////ע�⣺�жϹ��̱����������� �д�ʵ����תʱ������
		///////////////////////
		/*
			temp1 = a / g_InteralMemory.KeepWord[378];
			for(j=0;j<g_InteralMemory.KeepWord[378];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0.0)
					K_A2_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / cos(temp2) ;
				else
					K_A2_index[j]=g_InteralMemory.KeepWord[380];
			}*/
		
		}
	////////���
	/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
		if (g_InteralMemory.KeepWord[17] > 0)//��һ�γ��ȴ���0����Ϊ�ӹ����
		{
			Pianxinju_1 = (double)g_InteralMemory.KeepWord[41] / 10.0 ;
			Duanchang1  = (double)(g_InteralMemory.KeepWord[350] / 10.0) * g_InteralMemory.KeepWord[351] / 1000  ;
			Duanchang2  = (double)(g_InteralMemory.KeepWord[352] / 10.0) * g_InteralMemory.KeepWord[353] / 1000  ;			
			Duanchang3  = (double)(g_InteralMemory.KeepWord[354] / 10.0) * g_InteralMemory.KeepWord[355] / 1000  ;			
			Duanchang4  = (double)(g_InteralMemory.KeepWord[356] / 10.0) * g_InteralMemory.KeepWord[357] / 1000  ;
			Duanchang5  = (double)(g_InteralMemory.KeepWord[358] / 10.0) * g_InteralMemory.KeepWord[359] / 1000  ;			
			Duanchang6  = (double)(g_InteralMemory.KeepWord[360] / 10.0) * g_InteralMemory.KeepWord[361] / 1000  ;			
			Duanchang7  = (double)(g_InteralMemory.KeepWord[362] / 10.0) * g_InteralMemory.KeepWord[363] / 1000  ;			
			Duanchang8  = (double)(g_InteralMemory.KeepWord[364] / 10.0) * g_InteralMemory.KeepWord[365] / 1000  ;			
			Duanchang9  = (double)(g_InteralMemory.KeepWord[366] / 10.0) * g_InteralMemory.KeepWord[367] / 1000  ;			
			Duanchang10  = (double)(g_InteralMemory.KeepWord[368] / 10.0) * g_InteralMemory.KeepWord[369] / 1000  ;			
			Duanchang11  = (double)(g_InteralMemory.KeepWord[370] / 10.0) * g_InteralMemory.KeepWord[371] / 1000  ;			
			Duanchang12  = (double)(g_InteralMemory.KeepWord[372] / 10.0) * g_InteralMemory.KeepWord[373] / 1000  ;
						
			Daojianju   = (double)g_InteralMemory.KeepWord[42] / 10.0 ;
			
			
		/*	tmp = 0;
			for(j=17;j<=40;j++)
				tmp += g_InteralMemory.KeepWord[j];
			if(tmp > (g_InteralMemory.KeepWord[1] + g_InteralMemory.KeepWord[16]))///���ܳ���������ʱ����&&��Ҫ�����ж�
			{
				//if(g_InteralMemory.KeepWord[1] > 0)///��Ҫ�����ж�
				{
					g_InteralMemory.Word[PING_CTRL] = 7;//��������
					g_InteralMemory.Bit[2] = 0 ;
					return;
				}
			}
		*//////////////////////////////////////////
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_1)   
			Houdu = Pianxinju_1 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 

			i = (Pianxinju_1 - Houdu) / Pianxinju_1 ;
			a=acos(i);//��ڹ��̵�ת���ĽǶ�
			
			g_InteralMemory.KeepWord[386] = onecircle_pulse * a / 2.0 / PI ;//��ڹ�����Ҫ������������P_KW43//�µ�����
			g_InteralMemory.KeepWord[387] = onecircle_pulse - g_InteralMemory.KeepWord[386] - g_InteralMemory.KeepWord[44] ;//�µ����̷�����������
		
			g_InteralMemory.KeepWord[388] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / i * g_InteralMemory.KeepWord[260] / 1000 ;//A��KֵK_A1*10000
			/*	
			temp1 = a / g_InteralMemory.KeepWord[386];
			for(j=0;j<=g_InteralMemory.KeepWord[386];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0)
					K_A1_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / cos(temp2) ;
				else
					K_A1_index[j]=g_InteralMemory.KeepWord[388];
			}
		
		*/
		
			
			g_InteralMemory.KeepWord[389] = Duanchang1 * Encoder_Pulse / PI / Gunzhijing ;////�γ�1������������
			g_InteralMemory.KeepWord[390] = Duanchang2 * Encoder_Pulse / PI / Gunzhijing ;////�γ�2������������
			g_InteralMemory.KeepWord[391] = Duanchang3 * Encoder_Pulse / PI / Gunzhijing ;////�γ�3������������
			g_InteralMemory.KeepWord[392] = Duanchang4 * Encoder_Pulse / PI / Gunzhijing ;////�γ�4������������
			g_InteralMemory.KeepWord[393] = Duanchang5 * Encoder_Pulse / PI / Gunzhijing ;////�γ�5������������
			g_InteralMemory.KeepWord[394] = Duanchang6 * Encoder_Pulse / PI / Gunzhijing ;////�γ�6������������
			g_InteralMemory.KeepWord[395] = Duanchang7 * Encoder_Pulse / PI / Gunzhijing ;////�γ�7������������
			g_InteralMemory.KeepWord[396] = Duanchang8 * Encoder_Pulse / PI / Gunzhijing ;////�γ�8������������
			g_InteralMemory.KeepWord[397] = Duanchang9 * Encoder_Pulse / PI / Gunzhijing ;////�γ�9������������
			g_InteralMemory.KeepWord[398] = Duanchang10 * Encoder_Pulse / PI / Gunzhijing ;////�γ�10������������
			g_InteralMemory.KeepWord[399] = Duanchang11 * Encoder_Pulse / PI / Gunzhijing ;////�γ�11������������
			g_InteralMemory.KeepWord[400] = Duanchang12 * Encoder_Pulse / PI / Gunzhijing ;////�γ�12������������
			
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9-Duanchang10-Duanchang11-Duanchang12;
			
			g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
		
			g_InteralMemory.KeepWord[402] = 10000 * g_InteralMemory.KeepWord[387] / g_InteralMemory.KeepWord[388];////����µ����̱�����������
			
			g_InteralMemory.KeepWord[403] = Pianxinju_1 * sin(a) * Encoder_Pulse / PI / Gunzhijing * 1000 / g_InteralMemory.KeepWord[260];////��ڹ��̱�����������
			
			g_InteralMemory.KeepWord[404] = g_InteralMemory.KeepWord[403] + 10000 * (g_InteralMemory.KeepWord[44]-g_InteralMemory.KeepWord[386]) / g_InteralMemory.KeepWord[388];////���̧�����̱�����������
			
			g_InteralMemory.KeepWord[405] = Dingchang * Encoder_Pulse / PI / Gunzhijing ;////����������������
			
			tmp = g_InteralMemory.KeepWord[402] + g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
			
			g_InteralMemory.KeepWord[406] = g_InteralMemory.KeepWord[390] - tmp ;///��1�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[407] = g_InteralMemory.KeepWord[391] - tmp ;///��2�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[408] = g_InteralMemory.KeepWord[392] - tmp ;///��3�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[409] = g_InteralMemory.KeepWord[393] - tmp ;///��4�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[410] = g_InteralMemory.KeepWord[394] - tmp ;///��5�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[411] = g_InteralMemory.KeepWord[395] - tmp ;///��6�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[412] = g_InteralMemory.KeepWord[396] - tmp ;///��7�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[413] = g_InteralMemory.KeepWord[397] - tmp ;///��8�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[414] = g_InteralMemory.KeepWord[398] - tmp ;///��9�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[415] = g_InteralMemory.KeepWord[399] - tmp ;///��10�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[416] = g_InteralMemory.KeepWord[400] - tmp ;///��11�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[417] = g_InteralMemory.KeepWord[401] +  g_InteralMemory.KeepWord[389] - tmp ;///��12�γ����ɺ�ĵȴ�������
			
			////ע�⣺�жϹ��̱����������� �д�ʵ����תʱ������
			
			///////////////////////
			g_InteralMemory.KeepWord[79] = Daojianju * Encoder_Pulse / PI / Gunzhijing ;////������Ӧ������������
			
			///�����������
			g_InteralMemory.KeepWord[418] = 0;
			for(j=350;j<=372;j+=2)
			{
				if(g_InteralMemory.KeepWord[j] == 0)
					break;
				else
					g_InteralMemory.KeepWord[418]++;
			}
		
			////���һ�γ�ں�ĵȴ�������
			if(g_InteralMemory.KeepWord[418] == 1)
			{
				tmp = Dingchang-Duanchang1;
				g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
				g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401] - tmp ;
			}
			else if(g_InteralMemory.KeepWord[418] == 2)
			{
				tmp = Dingchang-Duanchang1-Duanchang2;
				g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
				g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401] - tmp ;
			}
			else if(g_InteralMemory.KeepWord[418] == 3)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3;
				g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
				g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401] - tmp ;
			}
			else if(g_InteralMemory.KeepWord[418] == 4)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4;
				g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
				g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401] - tmp ;
			}
			else if(g_InteralMemory.KeepWord[418] == 5)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5;
				g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
				g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401] - tmp ;
			}
			else if(g_InteralMemory.KeepWord[418] == 6)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6;
				g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
				g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401]  - tmp ;
			}
			else if(g_InteralMemory.KeepWord[418] == 7)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7;
				g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
				g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401] - tmp ;
			}
			else if(g_InteralMemory.KeepWord[418] == 8)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8;
				g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
				g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401] - tmp ;
			}
			else if(g_InteralMemory.KeepWord[418] == 9)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9;
				g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
				g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401]  - tmp ;
			}
			else if(g_InteralMemory.KeepWord[418] == 10)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9-Duanchang10;
				g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
				g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401]  - tmp ;
			}
			else if(g_InteralMemory.KeepWord[418] == 11)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9-Duanchang10-Duanchang11;
				g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
				g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401]- tmp ;
			}
			else if(g_InteralMemory.KeepWord[418] == 12)
			{
				tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9-Duanchang10-Duanchang11-Duanchang12;
				g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
				
				tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
				g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401]  - tmp ;
			}
		
		
		
		
		/////
		}///������ݴ������
		
		
	///////////////////////////////////
	/////
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		nn=0;
		Pulse_Out1=0;
		Pulse_Out2=0;
		
		g_InteralMemory.Word[501]=0;
		g_InteralMemory.Word[502]=0;
		g_InteralMemory.Word[503]=0;
		g_InteralMemory.Word[505]=0;
			
		//////////////
		g_InteralMemory.Bit[6] = 0 ;
		
		
		if(g_InteralMemory.KeepBit[200] == 0)
			g_InteralMemory.Word[PING_CTRL] = 9;////���ݼ�����ɺ��Զ����ز�Ʒ�������ý���
		else
			g_InteralMemory.Word[PING_CTRL] = 35;
	}
	
////////*****************************************************************///////////	

	if(g_InteralMemory.Bit[7] == 1)///////////////��Ʒ������д��
	{
		
	}
	if(g_InteralMemory.Bit[37] == 1)/////////////�������ֲ�Ʒд��
	{
		Pianxinju_2 = (double)g_InteralMemory.KeepWord[0] / 10.0 ;/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
		
		Dingchang1 = (double)(g_InteralMemory.KeepWord[80] ) * g_InteralMemory.KeepWord[82] / 1000 ;///
		Dingchang2 = (double)(g_InteralMemory.KeepWord[84]) * g_InteralMemory.KeepWord[86] / 1000 ;///
		Dingchang3 = (double)(g_InteralMemory.KeepWord[88] ) * g_InteralMemory.KeepWord[90] / 1000 ;///
			Gunzhijing= (double)g_InteralMemory.KeepWord[2] / 10.0 ;
			   Houdu  = (double)g_InteralMemory.KeepWord[170] / 10.0 ;
		 
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_2)   
			Houdu = Pianxinju_2 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 
			   
			i = (Pianxinju_2 - Houdu) / Pianxinju_2 ;
			a=acos(i);//�жϹ��̵�ת���ĽǶ�
			
			g_InteralMemory.KeepWord[92] = onecircle_pulse * a / 2.0 / PI ;//�жϹ�����Ҫ������������P_KW5
			
			g_InteralMemory.KeepWord[93] = onecircle_pulse - g_InteralMemory.KeepWord[92] - g_InteralMemory.KeepWord[4] ;//�µ����̷�����������
			
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
			g_InteralMemory.KeepWord[94] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / i ;//A��KֵK_A2
			
			//g_InteralMemory.KeepWord[95] = Dingchang1 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			//g_InteralMemory.KeepWord[96] = Dingchang2 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			//g_InteralMemory.KeepWord[97] = Dingchang3 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			
			Dingchang1_1 = Dingchang1 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			Dingchang2_1 = Dingchang2 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			Dingchang3_1 = Dingchang3 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			
			
			
			g_InteralMemory.KeepWord[98] = 10000 * g_InteralMemory.KeepWord[93] / g_InteralMemory.KeepWord[94];////�µ����̱�����������
				
			g_InteralMemory.KeepWord[99] = Pianxinju_2 * sin(a) * Encoder_Pulse / PI / Gunzhijing ;////�жϹ��̱�����������
			
			g_InteralMemory.KeepWord[100] = 10000 * g_InteralMemory.KeepWord[4] / g_InteralMemory.KeepWord[94];////̧�����̱�����������
		
			//g_InteralMemory.KeepWord[101] = g_InteralMemory.KeepWord[95] - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
			//g_InteralMemory.KeepWord[102] = g_InteralMemory.KeepWord[96] - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
			//g_InteralMemory.KeepWord[103] = g_InteralMemory.KeepWord[97] - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
			
			Dingchang1_2 = Dingchang1_1 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
			Dingchang2_2 = Dingchang2_1 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
			Dingchang3_2 = Dingchang3_1 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������

		
		
		/*
			temp1 = a / g_InteralMemory.KeepWord[92];
			for(j=0;j<g_InteralMemory.KeepWord[92];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0.0)
					K_A2_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / cos(temp2) ;
				else
					K_A2_index[j]=g_InteralMemory.KeepWord[94];
			}*/
			
		g_InteralMemory.Bit[37] = 0;	
		if(g_InteralMemory.KeepBit[200] == 0)
			g_InteralMemory.Word[PING_CTRL] = 15;////���ݼ�����ɺ��Զ����ز�Ʒ�������ý���
		else
			g_InteralMemory.Word[PING_CTRL] = 31;
	}
	
////////*****************************************************************///////////
////////*****************************************************************///////////
////////*****************************************************************///////////
///////��Ʒ��ѡ��ѡ������ص�����������ֵ����ر���	
	if(g_InteralMemory.Bit[11] == 1)///////////////ѡ�в�Ʒһ
	{
		InPulse1_Stop();//�л���Ʒǰ����ͣ�������� ������ȫ��������ٿ�����������
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		
		g_InteralMemory.Bit[8] = 1;
		g_InteralMemory.Bit[9] = 0;
		g_InteralMemory.Bit[10] = 0;
		
		g_InteralMemory.Bit[30] = 0;
		g_InteralMemory.Bit[31] = 0;
		g_InteralMemory.Bit[32] = 0;
		g_InteralMemory.Bit[42] = 0;
		g_InteralMemory.Bit[43] = 0;
		g_InteralMemory.Bit[38] = 0;
		g_InteralMemory.Bit[11] = 0;
		
		g_InteralMemory.KeepBit[13] =1;
	//	g_InteralMemory.Bit[80] = 1;
		g_InteralMemory.KeepWord[10] = 0;
		
		g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =1;
		g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =0;
		
		g_InteralMemory.Bit[86] =0;
		g_InteralMemory.Bit[88] =0;
		
		g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =0;
	///////////////////////////////////////
	//////��Ʒ����
	Pianxinju_2 = (double)g_InteralMemory.KeepWord[0] / 10.0 ;/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
		
		  Houdu  = (double)g_InteralMemory.KeepWord[3] / 10.0 ;
		 
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_2)   
			Houdu = Pianxinju_2 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 
			   
			i = (Pianxinju_2 - Houdu) / Pianxinju_2 ;
			a=acos(i);//�жϹ��̵�ת���ĽǶ�
			
			temp1 = a / g_InteralMemory.KeepWord[5];
			for(j=0;j<g_InteralMemory.KeepWord[5];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0.0)
					K_A2_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / cos(temp2) ;
				else
					K_A2_index[j]=g_InteralMemory.KeepWord[7];
			}
		
		
	
			Pianxinju_1 = (double)g_InteralMemory.KeepWord[41] / 10.0 ;
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_1)   
			Houdu = Pianxinju_1 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 

			i = (Pianxinju_1 - Houdu) / Pianxinju_1 ;
			a=acos(i);//��ڹ��̵�ת���ĽǶ�
			
			temp1 = a / g_InteralMemory.KeepWord[43];
			for(j=0;j<=g_InteralMemory.KeepWord[43];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0)
					K_A1_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / cos(temp2) * g_InteralMemory.KeepWord[260] / 1000 ;
				else
					K_A1_index[j]=g_InteralMemory.KeepWord[46];
			}
	//////////////////////////////////////////////////////
	////���������K_A2,dK2,Inpulse_xiadao2,Inpulse_qieduan2,Inpulse_taidao2,Inpulse_dengdai2,Outpulse_xiadao2,Outpulse_qieduan2,Outpulse_taidao2
		K_A2 = g_InteralMemory.KeepWord[7];
		//dK2  = g_InteralMemory.KeepWord[9];
		Inpulse_xiadao2 = g_InteralMemory.KeepWord[12];
		Inpulse_qieduan2 = g_InteralMemory.KeepWord[13];
		Inpulse_taidao2 = g_InteralMemory.KeepWord[14];
	//	Inpulse_dengdai2[0] = g_InteralMemory.KeepWord[11] - g_InteralMemory.KeepWord[12] ;
	
		Inpulse_dengdai2[0] = g_InteralMemory.KeepWord[15];
		Inpulse_dengdai2[1] = g_InteralMemory.KeepWord[15];
		
		Inpulse_all2 = g_InteralMemory.KeepWord[11];
		Outpulse_xiadao2 = g_InteralMemory.KeepWord[6];
		Outpulse_qieduan2 = g_InteralMemory.KeepWord[5];
		Outpulse_taidao2 = g_InteralMemory.KeepWord[4];
		
		Dingchang_on_off = g_InteralMemory.KeepWord[1];
		
		
		
		
	//////////////////////
	////������
	// K_A1,dK1,Inpulse_xiadao1,Inpulse_qieduan1,Inpulse_taidao1,Outpulse_xiadao1,Outpulse_qieduan1,Outpulse_taidao1;////������ȫ�ֱ���
	// Inpulse_dengdai1[13],Inpulse_all1[13];
		
		Inpulse_xiadao1 = g_InteralMemory.KeepWord[61];
		Inpulse_qieduan1 = g_InteralMemory.KeepWord[62];
		Inpulse_taidao1 = g_InteralMemory.KeepWord[63];
		Outpulse_xiadao1 =  g_InteralMemory.KeepWord[45];
		Outpulse_qieduan1 =  g_InteralMemory.KeepWord[43];
		Outpulse_taidao1 =  g_InteralMemory.KeepWord[44];
		
		K_A1 = g_InteralMemory.KeepWord[46];
	//	dK1 = g_InteralMemory.KeepWord[76];
		
		
	
		Inpulse_dengdai1[0] = g_InteralMemory.KeepWord[48]-g_InteralMemory.KeepWord[61];//�γ�1�ȴ�������=�γ�����-�µ�����
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
		
		Inpulse_all1[1] = g_InteralMemory.KeepWord[48];
		Inpulse_all1[2] = g_InteralMemory.KeepWord[49];
		Inpulse_all1[3] = g_InteralMemory.KeepWord[50];
		Inpulse_all1[4] = g_InteralMemory.KeepWord[51];
		Inpulse_all1[5] = g_InteralMemory.KeepWord[52];
		Inpulse_all1[6] = g_InteralMemory.KeepWord[53];
		Inpulse_all1[7] = g_InteralMemory.KeepWord[54];
		Inpulse_all1[8] = g_InteralMemory.KeepWord[55];
		Inpulse_all1[9] = g_InteralMemory.KeepWord[56];
		Inpulse_all1[10] = g_InteralMemory.KeepWord[57];
		Inpulse_all1[11] = g_InteralMemory.KeepWord[58];
		Inpulse_all1[12] = g_InteralMemory.KeepWord[59];
		
		Chongkoushu = g_InteralMemory.KeepWord[77];
		Chongkou_on_off = g_InteralMemory.KeepWord[17];
		chongkou_last_length = g_InteralMemory.KeepWord[60];
	////////////////////////////////////////////
		g_InteralMemory.Word[501]=0;
		g_InteralMemory.Word[502]=0;
		g_InteralMemory.Word[503]=0;
		g_InteralMemory.Word[504]=0;
		g_InteralMemory.Word[505]=0;
		g_InteralMemory.Word[506]=0;
		g_InteralMemory.Word[507]=0;
		g_InteralMemory.Word[508]=0;
		g_InteralMemory.Word[509]=0;////��������������
		g_InteralMemory.Word[511]=0;
		
		
		chongkou_current = 0;
		dingchang_current = 0;
		current_qieduan = 0;
		need_qieduan = 0;
		pulseout_qieduan1 = 0;
		pulseout_qieduan2 = 0;
		qieduan_enable = 0;
		cut_all = 0;
		
		for(j=0; j<10; j++)
			Enable_qieduan[j]=0;
			
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[1];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=Chongkoushu;
			
		/////////////////////////////////////////////////////
		g_InteralMemory.KeepBit[10] = 0;
		g_InteralMemory.KeepBit[12] = 0;
		
		if(g_InteralMemory.Bit[27]==1)///ϵͳ����
			InPulse1_Start();
	}
	
	if(g_InteralMemory.Bit[12] == 1)///////////////ѡ�в�Ʒ��
	{
		InPulse1_Stop();//�л���Ʒǰ����ͣ�������� ������ȫ��������ٿ�����������
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		
		g_InteralMemory.Bit[8] = 0;
		g_InteralMemory.Bit[9] = 1;
		g_InteralMemory.Bit[10] = 0;
		
		g_InteralMemory.Bit[30] = 0;
		g_InteralMemory.Bit[31] = 0;
		g_InteralMemory.Bit[32] = 0;
		g_InteralMemory.Bit[42] = 0;
		g_InteralMemory.Bit[43] = 0;
		g_InteralMemory.Bit[38] = 0;
		g_InteralMemory.Bit[12] = 0;
		
		g_InteralMemory.KeepBit[13] =2;

		g_InteralMemory.KeepWord[10] = 0;
		
		g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =0;
		
		g_InteralMemory.Bit[86] =0;
		g_InteralMemory.Bit[88] =0;
		
		g_InteralMemory.Bit[90] =1;
		g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =0;
		
	///////////////////////////////////////
	//////��Ʒ����
	
	
	 Pianxinju_2 = (double)g_InteralMemory.KeepWord[0] / 10.0 ;/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
		
			   Houdu  = (double)g_InteralMemory.KeepWord[376] / 10.0 ;
		 
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_2)   
			Houdu = Pianxinju_2 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 
			   
			i = (Pianxinju_2 - Houdu) / Pianxinju_2 ;
			a=acos(i);//�жϹ��̵�ת���ĽǶ�
			
			
		
			temp1 = a / g_InteralMemory.KeepWord[378];
			for(j=0;j<g_InteralMemory.KeepWord[378];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0.0)
					K_A2_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / cos(temp2) ;
				else
					K_A2_index[j]=g_InteralMemory.KeepWord[380];
			}
			
			
			Pianxinju_1 = (double)g_InteralMemory.KeepWord[41] / 10.0 ;
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_1)   
			Houdu = Pianxinju_1 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 

			i = (Pianxinju_1 - Houdu) / Pianxinju_1 ;
			a=acos(i);//��ڹ��̵�ת���ĽǶ�
			

			temp1 = a / g_InteralMemory.KeepWord[386];
			for(j=0;j<=g_InteralMemory.KeepWord[386];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0)
					K_A1_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / cos(temp2) * g_InteralMemory.KeepWord[260] / 1000 ;
				else
					K_A1_index[j]=g_InteralMemory.KeepWord[388];
			}
			
	//////////////////////////////////////////////////////
	////���������K_A2,dK2,Inpulse_xiadao2,Inpulse_qieduan2,Inpulse_taidao2,Inpulse_dengdai2,Outpulse_xiadao2,Outpulse_qieduan2,Outpulse_taidao2
		K_A2 = g_InteralMemory.KeepWord[380];
		
		Inpulse_xiadao2 = g_InteralMemory.KeepWord[382];
		Inpulse_qieduan2 = g_InteralMemory.KeepWord[383];
		Inpulse_taidao2 = g_InteralMemory.KeepWord[384];
	
		Inpulse_dengdai2[0] = g_InteralMemory.KeepWord[385];
		Inpulse_dengdai2[1] = g_InteralMemory.KeepWord[385];
		
		Inpulse_all2 = g_InteralMemory.KeepWord[381];
		Outpulse_xiadao2 = g_InteralMemory.KeepWord[379];
		Outpulse_qieduan2 = g_InteralMemory.KeepWord[378];
		Outpulse_taidao2 = g_InteralMemory.KeepWord[4];
		
		Dingchang_on_off = 1;
		
		
	//////////////////////
	////������
		
		Inpulse_xiadao1 = g_InteralMemory.KeepWord[402];
		Inpulse_qieduan1 = g_InteralMemory.KeepWord[403];
		Inpulse_taidao1 = g_InteralMemory.KeepWord[404];
		Outpulse_xiadao1 =  g_InteralMemory.KeepWord[387];
		Outpulse_qieduan1 =  g_InteralMemory.KeepWord[386];
		Outpulse_taidao1 =  g_InteralMemory.KeepWord[44];
		
		K_A1 = g_InteralMemory.KeepWord[388];

		Inpulse_dengdai1[0] = g_InteralMemory.KeepWord[389]-g_InteralMemory.KeepWord[402];//�γ�1�ȴ�������=�γ�����-�µ�����
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
		/*
		Inpulse_all1[1] = g_InteralMemory.KeepWord[48];
		Inpulse_all1[2] = g_InteralMemory.KeepWord[49];
		Inpulse_all1[3] = g_InteralMemory.KeepWord[50];
		Inpulse_all1[4] = g_InteralMemory.KeepWord[51];
		Inpulse_all1[5] = g_InteralMemory.KeepWord[52];
		Inpulse_all1[6] = g_InteralMemory.KeepWord[53];
		Inpulse_all1[7] = g_InteralMemory.KeepWord[54];
		Inpulse_all1[8] = g_InteralMemory.KeepWord[55];
		Inpulse_all1[9] = g_InteralMemory.KeepWord[56];
		Inpulse_all1[10] = g_InteralMemory.KeepWord[57];
		Inpulse_all1[11] = g_InteralMemory.KeepWord[58];
		Inpulse_all1[12] = g_InteralMemory.KeepWord[59];
		*/
		Chongkoushu = g_InteralMemory.KeepWord[418];
		Chongkou_on_off = 1;
		chongkou_last_length = g_InteralMemory.KeepWord[401];
	////////////////////////////////////////////
		g_InteralMemory.Word[501]=0;
		g_InteralMemory.Word[502]=0;
		g_InteralMemory.Word[503]=0;
		g_InteralMemory.Word[504]=0;
		g_InteralMemory.Word[505]=0;
		g_InteralMemory.Word[506]=0;
		g_InteralMemory.Word[507]=0;
		g_InteralMemory.Word[508]=0;
		g_InteralMemory.Word[509]=0;////��������������
		g_InteralMemory.Word[511]=0;
		
		
		chongkou_current = 0;
		dingchang_current = 0;
		current_qieduan = 0;
		need_qieduan = 0;
		pulseout_qieduan1 = 0;
		pulseout_qieduan2 = 0;
		qieduan_enable = 0;
		cut_all = 0;
		
		for(j=0; j<10; j++)
			Enable_qieduan[j]=0;
			
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[374];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=Chongkoushu;
			
		/////////////////////////////////////////////////////
		g_InteralMemory.KeepBit[10] = 0;
		g_InteralMemory.KeepBit[12] = 0;
		
		if(g_InteralMemory.Bit[27]==1)///ϵͳ����
			InPulse1_Start();
	
	
	/////////////////////////////////////////////////////
		
	}
	
	if(g_InteralMemory.Bit[13] == 1)///////////////ѡ�в�Ʒ��
	{
		g_InteralMemory.Bit[10] = 1;
		g_InteralMemory.Bit[8] = 0;
		g_InteralMemory.Bit[9] = 0;
		g_InteralMemory.Bit[30] = 0;
		g_InteralMemory.Bit[31] = 0;
		g_InteralMemory.Bit[32] = 0;
		g_InteralMemory.Bit[42] = 0;
		g_InteralMemory.Bit[43] = 0;
		g_InteralMemory.Bit[13] = 0;
		g_InteralMemory.Bit[38] = 0;
		//g_InteralMemory.KeepBit[13] =3;
	///////////////////////////////////////
	//////��Ʒ����
	
	
	/////////////////////////////////////////////////////
		g_InteralMemory.KeepBit[10] = 0;
		g_InteralMemory.KeepBit[12] = 0;
	}


	if(g_InteralMemory.Bit[33] == 1)///////////////ѡ�в�Ʒ4 �������ֶ�����Ʒ�����м��������ǵ��粻����� ���ѡ���Ʒ�����¼������һ�����������ϵ�û�в��������쳣
	{
		InPulse1_Stop();//�л���Ʒǰ����ͣ�������� ������ȫ��������ٿ�����������
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		g_InteralMemory.Bit[10] = 0;
		g_InteralMemory.Bit[8] = 0;
		g_InteralMemory.Bit[9] = 0;
		g_InteralMemory.Bit[30] = 1;
		g_InteralMemory.Bit[31] = 0;
		g_InteralMemory.Bit[32] = 0;
		
		g_InteralMemory.Bit[33] = 0;
		g_InteralMemory.Bit[38] = 0;
		g_InteralMemory.Bit[42] = 0;
		g_InteralMemory.Bit[43] = 0;
		g_InteralMemory.KeepBit[13] =3;
		
		g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =1;
		g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =0;
		
		g_InteralMemory.Bit[86] =0;
		g_InteralMemory.Bit[88] =0;
		
		g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =0;
	////////////////////////////////////////////////
	///////*************
	Pianxinju_2 = (double)g_InteralMemory.KeepWord[0] / 10.0 ;/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
		
		Dingchang1 = (double)(g_InteralMemory.KeepWord[80] ) * g_InteralMemory.KeepWord[82] / 1000 ;///
		Dingchang2 = (double)(g_InteralMemory.KeepWord[84] ) * g_InteralMemory.KeepWord[86] / 1000 ;///
		Dingchang3 = (double)(g_InteralMemory.KeepWord[88] ) * g_InteralMemory.KeepWord[90] / 1000 ;///
			Gunzhijing= (double)g_InteralMemory.KeepWord[2] / 10.0 ;
			   Houdu  = (double)g_InteralMemory.KeepWord[170] / 10.0 ;
		 
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_2)   
			Houdu = Pianxinju_2 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 
			   
			i = (Pianxinju_2 - Houdu) / Pianxinju_2 ;
			a=acos(i);//�жϹ��̵�ת���ĽǶ�
			
			g_InteralMemory.KeepWord[92] = onecircle_pulse * a / 2.0 / PI ;//�жϹ�����Ҫ������������P_KW5
			
			g_InteralMemory.KeepWord[93] = onecircle_pulse - g_InteralMemory.KeepWord[92] - g_InteralMemory.KeepWord[4] ;//�µ����̷�����������
			
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if(g_InteralMemory.KeepWord[80] >= 55)
				g_InteralMemory.KeepWord[94] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / i ;//A��KֵK_A2
			else
				g_InteralMemory.KeepWord[94] = 2 * 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / i ;//A��KֵK_A2
			
			Dingchang1_1 = Dingchang1 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			Dingchang2_1 = Dingchang2 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			Dingchang3_1 = Dingchang3 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			
			g_InteralMemory.KeepWord[98] = 10000 * g_InteralMemory.KeepWord[93] / g_InteralMemory.KeepWord[94];////�µ����̱�����������
				
			g_InteralMemory.KeepWord[99] = Pianxinju_2 * sin(a) * Encoder_Pulse / PI / Gunzhijing ;////�жϹ��̱�����������
			
			g_InteralMemory.KeepWord[100] = 10000 * g_InteralMemory.KeepWord[4] / g_InteralMemory.KeepWord[94];////̧�����̱�����������
		
			Dingchang1_2 = Dingchang1_1 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
			Dingchang2_2 = Dingchang2_1 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
			Dingchang3_2 = Dingchang3_1 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������

			temp1 = a / g_InteralMemory.KeepWord[92];
			for(j=0;j<g_InteralMemory.KeepWord[92];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0.0)
					K_A2_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / cos(temp2) ;
				else
					K_A2_index[j]=g_InteralMemory.KeepWord[94];
			}
	///////////////////////////////////////
	//////��Ʒ����
		K_A2 = g_InteralMemory.KeepWord[94];
		
		Inpulse_xiadao2 = g_InteralMemory.KeepWord[98];
		Inpulse_qieduan2 = g_InteralMemory.KeepWord[99];
		Inpulse_taidao2 = g_InteralMemory.KeepWord[100];
	
	//	Inpulse_dengdai2[0] = g_InteralMemory.KeepWord[101];///////101  102  
	//	Inpulse_dengdai2[1] = g_InteralMemory.KeepWord[101];
		Inpulse_dengdai2[0] = Dingchang1_2;///////101  102  103Dingchang1_1
		Inpulse_dengdai2[1] = Dingchang1_2;
	
		Outpulse_xiadao2 = g_InteralMemory.KeepWord[93];
		Outpulse_qieduan2 = g_InteralMemory.KeepWord[92];
		Outpulse_taidao2 = g_InteralMemory.KeepWord[4];
		
		Dingchang_on_off = 1;
		Chongkou_on_off = 0;
	//	g_InteralMemory.Bit[39] = 1;
		
		g_InteralMemory.KeepWord[10] = 0;
		
		
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[80];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=0;
	/////////////////////////////////////////////////////
		g_InteralMemory.KeepBit[10] = 0;
		g_InteralMemory.KeepBit[12] = 0;
		
		g_InteralMemory.Word[501]=0;
		g_InteralMemory.Word[502]=0;
		g_InteralMemory.Word[503]=0;
		g_InteralMemory.Word[504]=0;
		g_InteralMemory.Word[505]=0;
		g_InteralMemory.Word[506]=0;
		g_InteralMemory.Word[507]=0;
		g_InteralMemory.Word[508]=0;
		g_InteralMemory.Word[509]=0;////��������������
		g_InteralMemory.Word[511]=0;
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		chongkou_current = 0;
		dingchang_current = 0;
		current_qieduan = 0;
		need_qieduan = 0;
		pulseout_qieduan1 = 0;
		pulseout_qieduan2 = 0;
		qieduan_enable = 0;
		cut_all = 0;
		
		for(j=0; j<10; j++)
			Enable_qieduan[j]=0;
		if(g_InteralMemory.Bit[27]==1)///ϵͳ����	
			InPulse1_Start();
	}
	
	if(g_InteralMemory.Bit[34] == 1)///////////////ѡ�в�Ʒ5
	{
		InPulse1_Stop();//�л���Ʒǰ����ͣ�������� ������ȫ��������ٿ�����������
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		g_InteralMemory.Bit[10] = 0;
		g_InteralMemory.Bit[8] = 0;
		g_InteralMemory.Bit[9] = 0;
		g_InteralMemory.Bit[30] = 0;
		g_InteralMemory.Bit[31] = 1;
		g_InteralMemory.Bit[32] = 0;
		
		g_InteralMemory.Bit[34] = 0;
		g_InteralMemory.Bit[38] = 0;
		
		g_InteralMemory.Bit[42] = 0;
		g_InteralMemory.Bit[43] = 0;
		g_InteralMemory.KeepBit[13] =4;
		
		g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =1;
		g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =0;
		
		g_InteralMemory.Bit[86] =0;
		g_InteralMemory.Bit[88] =0;
		
		g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =0;
		///////////////////////////////////
		/////////*****************************
		Pianxinju_2 = (double)g_InteralMemory.KeepWord[0] / 10.0 ;/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
		
		Dingchang1 = (double)(g_InteralMemory.KeepWord[80]) * g_InteralMemory.KeepWord[82] / 1000 ;///
		Dingchang2 = (double)(g_InteralMemory.KeepWord[84] ) * g_InteralMemory.KeepWord[86] / 1000 ;///
		Dingchang3 = (double)(g_InteralMemory.KeepWord[88] ) * g_InteralMemory.KeepWord[90] / 1000 ;///
			Gunzhijing= (double)g_InteralMemory.KeepWord[2] / 10.0 ;
			   Houdu  = (double)g_InteralMemory.KeepWord[170] / 10.0 ;
		 
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_2)   
			Houdu = Pianxinju_2 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 
			   
			i = (Pianxinju_2 - Houdu) / Pianxinju_2 ;
			a=acos(i);//�жϹ��̵�ת���ĽǶ�
			
			g_InteralMemory.KeepWord[92] = onecircle_pulse * a / 2.0 / PI ;//�жϹ�����Ҫ������������P_KW5
			
			g_InteralMemory.KeepWord[93] = onecircle_pulse - g_InteralMemory.KeepWord[92] - g_InteralMemory.KeepWord[4] ;//�µ����̷�����������
			
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
			if(g_InteralMemory.KeepWord[84] >= 55)
				g_InteralMemory.KeepWord[94] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / i ;//A��KֵK_A2
			else
				g_InteralMemory.KeepWord[94] = 2 * 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / i ;//A��KֵK_A2
			
			Dingchang1_1 = Dingchang1 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			Dingchang2_1 = Dingchang2 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			Dingchang3_1 = Dingchang3 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			
			
			
			g_InteralMemory.KeepWord[98] = 10000 * g_InteralMemory.KeepWord[93] / g_InteralMemory.KeepWord[94];////�µ����̱�����������
				
			g_InteralMemory.KeepWord[99] = Pianxinju_2 * sin(a) * Encoder_Pulse / PI / Gunzhijing ;////�жϹ��̱�����������
			
			g_InteralMemory.KeepWord[100] = 10000 * g_InteralMemory.KeepWord[4] / g_InteralMemory.KeepWord[94];////̧�����̱�����������
		
			Dingchang1_2 = Dingchang1_1 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
			Dingchang2_2 = Dingchang2_1 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
			Dingchang3_2 = Dingchang3_1 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������

			temp1 = a / g_InteralMemory.KeepWord[92];
			for(j=0;j<g_InteralMemory.KeepWord[92];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0.0)
					K_A2_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / cos(temp2) ;
				else
					K_A2_index[j]=g_InteralMemory.KeepWord[94];
			}
	///////////////////////////////////////
	//////��Ʒ����
		K_A2 = g_InteralMemory.KeepWord[94];
		
		Inpulse_xiadao2 = g_InteralMemory.KeepWord[98];
		Inpulse_qieduan2 = g_InteralMemory.KeepWord[99];
		Inpulse_taidao2 = g_InteralMemory.KeepWord[100];
	
	
		Inpulse_dengdai2[0] = Dingchang2_2;///////101  102  103
		Inpulse_dengdai2[1] = Dingchang2_2;
		
		
		Outpulse_xiadao2 = g_InteralMemory.KeepWord[93];
		Outpulse_qieduan2 = g_InteralMemory.KeepWord[92];
		Outpulse_taidao2 = g_InteralMemory.KeepWord[4];
		
		Dingchang_on_off = 1;
		Chongkou_on_off = 0;
	//	g_InteralMemory.Bit[40] = 1;
		
		g_InteralMemory.KeepWord[10] = 0;
		
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[84];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=0;
	/////////////////////////////////////////////////////
		g_InteralMemory.KeepBit[10] = 0;
		g_InteralMemory.KeepBit[12] = 0;
		
		g_InteralMemory.Word[501]=0;
		g_InteralMemory.Word[502]=0;
		g_InteralMemory.Word[503]=0;
		g_InteralMemory.Word[504]=0;
		g_InteralMemory.Word[505]=0;
		g_InteralMemory.Word[506]=0;
		g_InteralMemory.Word[507]=0;
		g_InteralMemory.Word[508]=0;
		g_InteralMemory.Word[509]=0;////��������������
		g_InteralMemory.Word[511]=0;
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		chongkou_current = 0;
		dingchang_current = 0;
		current_qieduan = 0;
		need_qieduan = 0;
		pulseout_qieduan1 = 0;
		pulseout_qieduan2 = 0;
		qieduan_enable = 0;
		cut_all = 0;
		
		for(j=0; j<10; j++)
			Enable_qieduan[j]=0;
		
		if(g_InteralMemory.Bit[27]==1)///ϵͳ����	
			InPulse1_Start();
	}
	
	if(g_InteralMemory.Bit[35] == 1)///////////////ѡ�в�Ʒ6
	{
		InPulse1_Stop();//�л���Ʒǰ����ͣ�������� ������ȫ��������ٿ�����������
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		g_InteralMemory.Bit[10] = 0;
		g_InteralMemory.Bit[8] = 0;
		g_InteralMemory.Bit[9] = 0;
		g_InteralMemory.Bit[30] = 0;
		g_InteralMemory.Bit[31] = 0;
		g_InteralMemory.Bit[32] = 1;
		
		g_InteralMemory.Bit[35] = 0;
		g_InteralMemory.Bit[38] = 0;
		
		g_InteralMemory.Bit[42] = 0;
		g_InteralMemory.Bit[43] = 0;
		g_InteralMemory.KeepBit[13] =5;
		
		g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =1;
		g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =0;
		
		g_InteralMemory.Bit[86] =0;
		g_InteralMemory.Bit[88] =0;
		
		g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =0;
		///////////////////////////////////
		/////////////***************
		Pianxinju_2 = (double)g_InteralMemory.KeepWord[0] / 10.0 ;/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
		
		Dingchang1 = (double)(g_InteralMemory.KeepWord[80]) * g_InteralMemory.KeepWord[82] / 1000 ;///
		Dingchang2 = (double)(g_InteralMemory.KeepWord[84] ) * g_InteralMemory.KeepWord[86] / 1000 ;///
		Dingchang3 = (double)(g_InteralMemory.KeepWord[88] ) * g_InteralMemory.KeepWord[90] / 1000 ;///
			Gunzhijing= (double)g_InteralMemory.KeepWord[2] / 10.0 ;
			   Houdu  = (double)g_InteralMemory.KeepWord[170] / 10.0 ;
		 
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_2)   
			Houdu = Pianxinju_2 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 
			   
			i = (Pianxinju_2 - Houdu) / Pianxinju_2 ;
			a=acos(i);//�жϹ��̵�ת���ĽǶ�
			
			g_InteralMemory.KeepWord[92] = onecircle_pulse * a / 2.0 / PI ;//�жϹ�����Ҫ������������P_KW5
			
			g_InteralMemory.KeepWord[93] = onecircle_pulse - g_InteralMemory.KeepWord[92] - g_InteralMemory.KeepWord[4] ;//�µ����̷�����������
			
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
			if(g_InteralMemory.KeepWord[88] >= 55)
				g_InteralMemory.KeepWord[94] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / i ;//A��KֵK_A2
			else
				g_InteralMemory.KeepWord[94] = 2 * 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / i ;//A��KֵK_A2
			
			Dingchang1_1 = Dingchang1 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			Dingchang2_1 = Dingchang2 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			Dingchang3_1 = Dingchang3 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			
			
			
			g_InteralMemory.KeepWord[98] = 10000 * g_InteralMemory.KeepWord[93] / g_InteralMemory.KeepWord[94];////�µ����̱�����������
				
			g_InteralMemory.KeepWord[99] = Pianxinju_2 * sin(a) * Encoder_Pulse / PI / Gunzhijing ;////�жϹ��̱�����������
			
			g_InteralMemory.KeepWord[100] = 10000 * g_InteralMemory.KeepWord[4] / g_InteralMemory.KeepWord[94];////̧�����̱�����������
		
			Dingchang1_2 = Dingchang1_1 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
			Dingchang2_2 = Dingchang2_1 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
			Dingchang3_2 = Dingchang3_1 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������

			temp1 = a / g_InteralMemory.KeepWord[92];
			for(j=0;j<g_InteralMemory.KeepWord[92];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0.0)
					K_A2_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / cos(temp2) ;
				else
					K_A2_index[j]=g_InteralMemory.KeepWord[94];
			}
	///////////////////////////////////////
	//////��Ʒ����
		K_A2 = g_InteralMemory.KeepWord[94];
		
		Inpulse_xiadao2 = g_InteralMemory.KeepWord[98];
		Inpulse_qieduan2 = g_InteralMemory.KeepWord[99];
		Inpulse_taidao2 = g_InteralMemory.KeepWord[100];
	
	
		Inpulse_dengdai2[0] = Dingchang3_2;///////101  102  103
		Inpulse_dengdai2[1] = Dingchang3_2;
		
		
		Outpulse_xiadao2 = g_InteralMemory.KeepWord[93];
		Outpulse_qieduan2 = g_InteralMemory.KeepWord[92];
		Outpulse_taidao2 = g_InteralMemory.KeepWord[4];
		
		Dingchang_on_off = 1;
		Chongkou_on_off = 0;
		//g_InteralMemory.Bit[41] = 1;
		
		g_InteralMemory.KeepWord[10] = 0;
		
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[88];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=0;
	/////////////////////////////////////////////////////
		g_InteralMemory.KeepBit[10] = 0;
		g_InteralMemory.KeepBit[12] = 0;
		
		g_InteralMemory.Word[501]=0;
		g_InteralMemory.Word[502]=0;
		g_InteralMemory.Word[503]=0;
		g_InteralMemory.Word[504]=0;
		g_InteralMemory.Word[505]=0;
		g_InteralMemory.Word[506]=0;
		g_InteralMemory.Word[507]=0;
		g_InteralMemory.Word[508]=0;
		g_InteralMemory.Word[509]=0;////��������������
		g_InteralMemory.Word[511]=0;
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		chongkou_current = 0;
		dingchang_current = 0;
		current_qieduan = 0;
		need_qieduan = 0;
		pulseout_qieduan1 = 0;
		pulseout_qieduan2 = 0;
		qieduan_enable = 0;
		cut_all = 0;
		
		for(j=0; j<10; j++)
			Enable_qieduan[j]=0;
		
		if(g_InteralMemory.Bit[27]==1)///ϵͳ����	
			InPulse1_Start();
	}
	
	if(g_InteralMemory.Bit[36] == 1)///////////////ѡ�в�Ʒ4\5\6
	{
		InPulse1_Stop();//�л���Ʒǰ����ͣ�������� ������ȫ��������ٿ�����������
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		g_InteralMemory.Bit[10] = 0;
		g_InteralMemory.Bit[8] = 0;
		g_InteralMemory.Bit[9] = 0;
		g_InteralMemory.Bit[30] = 1;
		g_InteralMemory.Bit[31] = 1;
		g_InteralMemory.Bit[32] = 1;
		
		g_InteralMemory.Bit[36] = 0;
		g_InteralMemory.Bit[42] = 0;
		g_InteralMemory.Bit[43] = 0;
		
		g_InteralMemory.KeepBit[13] =6;/////789
		
		g_InteralMemory.Bit[38] = 1;///����˳��ӹ�
		
		g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =0;
		
		g_InteralMemory.Bit[86] =0;
		g_InteralMemory.Bit[88] =0;
		
		
		g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =0;
		///////////////////////
		////////***********************
		Pianxinju_2 = (double)g_InteralMemory.KeepWord[0] / 10.0 ;/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
		
		Dingchang1 = (double)(g_InteralMemory.KeepWord[80] ) * g_InteralMemory.KeepWord[82] / 1000 ;///
		Dingchang2 = (double)(g_InteralMemory.KeepWord[84] ) * g_InteralMemory.KeepWord[86] / 1000 ;///
		Dingchang3 = (double)(g_InteralMemory.KeepWord[88] ) * g_InteralMemory.KeepWord[90] / 1000 ;///
			Gunzhijing= (double)g_InteralMemory.KeepWord[2] / 10.0 ;
			   Houdu  = (double)g_InteralMemory.KeepWord[170] / 10.0 ;
		 
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_2)   
			Houdu = Pianxinju_2 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 
			   
			i = (Pianxinju_2 - Houdu) / Pianxinju_2 ;
			a=acos(i);//�жϹ��̵�ת���ĽǶ�
			
			g_InteralMemory.KeepWord[92] = onecircle_pulse * a / 2.0 / PI ;//�жϹ�����Ҫ������������P_KW5
			
			g_InteralMemory.KeepWord[93] = onecircle_pulse - g_InteralMemory.KeepWord[92] - g_InteralMemory.KeepWord[4] ;//�µ����̷�����������
			
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
			if((g_InteralMemory.KeepWord[80] >= 55)&&(g_InteralMemory.KeepWord[84] >= 55)&&(g_InteralMemory.KeepWord[88] >= 55))
				g_InteralMemory.KeepWord[94] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / i ;//A��KֵK_A2
			else
				g_InteralMemory.KeepWord[94] = 2 * 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / i ;//A��KֵK_A2
			
			Dingchang1_1 = Dingchang1 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			Dingchang2_1 = Dingchang2 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			Dingchang3_1 = Dingchang3 * Encoder_Pulse / PI / Gunzhijing ;////����������������
			
			g_InteralMemory.KeepWord[98] = 10000 * g_InteralMemory.KeepWord[93] / g_InteralMemory.KeepWord[94];////�µ����̱�����������
				
			g_InteralMemory.KeepWord[99] = Pianxinju_2 * sin(a) * Encoder_Pulse / PI / Gunzhijing ;////�жϹ��̱�����������
			
			g_InteralMemory.KeepWord[100] = 10000 * g_InteralMemory.KeepWord[4] / g_InteralMemory.KeepWord[94];////̧�����̱�����������
		
			Dingchang1_2 = Dingchang1_1 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
			Dingchang2_2 = Dingchang2_1 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
			Dingchang3_2 = Dingchang3_1 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������

		
		
		
			temp1 = a / g_InteralMemory.KeepWord[92];
			for(j=0;j<g_InteralMemory.KeepWord[92];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0.0)
					K_A2_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / cos(temp2) ;
				else
					K_A2_index[j]=g_InteralMemory.KeepWord[94];
			}
	///////////////////////////////////////
	//////��Ʒ����K_A2,dK2,Inpulse_xiadao2,Inpulse_qieduan2,Inpulse_taidao2,Inpulse_dengdai2,Outpulse_xiadao2,Outpulse_qieduan2,Outpulse_taidao2
		K_A2 = g_InteralMemory.KeepWord[94];
		
		Inpulse_xiadao2 = g_InteralMemory.KeepWord[98];
		Inpulse_qieduan2 = g_InteralMemory.KeepWord[99];
		Inpulse_taidao2 = g_InteralMemory.KeepWord[100];
	
	
		Inpulse_dengdai2[0] =Dingchang1_2;///////101  102  103
		Inpulse_dengdai2[1] =Dingchang1_2;
		
		
		Outpulse_xiadao2 = g_InteralMemory.KeepWord[93];
		Outpulse_qieduan2 = g_InteralMemory.KeepWord[92];
		Outpulse_taidao2 = g_InteralMemory.KeepWord[4];
		
		Dingchang_on_off = 1;
		Chongkou_on_off = 0;
		current_product = 0;//���ڼ��㵱ǰ���Ǹ���Ʒ
		
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[80];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=0;
	/////////////////////////////////////////////////////
		g_InteralMemory.KeepBit[10] = 0;
		g_InteralMemory.KeepBit[12] = 0;
		g_InteralMemory.KeepWord[10] = 0;
		
		g_InteralMemory.Word[501]=0;
		g_InteralMemory.Word[502]=0;
		g_InteralMemory.Word[503]=0;
		g_InteralMemory.Word[504]=0;
		g_InteralMemory.Word[505]=0;
		g_InteralMemory.Word[506]=0;
		g_InteralMemory.Word[507]=0;
		g_InteralMemory.Word[508]=0;
		g_InteralMemory.Word[509]=0;////��������������
		g_InteralMemory.Word[511]=0;
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		chongkou_current = 0;
		dingchang_current = 0;
		current_qieduan = 0;
		need_qieduan = 0;
		pulseout_qieduan1 = 0;
		pulseout_qieduan2 = 0;
		qieduan_enable = 0;
		cut_all = 0;
		current_product = 0;
		
		for(j=0; j<10; j++)
			Enable_qieduan[j]=0;
		
		if(g_InteralMemory.Bit[27]==1)///ϵͳ����	
			InPulse1_Start();
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////

	if(g_InteralMemory.Bit[38] == 1)/////��������kw83   87  91
	{
	//	g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =0;
		g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =0;
		
		if(( g_InteralMemory.KeepWord[10] >=  g_InteralMemory.KeepWord[83]) && (current_product == 0))
		{
			Inpulse_dengdai2[0] = Dingchang2_2;///////101  102  103
			Inpulse_dengdai2[1] = Dingchang2_2;
			current_product++;
			g_InteralMemory.KeepWord[10] = 0;
			g_InteralMemory.KeepBit[13] = 7;//////��ǰ��Ʒ�л����ڶ���
			
			g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[84];/////������������ʾ�����ͳ����
			g_InteralMemory.Word[515]=0;
		}
		else if(( g_InteralMemory.KeepWord[10] >=  g_InteralMemory.KeepWord[87]) && (current_product == 1))
		{
			Inpulse_dengdai2[0] = Dingchang3_2;///////101  102  103
			Inpulse_dengdai2[1] = Dingchang3_2;
			current_product++;
			g_InteralMemory.KeepWord[10] = 0;
			g_InteralMemory.KeepBit[13] = 8;//////��ǰ��Ʒ�л����ڶ���
			
			g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[88];/////������������ʾ�����ͳ����
			g_InteralMemory.Word[515]=0;
		}
		else if(( g_InteralMemory.KeepWord[10] >=  g_InteralMemory.KeepWord[91]) && (current_product == 2))
		{
			Inpulse_dengdai2[0] = Dingchang1_2;///////101  102  103
			Inpulse_dengdai2[1] = Dingchang1_2;
			current_product = 0;
			g_InteralMemory.KeepWord[10] = 0;
			Set_Y_Value(7,0);//��Ƶ��FWDͣ��
			InPulse1_Stop();//��������
			g_InteralMemory.Bit[38] = 0;
			g_InteralMemory.Bit[23] = 1;//ֹͣ��ť����ִ��һ��
			if(g_InteralMemory.KeepBit[200] == 0)
				g_InteralMemory.Word[PING_CTRL] = 17;////���ݼ�����ɺ��Զ����ز�Ʒ�������ý���
			else
				g_InteralMemory.Word[PING_CTRL] = 48;
			
			g_InteralMemory.Bit[30] = 0;
			g_InteralMemory.Bit[31] = 0;
			g_InteralMemory.Bit[32] = 0;
			Dingchang_on_off = 0;
		}
		
		////////////////////////////////////////////////////////////
		///���ֲ������������޸�
		Dingchang1 = (double)(g_InteralMemory.KeepWord[80] ) * g_InteralMemory.KeepWord[82] / 1000 ;///
		Dingchang2 = (double)(g_InteralMemory.KeepWord[84]) * g_InteralMemory.KeepWord[86] / 1000 ;///
		Dingchang3 = (double)(g_InteralMemory.KeepWord[88] ) * g_InteralMemory.KeepWord[90] / 1000 ;///
		
		Dingchang1_2 = Dingchang1 * Encoder_Pulse / PI / 51 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
		Dingchang2_2 = Dingchang2 * Encoder_Pulse / PI / 51 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
		Dingchang3_2 = Dingchang3 * Encoder_Pulse / PI / 51 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
		
	/*
		
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
		}*/
	
	}
	////////////////////////////
	////****����������Ʒ�ֱ�ӹ�
	if(g_InteralMemory.Bit[39] == 1)/////����kw83  
	{
		g_InteralMemory.Bit[38] =0;
	//	g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =0;
		g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =0;
		
		if( g_InteralMemory.KeepWord[10] >=  g_InteralMemory.KeepWord[83])
		{
			Set_Y_Value(7,0);//��Ƶ��FWDͣ��
			InPulse1_Stop();//��������
			g_InteralMemory.Bit[39] = 0;
			g_InteralMemory.Bit[23] = 1;//ֹͣ��ť����ִ��һ��
			
			if(g_InteralMemory.KeepBit[200] == 0)
				g_InteralMemory.Word[PING_CTRL] = 17;////���ݼ�����ɺ��Զ����ز�Ʒ�������ý���
			else
				g_InteralMemory.Word[PING_CTRL] = 48;
			
			g_InteralMemory.Bit[30] = 0;
			Dingchang_on_off = 0;
		}
		////////////////////////////////////////////////////////////
		///���ֲ������������޸�
		Dingchang1 = (double)(g_InteralMemory.KeepWord[80] ) * g_InteralMemory.KeepWord[82] / 1000 ;///
		
		Dingchang1_2 = Dingchang1 * Encoder_Pulse / PI / 51 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
		/*
		Inpulse_dengdai2[0] = Dingchang1_2;///////101  102  103
		Inpulse_dengdai2[1] = Dingchang1_2;*/
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[80];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=0;
	}
	if(g_InteralMemory.Bit[40] == 1)/////����kw 87  
	{
		g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =0;
	//	g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =0;
		g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =0;
		
		if( g_InteralMemory.KeepWord[10] >=  g_InteralMemory.KeepWord[87])
		{
			Set_Y_Value(7,0);//��Ƶ��FWDͣ��
			InPulse1_Stop();//��������
			g_InteralMemory.Bit[40] = 0;
			g_InteralMemory.Bit[23] = 1;//ֹͣ��ť����ִ��һ��
			if(g_InteralMemory.KeepBit[200] == 0)
				g_InteralMemory.Word[PING_CTRL] = 17;////���ݼ�����ɺ��Զ����ز�Ʒ�������ý���
			else
				g_InteralMemory.Word[PING_CTRL] = 48;
			g_InteralMemory.Bit[31] = 0;
			Dingchang_on_off = 0;
		}
		////////////////////////////////////////////////////////////
		///���ֲ������������޸�
		Dingchang2 = (double)(g_InteralMemory.KeepWord[84]) * g_InteralMemory.KeepWord[86] / 1000 ;///
		
		Dingchang2_2 = Dingchang2 * Encoder_Pulse / PI / 51 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
		/*
		Inpulse_dengdai2[0] = Dingchang2_2;///////101  102  103
		Inpulse_dengdai2[1] = Dingchang2_2;*/
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[84];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=0;
	}
	if(g_InteralMemory.Bit[41] == 1)/////����kw91
	{
		g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =0;
	//	g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =0;
		g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =0;
		
		if( g_InteralMemory.KeepWord[10] >=  g_InteralMemory.KeepWord[91])
		{
			Set_Y_Value(7,0);//��Ƶ��FWDͣ��
			InPulse1_Stop();//��������
			g_InteralMemory.Bit[41] = 0;
			g_InteralMemory.Bit[23] = 1;//ֹͣ��ť����ִ��һ��
			if(g_InteralMemory.KeepBit[200] == 0)
				g_InteralMemory.Word[PING_CTRL] = 17;////���ݼ�����ɺ��Զ����ز�Ʒ�������ý���
			else
				g_InteralMemory.Word[PING_CTRL] = 48;
			g_InteralMemory.Bit[32] = 0;
			Dingchang_on_off = 0;
		}
		////////////////////////////////////////////////////////////
		///���ֲ������������޸�
		Dingchang3 = (double)(g_InteralMemory.KeepWord[88] ) * g_InteralMemory.KeepWord[90] / 1000 ;///
		
		Dingchang3_2 = Dingchang3 * Encoder_Pulse / PI / 51 - g_InteralMemory.KeepWord[98] - g_InteralMemory.KeepWord[99] - g_InteralMemory.KeepWord[100];////ԭ��ȴ����̱�����������
/*
		Inpulse_dengdai2[0] = Dingchang3_2;///////101  102  103
		Inpulse_dengdai2[1] = Dingchang3_2;*/
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[88];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=0;
			
	}
	
	if(g_InteralMemory.Bit[80] == 1)/////���-����1��Ʒһ���
	{
		g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =0;
	//	g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =0;
		g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =0;
		
		if( g_InteralMemory.KeepWord[10] >=  g_InteralMemory.KeepWord[162])
		{
			Set_Y_Value(7,0);//��Ƶ��FWDͣ��
			InPulse1_Stop();//��������
			g_InteralMemory.Bit[80] = 0;
			g_InteralMemory.Bit[23] = 1;//ֹͣ��ť����ִ��һ��
			if(g_InteralMemory.KeepBit[200] == 0)
				g_InteralMemory.Word[PING_CTRL] = 17;////���ݼ�����ɺ��Զ����ز�Ʒ�������ý���
			else
				g_InteralMemory.Word[PING_CTRL] = 48;
			g_InteralMemory.Bit[8] = 0;
			Dingchang_on_off = 0;
			Chongkou_on_off = 0;

		}
		////////////////////////////////////////////////////////////
		///���ֲ������������޸�
		Dingchang = (double)(g_InteralMemory.KeepWord[1]) * g_InteralMemory.KeepWord[16] / 1000 ;///kw16���Ȳ���
		
		Duanchang1  = (double)(g_InteralMemory.KeepWord[17] / 10.0) * g_InteralMemory.KeepWord[18] / 1000  ;
		Duanchang2  = (double)(g_InteralMemory.KeepWord[19] / 10.0) * g_InteralMemory.KeepWord[20] / 1000  ;			
		Duanchang3  = (double)(g_InteralMemory.KeepWord[21] / 10.0) * g_InteralMemory.KeepWord[22] / 1000  ;			
		Duanchang4  = (double)(g_InteralMemory.KeepWord[23] / 10.0) * g_InteralMemory.KeepWord[24] / 1000  ;
		Duanchang5  = (double)(g_InteralMemory.KeepWord[25] / 10.0) * g_InteralMemory.KeepWord[26] / 1000  ;			
		Duanchang6  = (double)(g_InteralMemory.KeepWord[27] / 10.0) * g_InteralMemory.KeepWord[28] / 1000  ;			
		Duanchang7  = (double)(g_InteralMemory.KeepWord[29] / 10.0) * g_InteralMemory.KeepWord[30] / 1000  ;			
		Duanchang8  = (double)(g_InteralMemory.KeepWord[31] / 10.0) * g_InteralMemory.KeepWord[32] / 1000  ;			
		Duanchang9  = (double)(g_InteralMemory.KeepWord[33] / 10.0) * g_InteralMemory.KeepWord[34] / 1000  ;			
		Duanchang10  = (double)(g_InteralMemory.KeepWord[35] / 10.0) * g_InteralMemory.KeepWord[36] / 1000  ;			
		Duanchang11  = (double)(g_InteralMemory.KeepWord[37] / 10.0) * g_InteralMemory.KeepWord[38] / 1000  ;			
		Duanchang12  = (double)(g_InteralMemory.KeepWord[39] / 10.0) * g_InteralMemory.KeepWord[40] / 1000  ;
		
		g_InteralMemory.KeepWord[48] = Duanchang1 * Encoder_Pulse / PI / Gunzhijing ;////�γ�1������������
		g_InteralMemory.KeepWord[49] = Duanchang2 * Encoder_Pulse / PI / Gunzhijing ;////�γ�2������������
		g_InteralMemory.KeepWord[50] = Duanchang3 * Encoder_Pulse / PI / Gunzhijing ;////�γ�3������������
		g_InteralMemory.KeepWord[51] = Duanchang4 * Encoder_Pulse / PI / Gunzhijing ;////�γ�4������������
		g_InteralMemory.KeepWord[52] = Duanchang5 * Encoder_Pulse / PI / Gunzhijing ;////�γ�5������������
		g_InteralMemory.KeepWord[53] = Duanchang6 * Encoder_Pulse / PI / Gunzhijing ;////�γ�6������������
		g_InteralMemory.KeepWord[54] = Duanchang7 * Encoder_Pulse / PI / Gunzhijing ;////�γ�7������������
		g_InteralMemory.KeepWord[55] = Duanchang8 * Encoder_Pulse / PI / Gunzhijing ;////�γ�8������������
		g_InteralMemory.KeepWord[56] = Duanchang9 * Encoder_Pulse / PI / Gunzhijing ;////�γ�9������������
		g_InteralMemory.KeepWord[57] = Duanchang10 * Encoder_Pulse / PI / Gunzhijing ;////�γ�10������������
		g_InteralMemory.KeepWord[58] = Duanchang11 * Encoder_Pulse / PI / Gunzhijing ;////�γ�11������������
		g_InteralMemory.KeepWord[59] = Duanchang12 * Encoder_Pulse / PI / Gunzhijing ;////�γ�12������������
		
		tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9-Duanchang10-Duanchang11-Duanchang12;
		
		g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
	
		
		g_InteralMemory.KeepWord[11] = Dingchang * Encoder_Pulse / PI / Gunzhijing ;////����������������
		
		tmp = g_InteralMemory.KeepWord[61] + g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
		
		g_InteralMemory.KeepWord[64] = g_InteralMemory.KeepWord[49] - tmp ;///��1�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[65] = g_InteralMemory.KeepWord[50] - tmp ;///��2�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[66] = g_InteralMemory.KeepWord[51] - tmp ;///��3�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[67] = g_InteralMemory.KeepWord[52] - tmp ;///��4�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[68] = g_InteralMemory.KeepWord[53] - tmp ;///��5�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[69] = g_InteralMemory.KeepWord[54] - tmp ;///��6�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[70] = g_InteralMemory.KeepWord[55] - tmp ;///��7�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[71] = g_InteralMemory.KeepWord[56] - tmp ;///��8�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[72] = g_InteralMemory.KeepWord[57] - tmp ;///��9�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[73] = g_InteralMemory.KeepWord[58] - tmp ;///��10�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[74] = g_InteralMemory.KeepWord[59] - tmp ;///��11�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[75] = g_InteralMemory.KeepWord[60] +  g_InteralMemory.KeepWord[48] - tmp ;///��12�γ����ɺ�ĵȴ�������
		
		///////////////////////
		
		////���һ�γ�ں�ĵȴ�������
		if(g_InteralMemory.KeepWord[77] == 1)
		{
			tmp = Dingchang-Duanchang1;
			g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
			g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60] - tmp ;
		}
		else if(g_InteralMemory.KeepWord[77] == 2)
		{
			tmp = Dingchang-Duanchang1-Duanchang2;
			g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
			g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60] - tmp ;
		}
		else if(g_InteralMemory.KeepWord[77] == 3)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3;
			g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
			g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60] - tmp ;
		}
		else if(g_InteralMemory.KeepWord[77] == 4)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4;
			g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
			g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60] - tmp ;
		}
		else if(g_InteralMemory.KeepWord[77] == 5)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5;
			g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
			g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60] - tmp ;
		}
		else if(g_InteralMemory.KeepWord[77] == 6)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6;
			g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp =  g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
			g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60]  - tmp ;
		}
		else if(g_InteralMemory.KeepWord[77] == 7)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7;
			g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp =  g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
			g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60] - tmp ;
		}
		else if(g_InteralMemory.KeepWord[77] == 8)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8;
			g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
			g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60] - tmp ;
		}
		else if(g_InteralMemory.KeepWord[77] == 9)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9;
			g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
			g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60]  - tmp ;
		}
		else if(g_InteralMemory.KeepWord[77] == 10)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9-Duanchang10;
			g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp =  g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
			g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60]  - tmp ;
		}
		else if(g_InteralMemory.KeepWord[77] == 11)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9-Duanchang10-Duanchang11;
			g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp =  g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
			g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60]- tmp ;
		}
		else if(g_InteralMemory.KeepWord[77] == 12)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9-Duanchang10-Duanchang11-Duanchang12;
			g_InteralMemory.KeepWord[60] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp =  g_InteralMemory.KeepWord[62] + g_InteralMemory.KeepWord[63];
			g_InteralMemory.KeepWord[78] = g_InteralMemory.KeepWord[60]  - tmp ;
		}
			
	/*
		Inpulse_dengdai1[0] = g_InteralMemory.KeepWord[48]-g_InteralMemory.KeepWord[61];//�γ�1�ȴ�������=�γ�����-�µ�����
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
		
		chongkou_last_length = g_InteralMemory.KeepWord[60];*/
		
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[1];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=Chongkoushu;
	}
	
	
	if(g_InteralMemory.Bit[90] == 1)/////���-����2��Ʒһ���
	{
		g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =0;
		//g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =0;
		
		if( g_InteralMemory.KeepWord[10] >=  g_InteralMemory.KeepWord[377])
		{
			Set_Y_Value(7,0);//��Ƶ��FWDͣ��
			InPulse1_Stop();//��������
			g_InteralMemory.Bit[90] = 0;
			g_InteralMemory.Bit[23] = 1;//ֹͣ��ť����ִ��һ��
			if(g_InteralMemory.KeepBit[200] == 0)
				g_InteralMemory.Word[PING_CTRL] = 17;////���ݼ�����ɺ��Զ����ز�Ʒ�������ý���
			else
				g_InteralMemory.Word[PING_CTRL] = 48;
			g_InteralMemory.Bit[9] = 0;
			Dingchang_on_off = 0;
			Chongkou_on_off = 0;

		}
		////////////////////////////////////////////////////////////
		///���ֲ������������޸�
		Dingchang = (double)(g_InteralMemory.KeepWord[374]) * g_InteralMemory.KeepWord[375] / 1000 ;///kw16���Ȳ���
		
		Duanchang1  = (double)(g_InteralMemory.KeepWord[350] / 10.0) * g_InteralMemory.KeepWord[351] / 1000  ;
		Duanchang2  = (double)(g_InteralMemory.KeepWord[352] / 10.0) * g_InteralMemory.KeepWord[353] / 1000  ;			
		Duanchang3  = (double)(g_InteralMemory.KeepWord[354] / 10.0) * g_InteralMemory.KeepWord[355] / 1000  ;			
		Duanchang4  = (double)(g_InteralMemory.KeepWord[356] / 10.0) * g_InteralMemory.KeepWord[357] / 1000  ;
		Duanchang5  = (double)(g_InteralMemory.KeepWord[358] / 10.0) * g_InteralMemory.KeepWord[359] / 1000  ;			
		Duanchang6  = (double)(g_InteralMemory.KeepWord[360] / 10.0) * g_InteralMemory.KeepWord[361] / 1000  ;			
		Duanchang7  = (double)(g_InteralMemory.KeepWord[362] / 10.0) * g_InteralMemory.KeepWord[363] / 1000  ;			
		Duanchang8  = (double)(g_InteralMemory.KeepWord[364] / 10.0) * g_InteralMemory.KeepWord[365] / 1000  ;			
		Duanchang9  = (double)(g_InteralMemory.KeepWord[366] / 10.0) * g_InteralMemory.KeepWord[367] / 1000  ;			
		Duanchang10  = (double)(g_InteralMemory.KeepWord[368] / 10.0) * g_InteralMemory.KeepWord[369] / 1000  ;			
		Duanchang11  = (double)(g_InteralMemory.KeepWord[370] / 10.0) * g_InteralMemory.KeepWord[371] / 1000  ;			
		Duanchang12  = (double)(g_InteralMemory.KeepWord[372] / 10.0) * g_InteralMemory.KeepWord[373] / 1000  ;
		
		g_InteralMemory.KeepWord[389] = Duanchang1 * Encoder_Pulse / PI / Gunzhijing ;////�γ�1������������
		g_InteralMemory.KeepWord[390] = Duanchang2 * Encoder_Pulse / PI / Gunzhijing ;////�γ�2������������
		g_InteralMemory.KeepWord[391] = Duanchang3 * Encoder_Pulse / PI / Gunzhijing ;////�γ�3������������
		g_InteralMemory.KeepWord[392] = Duanchang4 * Encoder_Pulse / PI / Gunzhijing ;////�γ�4������������
		g_InteralMemory.KeepWord[393] = Duanchang5 * Encoder_Pulse / PI / Gunzhijing ;////�γ�5������������
		g_InteralMemory.KeepWord[394] = Duanchang6 * Encoder_Pulse / PI / Gunzhijing ;////�γ�6������������
		g_InteralMemory.KeepWord[395] = Duanchang7 * Encoder_Pulse / PI / Gunzhijing ;////�γ�7������������
		g_InteralMemory.KeepWord[396] = Duanchang8 * Encoder_Pulse / PI / Gunzhijing ;////�γ�8������������
		g_InteralMemory.KeepWord[397] = Duanchang9 * Encoder_Pulse / PI / Gunzhijing ;////�γ�9������������
		g_InteralMemory.KeepWord[398] = Duanchang10 * Encoder_Pulse / PI / Gunzhijing ;////�γ�10������������
		g_InteralMemory.KeepWord[399] = Duanchang11 * Encoder_Pulse / PI / Gunzhijing ;////�γ�11������������
		g_InteralMemory.KeepWord[400] = Duanchang12 * Encoder_Pulse / PI / Gunzhijing ;////�γ�12������������
		
		tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9-Duanchang10-Duanchang11-Duanchang12;
		
		
		
		g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
		
			
		g_InteralMemory.KeepWord[405] = Dingchang * Encoder_Pulse / PI / Gunzhijing ;////����������������
		
		tmp = g_InteralMemory.KeepWord[402] + g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
		
		g_InteralMemory.KeepWord[406] = g_InteralMemory.KeepWord[390] - tmp ;///��1�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[407] = g_InteralMemory.KeepWord[391] - tmp ;///��2�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[408] = g_InteralMemory.KeepWord[392] - tmp ;///��3�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[409] = g_InteralMemory.KeepWord[393] - tmp ;///��4�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[410] = g_InteralMemory.KeepWord[394] - tmp ;///��5�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[411] = g_InteralMemory.KeepWord[395] - tmp ;///��6�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[412] = g_InteralMemory.KeepWord[396] - tmp ;///��7�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[413] = g_InteralMemory.KeepWord[397] - tmp ;///��8�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[414] = g_InteralMemory.KeepWord[398] - tmp ;///��9�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[415] = g_InteralMemory.KeepWord[399] - tmp ;///��10�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[416] = g_InteralMemory.KeepWord[400] - tmp ;///��11�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[417] = g_InteralMemory.KeepWord[401] +  g_InteralMemory.KeepWord[389] - tmp ;///��12�γ����ɺ�ĵȴ�������
		
		
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[374];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=Chongkoushu;
		
		////���һ�γ�ں�ĵȴ�������
		if(g_InteralMemory.KeepWord[418] == 1)
		{
			tmp = Dingchang-Duanchang1;
			g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
			g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401] - tmp ;
		}
		else if(g_InteralMemory.KeepWord[418] == 2)
		{
			tmp = Dingchang-Duanchang1-Duanchang2;
			g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
			g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401] - tmp ;
		}
		else if(g_InteralMemory.KeepWord[418] == 3)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3;
			g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
			g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401] - tmp ;
		}
		else if(g_InteralMemory.KeepWord[418] == 4)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4;
			g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
			g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401] - tmp ;
		}
		else if(g_InteralMemory.KeepWord[418] == 5)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5;
			g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
			g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401] - tmp ;
		}
		else if(g_InteralMemory.KeepWord[418] == 6)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6;
			g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
			g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401]  - tmp ;
		}
		else if(g_InteralMemory.KeepWord[418] == 7)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7;
			g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
			g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401] - tmp ;
		}
		else if(g_InteralMemory.KeepWord[418] == 8)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8;
			g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
			g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401] - tmp ;
		}
		else if(g_InteralMemory.KeepWord[418] == 9)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9;
			g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
			g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401]  - tmp ;
		}
		else if(g_InteralMemory.KeepWord[418] == 10)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9-Duanchang10;
			g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
			g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401]  - tmp ;
		}
		else if(g_InteralMemory.KeepWord[418] == 11)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9-Duanchang10-Duanchang11;
			g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
			g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401]- tmp ;
		}
		else if(g_InteralMemory.KeepWord[418] == 12)
		{
			tmp = Dingchang-Duanchang1-Duanchang2-Duanchang3-Duanchang4-Duanchang5-Duanchang6-Duanchang7-Duanchang8-Duanchang9-Duanchang10-Duanchang11-Duanchang12;
			g_InteralMemory.KeepWord[401] = tmp * Encoder_Pulse / PI / Gunzhijing ;////ĩ�α�����������
			
			tmp = g_InteralMemory.KeepWord[403] + g_InteralMemory.KeepWord[404];
			g_InteralMemory.KeepWord[419] = g_InteralMemory.KeepWord[401]  - tmp ;
		}
			
	
	}
	
	
	
	
	
	
	if(g_InteralMemory.Bit[81] == 1)/////б��-������Ʒһ���
	{
		g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =0;
	//	g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =0;
		g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =0;
		
		if( g_InteralMemory.KeepWord[10] >=  g_InteralMemory.KeepWord[160])
		{
			Set_Y_Value(7,0);//��Ƶ��FWDͣ��
			InPulse1_Stop();//��������
			g_InteralMemory.Bit[81] = 0;
			g_InteralMemory.Bit[23] = 1;//ֹͣ��ť����ִ��һ��
			if(g_InteralMemory.KeepBit[200] == 0)
				g_InteralMemory.Word[PING_CTRL] = 17;////���ݼ�����ɺ��Զ����ز�Ʒ�������ý���
			else
				g_InteralMemory.Word[PING_CTRL] = 48;
			g_InteralMemory.Bit[42] = 0;
			Dingchang_on_off = 0;
			Chongkou_on_off = 0;

		}
		/////////////////////////////////////////////////////////////////////////////////////////
		/////*********************************************************
		/////�����޸�����
		
		//Dingchang =(g_InteralMemory.KeepWord[104]) * g_InteralMemory.KeepWord[105] / 1000 +(g_InteralMemory.KeepWord[106]) * g_InteralMemory.KeepWord[107] / 1000 
		//			+ (g_InteralMemory.KeepWord[108]) * g_InteralMemory.KeepWord[109] / 1000 + (g_InteralMemory.KeepWord[110] ) * g_InteralMemory.KeepWord[111] / 1000 ;
		
			
		//g_InteralMemory.KeepWord[116] = Dingchang * Encoder_Pulse / PI / Gunzhijing ;////����������������
		
	//	g_InteralMemory.KeepWord[120] = g_InteralMemory.KeepWord[116] - g_InteralMemory.KeepWord[117] - g_InteralMemory.KeepWord[118] - g_InteralMemory.KeepWord[119];////ԭ��ȴ����̱�����������
	//	
	//	Duanchang1  = g_InteralMemory.KeepWord[104] * g_InteralMemory.KeepWord[105] / 1000  ;
	//	Duanchang2  = g_InteralMemory.KeepWord[106] * g_InteralMemory.KeepWord[107] / 1000  ;			
	//	Duanchang3  = g_InteralMemory.KeepWord[108] * g_InteralMemory.KeepWord[109] / 1000  ;			
	//	Duanchang4  = g_InteralMemory.KeepWord[110] * g_InteralMemory.KeepWord[111] / 1000  ;
		
		
		g_InteralMemory.KeepWord[124] = g_InteralMemory.KeepWord[104] * g_InteralMemory.KeepWord[105] / 1000 * Encoder_Pulse / PI / Gunzhijing ;////�γ�1������������
		g_InteralMemory.KeepWord[125] = g_InteralMemory.KeepWord[106] * g_InteralMemory.KeepWord[107] / 1000 * Encoder_Pulse / PI / Gunzhijing ;////�γ�2������������
		g_InteralMemory.KeepWord[126] = g_InteralMemory.KeepWord[108] * g_InteralMemory.KeepWord[109] / 1000 * Encoder_Pulse / PI / Gunzhijing ;////�γ�3������������
		g_InteralMemory.KeepWord[127] = g_InteralMemory.KeepWord[110] * g_InteralMemory.KeepWord[111] / 1000 * Encoder_Pulse / PI / Gunzhijing ;////�γ�4������������

		
		g_InteralMemory.KeepWord[130] = g_InteralMemory.KeepWord[129] + 10000 * (g_InteralMemory.KeepWord[44]-g_InteralMemory.KeepWord[121]) / g_InteralMemory.KeepWord[123];////���̧�����̱�����������
		
		
		tmp = g_InteralMemory.KeepWord[128] + g_InteralMemory.KeepWord[129] + g_InteralMemory.KeepWord[130];
		
		g_InteralMemory.KeepWord[131] = g_InteralMemory.KeepWord[125] - tmp ;///��1�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[132] = g_InteralMemory.KeepWord[126] - tmp ;///��2�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[133] = g_InteralMemory.KeepWord[127] - tmp ;///��3�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[134] = g_InteralMemory.KeepWord[124] - tmp ;///��4�γ����ɺ�ĵȴ�������
		
			
	//	Inpulse_dengdai2[0] = g_InteralMemory.KeepWord[120];
	///	Inpulse_dengdai2[1] = g_InteralMemory.KeepWord[120];
		
		/*
		Inpulse_dengdai1[0] = g_InteralMemory.KeepWord[134];
		Inpulse_dengdai1[1] = g_InteralMemory.KeepWord[131];
		Inpulse_dengdai1[2] = g_InteralMemory.KeepWord[132];
		Inpulse_dengdai1[3] = g_InteralMemory.KeepWord[133];
		
		if(g_InteralMemory.KeepBit[15] == 0)
			Chongkoushu = 3;
		else if(g_InteralMemory.KeepBit[15] == 1)
			Chongkoushu = 0;
		*/
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[104] +g_InteralMemory.KeepWord[106]+g_InteralMemory.KeepWord[108]+g_InteralMemory.KeepWord[110];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=Chongkoushu;
	}
	
	if(g_InteralMemory.Bit[91] == 1)/////б��-������Ʒ2���
	{
		g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =0;
		g_InteralMemory.Bit[90] =0;
		//g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =0;
		
		if( g_InteralMemory.KeepWord[10] >=  g_InteralMemory.KeepWord[460])
		{
			Set_Y_Value(7,0);//��Ƶ��FWDͣ��
			InPulse1_Stop();//��������
			g_InteralMemory.Bit[91] = 0;
			g_InteralMemory.Bit[23] = 1;//ֹͣ��ť����ִ��һ��
			if(g_InteralMemory.KeepBit[200] == 0)
				g_InteralMemory.Word[PING_CTRL] = 17;////���ݼ�����ɺ��Զ����ز�Ʒ�������ý���
			else
				g_InteralMemory.Word[PING_CTRL] = 48;
			g_InteralMemory.Bit[86] = 0;
			Dingchang_on_off = 0;
			Chongkou_on_off = 0;

		}
		/////////////////////////////////////////////////////////////////////////////////////////
		/////*********************************************************
		/////�����޸�����
	
		
		g_InteralMemory.KeepWord[472] = g_InteralMemory.KeepWord[450] * g_InteralMemory.KeepWord[451] / 1000 * Encoder_Pulse / PI / Gunzhijing ;////�γ�1������������
		g_InteralMemory.KeepWord[473] = g_InteralMemory.KeepWord[452] * g_InteralMemory.KeepWord[453] / 1000 * Encoder_Pulse / PI / Gunzhijing ;////�γ�2������������
		g_InteralMemory.KeepWord[474] = g_InteralMemory.KeepWord[454] * g_InteralMemory.KeepWord[455] / 1000 * Encoder_Pulse / PI / Gunzhijing ;////�γ�3������������
		g_InteralMemory.KeepWord[475] = g_InteralMemory.KeepWord[456] * g_InteralMemory.KeepWord[457] / 1000 * Encoder_Pulse / PI / Gunzhijing ;////�γ�4������������
			
		
		g_InteralMemory.KeepWord[478] = g_InteralMemory.KeepWord[477] + 10000 * (g_InteralMemory.KeepWord[44]-g_InteralMemory.KeepWord[469]) / g_InteralMemory.KeepWord[471];////���̧�����̱�����������
			
		
		tmp = g_InteralMemory.KeepWord[476] + g_InteralMemory.KeepWord[477] + g_InteralMemory.KeepWord[478];
			
		g_InteralMemory.KeepWord[479] = g_InteralMemory.KeepWord[473] - tmp ;///��1�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[480] = g_InteralMemory.KeepWord[474] - tmp ;///��2�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[481] = g_InteralMemory.KeepWord[475] - tmp ;///��3�γ����ɺ�ĵȴ�������
		g_InteralMemory.KeepWord[482] = g_InteralMemory.KeepWord[472] - tmp ;///��4�γ����ɺ�ĵȴ�������
		
		g_InteralMemory.KeepWord[483]= g_InteralMemory.KeepWord[450] +g_InteralMemory.KeepWord[452]+g_InteralMemory.KeepWord[454]+g_InteralMemory.KeepWord[456];/////������������ʾ�����ͳ����
	
		
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[450] +g_InteralMemory.KeepWord[452]+g_InteralMemory.KeepWord[454]+g_InteralMemory.KeepWord[456];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=Chongkoushu;
	
	}
	
	if(g_InteralMemory.Bit[82] == 1)/////����-������Ʒһ���
	{
		g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =0;
	//	g_InteralMemory.Bit[82] =0;
		g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =0;
		
		if( g_InteralMemory.KeepWord[10] >=  g_InteralMemory.KeepWord[161])
		{
			Set_Y_Value(7,0);//��Ƶ��FWDͣ��
			InPulse1_Stop();//��������
			g_InteralMemory.Bit[82] = 0;
			g_InteralMemory.Bit[23] = 1;//ֹͣ��ť����ִ��һ��
			if(g_InteralMemory.KeepBit[200] == 0)
				g_InteralMemory.Word[PING_CTRL] = 17;////���ݼ�����ɺ��Զ����ز�Ʒ�������ý���
			else
				g_InteralMemory.Word[PING_CTRL] = 48;
			g_InteralMemory.Bit[43] = 0;
			Dingchang_on_off = 0;
			Chongkou_on_off = 0;

		}
		/////////////////////////////////////////////////////////////////////////////////
		///////*********************************
		///////���ֲ��������޸�
		///��������	
		Dingchang = (double)(g_InteralMemory.KeepWord[139]) * g_InteralMemory.KeepWord[140] / 1000 ;
		
		g_InteralMemory.KeepWord[145] = Dingchang * Encoder_Pulse / PI / Gunzhijing ;////����������������
		
		g_InteralMemory.KeepWord[149] = g_InteralMemory.KeepWord[145] - g_InteralMemory.KeepWord[146] - g_InteralMemory.KeepWord[147] - g_InteralMemory.KeepWord[148];////ԭ��ȴ����̱�����������
		
		
		Duanchang1  = (double)(g_InteralMemory.KeepWord[135]) * g_InteralMemory.KeepWord[136] / 1000  ;
		Duanchang2  = (double)(g_InteralMemory.KeepWord[137]) * g_InteralMemory.KeepWord[138] / 1000  ;	
		
		
		g_InteralMemory.KeepWord[153] = Duanchang1 * Encoder_Pulse / PI / Gunzhijing ;////�γ�1������������
		g_InteralMemory.KeepWord[154] = Duanchang2 * Encoder_Pulse / PI / Gunzhijing ;////�γ�2������������
		
		tmp = g_InteralMemory.KeepWord[155] + g_InteralMemory.KeepWord[156] + g_InteralMemory.KeepWord[157];
		
		g_InteralMemory.KeepWord[158] = g_InteralMemory.KeepWord[154] - tmp ;///��1�γ����ɺ�ĵȴ�������
		
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[139];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=Chongkoushu;
		
	}
	
	if(g_InteralMemory.Bit[92] == 1)/////����-������Ʒ2���
	{
		g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =0;
		g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =0;
	//	g_InteralMemory.Bit[92] =0;
		
		if( g_InteralMemory.KeepWord[10] >=  g_InteralMemory.KeepWord[508])
		{
			Set_Y_Value(7,0);//��Ƶ��FWDͣ��
			InPulse1_Stop();//��������
			g_InteralMemory.Bit[92] = 0;
			g_InteralMemory.Bit[23] = 1;//ֹͣ��ť����ִ��һ��
			if(g_InteralMemory.KeepBit[200] == 0)
				g_InteralMemory.Word[PING_CTRL] = 17;////���ݼ�����ɺ��Զ����ز�Ʒ�������ý���
			else
				g_InteralMemory.Word[PING_CTRL] = 48;
			g_InteralMemory.Bit[88] = 0;
			Dingchang_on_off = 0;
			Chongkou_on_off = 0;

		}
		/////////////////////////////////////////////////////////////////////////////////
		///////*********************************
		///////���ֲ��������޸�
		
		
		Dingchang = (double)(g_InteralMemory.KeepWord[505]) * g_InteralMemory.KeepWord[506] / 1000 ;
		
		g_InteralMemory.KeepWord[512] = Dingchang * Encoder_Pulse / PI / Gunzhijing ;////����������������
		
		g_InteralMemory.KeepWord[516] = g_InteralMemory.KeepWord[512] - g_InteralMemory.KeepWord[513] - g_InteralMemory.KeepWord[514] - g_InteralMemory.KeepWord[515];////ԭ��ȴ����̱�����������
		
		Duanchang1  = (double)(g_InteralMemory.KeepWord[501]) * g_InteralMemory.KeepWord[502] / 1000  ;
		Duanchang2  = (double)(g_InteralMemory.KeepWord[503]) * g_InteralMemory.KeepWord[504] / 1000  ;	
		
		g_InteralMemory.KeepWord[520] = Duanchang1 * Encoder_Pulse / PI / Gunzhijing ;////�γ�1������������
		g_InteralMemory.KeepWord[521] = Duanchang2 * Encoder_Pulse / PI / Gunzhijing ;////�γ�2������������
		

		tmp = g_InteralMemory.KeepWord[522] + g_InteralMemory.KeepWord[523] + g_InteralMemory.KeepWord[524];
		
		g_InteralMemory.KeepWord[525] = g_InteralMemory.KeepWord[521] - tmp ;///��1�γ����ɺ�ĵȴ�������
		
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[505];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=Chongkoushu;
		
	}
	
	///////////////////////////////////
	/////////**********************
	///б����1
	if(g_InteralMemory.Bit[44] == 1)
	{
		g_InteralMemory.Bit[44] = 0;
		
	///��������
		 Pianxinju_2 = (double)g_InteralMemory.KeepWord[0] / 10.0 ;/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
		
		Dingchang = (double)(g_InteralMemory.KeepWord[104]) * g_InteralMemory.KeepWord[105] / 1000 + (double)(g_InteralMemory.KeepWord[106]) * g_InteralMemory.KeepWord[107] / 1000 
					+ (double)(g_InteralMemory.KeepWord[108]) * g_InteralMemory.KeepWord[109] / 1000 + (double)(g_InteralMemory.KeepWord[110] ) * g_InteralMemory.KeepWord[111] / 1000 ;
		
			Gunzhijing= (double)g_InteralMemory.KeepWord[2] / 10.0 ;
			   Houdu  = (double)g_InteralMemory.KeepWord[112] ;
		 
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_2)   
			Houdu = Pianxinju_2 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 
			   
			i = (Pianxinju_2 - Houdu) / Pianxinju_2 ;
			a=acos(i);//�жϹ��̵�ת���ĽǶ�
			
			g_InteralMemory.KeepWord[113] = onecircle_pulse * a / 2.0 / PI ;//�жϹ�����Ҫ������������P_KW5
			
			g_InteralMemory.KeepWord[114] = onecircle_pulse - g_InteralMemory.KeepWord[113] - g_InteralMemory.KeepWord[4] ;//�µ����̷�����������
			
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
			g_InteralMemory.KeepWord[115] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / i ;//A��KֵK_A2
			
			g_InteralMemory.KeepWord[116] = Dingchang * Encoder_Pulse / PI / Gunzhijing ;////����������������
			g_InteralMemory.KeepWord[117] = 10000 * g_InteralMemory.KeepWord[114] / g_InteralMemory.KeepWord[115];////�µ����̱�����������
				
			g_InteralMemory.KeepWord[118] = Pianxinju_2 * sin(a) * Encoder_Pulse / PI / Gunzhijing ;////�жϹ��̱�����������
			
			g_InteralMemory.KeepWord[119] = 10000 * g_InteralMemory.KeepWord[4] / g_InteralMemory.KeepWord[115];////̧�����̱�����������
		
			g_InteralMemory.KeepWord[120] = g_InteralMemory.KeepWord[116] - g_InteralMemory.KeepWord[117] - g_InteralMemory.KeepWord[118] - g_InteralMemory.KeepWord[119];////ԭ��ȴ����̱�����������
			////ע�⣺�жϹ��̱����������� �д�ʵ����תʱ������
		///////////////////////
		/*
			temp1 = a / g_InteralMemory.KeepWord[113];
			for(j=0;j<g_InteralMemory.KeepWord[113];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0.0)
					K_A2_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / cos(temp2) ;
				else
					K_A2_index[j]=g_InteralMemory.KeepWord[115];
			}*/
		/////
		/////��ڲ���
			Pianxinju_1 = (double)g_InteralMemory.KeepWord[41] / 10.0 ;
			Duanchang1  = (double)(g_InteralMemory.KeepWord[104]) * g_InteralMemory.KeepWord[105] / 1000  ;
			Duanchang2  = (double)(g_InteralMemory.KeepWord[106]) * g_InteralMemory.KeepWord[107] / 1000  ;			
			Duanchang3  = (double)(g_InteralMemory.KeepWord[108] ) * g_InteralMemory.KeepWord[109] / 1000  ;			
			Duanchang4  = (double)(g_InteralMemory.KeepWord[110]) * g_InteralMemory.KeepWord[111] / 1000  ;
			
			Daojianju   = (double)g_InteralMemory.KeepWord[42] / 10.0 ;
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_1)   
			Houdu = Pianxinju_1 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 

			i = (Pianxinju_1 - Houdu) / Pianxinju_1 ;
			a=acos(i);//��ڹ��̵�ת���ĽǶ�
			
			g_InteralMemory.KeepWord[121] = onecircle_pulse * a / 2.0 / PI ;//��ڹ�����Ҫ������������
			g_InteralMemory.KeepWord[122] = onecircle_pulse - g_InteralMemory.KeepWord[121] - g_InteralMemory.KeepWord[44] ;//�µ����̷�����������
		
			g_InteralMemory.KeepWord[123] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / i * g_InteralMemory.KeepWord[260] / 1000 ;//A��KֵK_A1*10000
			/*	
			temp1 = a / g_InteralMemory.KeepWord[121];
			for(j=0;j<=g_InteralMemory.KeepWord[121];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0)
					K_A1_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / cos(temp2) ;
				else
					K_A1_index[j]=g_InteralMemory.KeepWord[123];
			}*/
		
			g_InteralMemory.KeepWord[124] = Duanchang1 * Encoder_Pulse / PI / Gunzhijing ;////�γ�1������������
			g_InteralMemory.KeepWord[125] = Duanchang2 * Encoder_Pulse / PI / Gunzhijing ;////�γ�2������������
			g_InteralMemory.KeepWord[126] = Duanchang3 * Encoder_Pulse / PI / Gunzhijing ;////�γ�3������������
			g_InteralMemory.KeepWord[127] = Duanchang4 * Encoder_Pulse / PI / Gunzhijing ;////�γ�4������������
	
			g_InteralMemory.KeepWord[128] = 10000 * g_InteralMemory.KeepWord[122] / g_InteralMemory.KeepWord[123];////����µ����̱�����������
			
			g_InteralMemory.KeepWord[129] = Pianxinju_1 * sin(a) * Encoder_Pulse / PI / Gunzhijing * 1000 / g_InteralMemory.KeepWord[260];////��ڹ��̱�����������
			
			g_InteralMemory.KeepWord[130] = g_InteralMemory.KeepWord[129] + 10000 * (g_InteralMemory.KeepWord[44]-g_InteralMemory.KeepWord[121]) / g_InteralMemory.KeepWord[123];////���̧�����̱�����������
			
			
			tmp = g_InteralMemory.KeepWord[128] + g_InteralMemory.KeepWord[129] + g_InteralMemory.KeepWord[130];
			
			g_InteralMemory.KeepWord[131] = g_InteralMemory.KeepWord[125] - tmp ;///��1�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[132] = g_InteralMemory.KeepWord[126] - tmp ;///��2�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[133] = g_InteralMemory.KeepWord[127] - tmp ;///��3�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[134] = g_InteralMemory.KeepWord[124] - tmp ;///��4�γ����ɺ�ĵȴ�������
			
			g_InteralMemory.KeepWord[163]= g_InteralMemory.KeepWord[104] +g_InteralMemory.KeepWord[106]+g_InteralMemory.KeepWord[108]+g_InteralMemory.KeepWord[110];/////������������ʾ�����ͳ����
		
		/////
		///������ݴ������
		
		if(g_InteralMemory.KeepBit[200] == 0)
			g_InteralMemory.Word[PING_CTRL] = 20;////���ݼ�����ɺ��Զ����ز�Ʒ�������ý���
		else
			g_InteralMemory.Word[PING_CTRL] = 37;
	}
	if(g_InteralMemory.Bit[47] ==1)
	{
		InPulse1_Stop();//�л���Ʒǰ����ͣ�������� ������ȫ��������ٿ�����������
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		g_InteralMemory.Bit[47] = 0;
		g_InteralMemory.Bit[42] =1 ;
		g_InteralMemory.Bit[8] = 0;
		g_InteralMemory.Bit[9] = 0;
		g_InteralMemory.Bit[10] = 0;
		g_InteralMemory.Bit[30] = 0;
		g_InteralMemory.Bit[31] = 0;
		g_InteralMemory.Bit[32] = 0;
		g_InteralMemory.Bit[38] = 0;
		g_InteralMemory.Bit[43] = 0;
		////////
	//	g_InteralMemory.Bit[81] = 1;
		g_InteralMemory.KeepWord[10] = 0;
		g_InteralMemory.KeepBit[13] = 9;
		
		g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =1;
		g_InteralMemory.Bit[82] =0;
		
		g_InteralMemory.Bit[86] =0;
		g_InteralMemory.Bit[88] =0;
		
		g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =0;
		////��������
		
		 Pianxinju_2 = (double)g_InteralMemory.KeepWord[0] / 10.0 ;/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
			
			  Houdu  = (double)g_InteralMemory.KeepWord[112] ;
			  
		    /**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_2)   
			Houdu = Pianxinju_2 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 
			
			i = (Pianxinju_2 - Houdu) / Pianxinju_2 ;
			a=acos(i);//�жϹ��̵�ת���ĽǶ�
			
			temp1 = a / g_InteralMemory.KeepWord[113];
			for(j=0;j<g_InteralMemory.KeepWord[113];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0.0)
					K_A2_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / cos(temp2) ;
				else
					K_A2_index[j]=g_InteralMemory.KeepWord[115];
			}
		
		Pianxinju_1 = (double)g_InteralMemory.KeepWord[41] / 10.0 ;
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_1)   
			Houdu = Pianxinju_1 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 

			i = (Pianxinju_1 - Houdu) / Pianxinju_1 ;
			a=acos(i);//��ڹ��̵�ת���ĽǶ�
			
			temp1 = a / g_InteralMemory.KeepWord[121];
			for(j=0;j<=g_InteralMemory.KeepWord[121];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0)
					K_A1_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / cos(temp2) * g_InteralMemory.KeepWord[260] / 1000 ;
				else
					K_A1_index[j]=g_InteralMemory.KeepWord[123];
			}
		
		///////////////////////////////////////
	//////��Ʒ����
	////���������K_A2,dK2,Inpulse_xiadao2,Inpulse_qieduan2,Inpulse_taidao2,Inpulse_dengdai2,Outpulse_xiadao2,Outpulse_qieduan2,Outpulse_taidao2
		K_A2 = g_InteralMemory.KeepWord[115];
		
		Inpulse_xiadao2 = g_InteralMemory.KeepWord[117];
		Inpulse_qieduan2 = g_InteralMemory.KeepWord[118];
		Inpulse_taidao2 = g_InteralMemory.KeepWord[119];
	
		Inpulse_dengdai2[0] = g_InteralMemory.KeepWord[120];
		Inpulse_dengdai2[1] = g_InteralMemory.KeepWord[120];
		
		Outpulse_xiadao2 = g_InteralMemory.KeepWord[114];
		Outpulse_qieduan2 = g_InteralMemory.KeepWord[113];
		Outpulse_taidao2 = g_InteralMemory.KeepWord[4];
		
		Dingchang_on_off = 1;
	
	//////////////////////
	////������
	// K_A1,dK1,Inpulse_xiadao1,Inpulse_qieduan1,Inpulse_taidao1,Outpulse_xiadao1,Outpulse_qieduan1,Outpulse_taidao1;////������ȫ�ֱ���
	// Inpulse_dengdai1[13],Inpulse_all1[13];
		
		Inpulse_xiadao1 = g_InteralMemory.KeepWord[128];
		Inpulse_qieduan1 = g_InteralMemory.KeepWord[129];
		Inpulse_taidao1 = g_InteralMemory.KeepWord[130];
		Outpulse_xiadao1 =  g_InteralMemory.KeepWord[122];
		Outpulse_qieduan1 =  g_InteralMemory.KeepWord[121];
		Outpulse_taidao1 =  g_InteralMemory.KeepWord[44];
		
		K_A1 = g_InteralMemory.KeepWord[123];
	
		Inpulse_dengdai1[0] = g_InteralMemory.KeepWord[134];
		Inpulse_dengdai1[1] = g_InteralMemory.KeepWord[131];
		Inpulse_dengdai1[2] = g_InteralMemory.KeepWord[132];
		Inpulse_dengdai1[3] = g_InteralMemory.KeepWord[133];
		/*
		if(g_InteralMemory.KeepBit[15] == 0)
			Chongkoushu = 3;
		else if(g_InteralMemory.KeepBit[15] == 1)
			Chongkoushu = 0;*/
		Chongkoushu = g_InteralMemory.KeepWord[168];
		
		Chongkou_on_off = 1;
	//	chongkou_last_length = g_InteralMemory.KeepWord[60];
	////////////////////////////////////////////
		g_InteralMemory.Word[501]=0;
		g_InteralMemory.Word[502]=0;
		g_InteralMemory.Word[503]=0;
		g_InteralMemory.Word[504]=0;
		g_InteralMemory.Word[505]=0;
		g_InteralMemory.Word[506]=0;
		g_InteralMemory.Word[507]=0;
		g_InteralMemory.Word[508]=0;
		g_InteralMemory.Word[509]=0;////��������������
		g_InteralMemory.Word[511]=0;
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		chongkou_current = 0;
		dingchang_current = 0;
		current_qieduan = 0;
		need_qieduan = 0;
		pulseout_qieduan1 = 0;
		pulseout_qieduan2 = 0;
		qieduan_enable = 0;
		cut_all = 0;
		
		for(j=0; j<10; j++)
			Enable_qieduan[j]=0;
			
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[104] +g_InteralMemory.KeepWord[106]+g_InteralMemory.KeepWord[108]+g_InteralMemory.KeepWord[110];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=Chongkoushu;
			
		/////////////////////////////////////////////////////
		g_InteralMemory.KeepBit[10] = 0;
		g_InteralMemory.KeepBit[12] = 0;
		
		if(g_InteralMemory.Bit[27]==1)///ϵͳ����
			InPulse1_Start();
	
	}
	
	
	
	
	///////////////////////////////////
	/////////**********************
	///б����2
	if(g_InteralMemory.Bit[95] == 1)
	{
		g_InteralMemory.Bit[95] = 0;
		
	///��������
		 Pianxinju_2 = (double)g_InteralMemory.KeepWord[0] / 10.0 ;/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
		
		Dingchang = (double)(g_InteralMemory.KeepWord[450]) * g_InteralMemory.KeepWord[451] / 1000 + (double)(g_InteralMemory.KeepWord[452]) * g_InteralMemory.KeepWord[453] / 1000 
					+ (double)(g_InteralMemory.KeepWord[454]) * g_InteralMemory.KeepWord[455] / 1000 + (double)(g_InteralMemory.KeepWord[456] ) * g_InteralMemory.KeepWord[457] / 1000 ;
		
			Gunzhijing= (double)g_InteralMemory.KeepWord[2] / 10.0 ;
			   Houdu  = (double)g_InteralMemory.KeepWord[458] ;
		 
			   
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_2)   
			Houdu = Pianxinju_2 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 
			
			i = (Pianxinju_2 - Houdu) / Pianxinju_2 ;
			a=acos(i);//�жϹ��̵�ת���ĽǶ�
			
			g_InteralMemory.KeepWord[461] = onecircle_pulse * a / 2.0 / PI ;//�жϹ�����Ҫ������������P_KW5
			
			g_InteralMemory.KeepWord[462] = onecircle_pulse - g_InteralMemory.KeepWord[461] - g_InteralMemory.KeepWord[4] ;//�µ����̷�����������
			
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
			g_InteralMemory.KeepWord[463] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / i ;//A��KֵK_A2
			
			g_InteralMemory.KeepWord[464] = Dingchang * Encoder_Pulse / PI / Gunzhijing ;////����������������
			g_InteralMemory.KeepWord[465] = 10000 * g_InteralMemory.KeepWord[462] / g_InteralMemory.KeepWord[463];////�µ����̱�����������
				
			g_InteralMemory.KeepWord[466] = Pianxinju_2 * sin(a) * Encoder_Pulse / PI / Gunzhijing ;////�жϹ��̱�����������
			
			g_InteralMemory.KeepWord[467] = 10000 * g_InteralMemory.KeepWord[4] / g_InteralMemory.KeepWord[463];////̧�����̱�����������
		
			g_InteralMemory.KeepWord[468] = g_InteralMemory.KeepWord[464] - g_InteralMemory.KeepWord[465] - g_InteralMemory.KeepWord[466] - g_InteralMemory.KeepWord[467];////ԭ��ȴ����̱�����������
			////ע�⣺�жϹ��̱����������� �д�ʵ����תʱ������
		///////////////////////
		/*
			temp1 = a / g_InteralMemory.KeepWord[461];
			for(j=0;j<g_InteralMemory.KeepWord[461];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0.0)
					K_A2_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / cos(temp2) ;
				else
					K_A2_index[j]=g_InteralMemory.KeepWord[463];
			}*/
		/////
		/////��ڲ���
			Pianxinju_1 = (double)g_InteralMemory.KeepWord[41] / 10.0 ;
			Duanchang1  = (double)(g_InteralMemory.KeepWord[450]) * g_InteralMemory.KeepWord[451] / 1000  ;
			Duanchang2  = (double)(g_InteralMemory.KeepWord[452]) * g_InteralMemory.KeepWord[453] / 1000  ;			
			Duanchang3  = (double)(g_InteralMemory.KeepWord[454]) * g_InteralMemory.KeepWord[455] / 1000  ;			
			Duanchang4  = (double)(g_InteralMemory.KeepWord[456]) * g_InteralMemory.KeepWord[457] / 1000  ;
			
			Daojianju   = (double)g_InteralMemory.KeepWord[42] / 10.0 ;
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_1)   
			Houdu = Pianxinju_1 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 

			i = (Pianxinju_1 - Houdu) / Pianxinju_1 ;
			a=acos(i);//��ڹ��̵�ת���ĽǶ�
			
			g_InteralMemory.KeepWord[469] = onecircle_pulse * a / 2.0 / PI ;//��ڹ�����Ҫ������������
			g_InteralMemory.KeepWord[470] = onecircle_pulse - g_InteralMemory.KeepWord[469] - g_InteralMemory.KeepWord[44] ;//�µ����̷�����������
		
			g_InteralMemory.KeepWord[471] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / i * g_InteralMemory.KeepWord[260] / 1000 ;//A��KֵK_A1*10000
			/*	
			temp1 = a / g_InteralMemory.KeepWord[469];
			for(j=0;j<=g_InteralMemory.KeepWord[469];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0)
					K_A1_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / cos(temp2) ;
				else
					K_A1_index[j]=g_InteralMemory.KeepWord[471];
			}*/
		
			g_InteralMemory.KeepWord[472] = Duanchang1 * Encoder_Pulse / PI / Gunzhijing ;////�γ�1������������
			g_InteralMemory.KeepWord[473] = Duanchang2 * Encoder_Pulse / PI / Gunzhijing ;////�γ�2������������
			g_InteralMemory.KeepWord[474] = Duanchang3 * Encoder_Pulse / PI / Gunzhijing ;////�γ�3������������
			g_InteralMemory.KeepWord[475] = Duanchang4 * Encoder_Pulse / PI / Gunzhijing ;////�γ�4������������
	
			g_InteralMemory.KeepWord[476] = 10000 * g_InteralMemory.KeepWord[470] / g_InteralMemory.KeepWord[471];////����µ����̱�����������
			
			g_InteralMemory.KeepWord[477] = Pianxinju_1 * sin(a) * Encoder_Pulse / PI / Gunzhijing * 1000 / g_InteralMemory.KeepWord[260];////��ڹ��̱�����������
			
			g_InteralMemory.KeepWord[478] = g_InteralMemory.KeepWord[477] + 10000 * (g_InteralMemory.KeepWord[44]-g_InteralMemory.KeepWord[469]) / g_InteralMemory.KeepWord[471];////���̧�����̱�����������
			
			
			tmp = g_InteralMemory.KeepWord[476] + g_InteralMemory.KeepWord[477] + g_InteralMemory.KeepWord[478];
			
			g_InteralMemory.KeepWord[479] = g_InteralMemory.KeepWord[473] - tmp ;///��1�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[480] = g_InteralMemory.KeepWord[474] - tmp ;///��2�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[481] = g_InteralMemory.KeepWord[475] - tmp ;///��3�γ����ɺ�ĵȴ�������
			g_InteralMemory.KeepWord[482] = g_InteralMemory.KeepWord[472] - tmp ;///��4�γ����ɺ�ĵȴ�������
			
			g_InteralMemory.KeepWord[483]= g_InteralMemory.KeepWord[450] +g_InteralMemory.KeepWord[452]+g_InteralMemory.KeepWord[454]+g_InteralMemory.KeepWord[456];/////������������ʾ�����ͳ����
		
		/////
		///������ݴ������
		
		if(g_InteralMemory.KeepBit[200] == 0)
			g_InteralMemory.Word[PING_CTRL] = 24;////���ݼ�����ɺ��Զ����ز�Ʒ�������ý���
		else
			g_InteralMemory.Word[PING_CTRL] = 39;
	}
	if(g_InteralMemory.Bit[85] ==1)
	{
		InPulse1_Stop();//�л���Ʒǰ����ͣ�������� ������ȫ��������ٿ�����������
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		g_InteralMemory.Bit[85] = 0;
		
		g_InteralMemory.Bit[8] = 0;
		g_InteralMemory.Bit[9] = 0;
		g_InteralMemory.Bit[10] = 0;
		g_InteralMemory.Bit[30] = 0;
		g_InteralMemory.Bit[31] = 0;
		g_InteralMemory.Bit[32] = 0;
		g_InteralMemory.Bit[38] = 0;
		g_InteralMemory.Bit[43] = 0;
		g_InteralMemory.Bit[42] = 0;
		////////
	//	g_InteralMemory.Bit[81] = 1;
		g_InteralMemory.KeepWord[10] = 0;
		g_InteralMemory.KeepBit[13] = 10;
		
		g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =0;
		
		g_InteralMemory.Bit[86] =1;
		g_InteralMemory.Bit[88] =0;
		
		g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =1;
		g_InteralMemory.Bit[92] =0;
		////��������
		////��������
		 Pianxinju_2 = (double)g_InteralMemory.KeepWord[0] / 10.0 ;/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
		
		
			   Houdu  = (double)g_InteralMemory.KeepWord[458] ;
		
			   /**************Bug�޸� by����ΰ*********************/ 
			   if(Houdu >= Pianxinju_2)
			   Houdu = Pianxinju_2 - 1.0; 
			   /**************Bug�޸� by����ΰ*********************/ 
			   
			i = (Pianxinju_2 - Houdu) / Pianxinju_2 ;
			a=acos(i);//�жϹ��̵�ת���ĽǶ�
			
		temp1 = a / g_InteralMemory.KeepWord[461];
		for(j=0;j<g_InteralMemory.KeepWord[461];j++)
		{
			temp2 = a-j*temp1;
			if(temp2 >= 0.0)
				K_A2_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / cos(temp2);
			else
				K_A2_index[j]=g_InteralMemory.KeepWord[463];
		}
		
		Pianxinju_1 = (double)g_InteralMemory.KeepWord[41] / 10.0 ;
		/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_1)   
			Houdu = Pianxinju_1 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 
	
		i = (Pianxinju_1 - Houdu) / Pianxinju_1 ;
		a=acos(i);//��ڹ��̵�ת���ĽǶ�
		
			
		temp1 = a / g_InteralMemory.KeepWord[469];
		for(j=0;j<=g_InteralMemory.KeepWord[469];j++)
		{
			temp2 = a-j*temp1;
			if(temp2 >= 0)
				K_A1_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / cos(temp2)  * g_InteralMemory.KeepWord[260] / 1000 ;
			else
				K_A1_index[j]=g_InteralMemory.KeepWord[471];
		}
		
		///////////////////////////////////////
	//////��Ʒ����
	////���������K_A2,dK2,Inpulse_xiadao2,Inpulse_qieduan2,Inpulse_taidao2,Inpulse_dengdai2,Outpulse_xiadao2,Outpulse_qieduan2,Outpulse_taidao2
		K_A2 = g_InteralMemory.KeepWord[463];
		
		Inpulse_xiadao2 = g_InteralMemory.KeepWord[465];
		Inpulse_qieduan2 = g_InteralMemory.KeepWord[466];
		Inpulse_taidao2 = g_InteralMemory.KeepWord[467];
	
		Inpulse_dengdai2[0] = g_InteralMemory.KeepWord[468];
		Inpulse_dengdai2[1] = g_InteralMemory.KeepWord[468];
		
		Outpulse_xiadao2 = g_InteralMemory.KeepWord[462];
		Outpulse_qieduan2 = g_InteralMemory.KeepWord[461];
		Outpulse_taidao2 = g_InteralMemory.KeepWord[4];
		
		Dingchang_on_off = 1;
	
	//////////////////////
	////������
	// K_A1,dK1,Inpulse_xiadao1,Inpulse_qieduan1,Inpulse_taidao1,Outpulse_xiadao1,Outpulse_qieduan1,Outpulse_taidao1;////������ȫ�ֱ���
	// Inpulse_dengdai1[13],Inpulse_all1[13];
		
		Inpulse_xiadao1 = g_InteralMemory.KeepWord[476];
		Inpulse_qieduan1 = g_InteralMemory.KeepWord[477];
		Inpulse_taidao1 = g_InteralMemory.KeepWord[478];
		Outpulse_xiadao1 =  g_InteralMemory.KeepWord[470];
		Outpulse_qieduan1 =  g_InteralMemory.KeepWord[469];
		Outpulse_taidao1 =  g_InteralMemory.KeepWord[44];
		
		K_A1 = g_InteralMemory.KeepWord[471];
	
		Inpulse_dengdai1[0] = g_InteralMemory.KeepWord[482];
		Inpulse_dengdai1[1] = g_InteralMemory.KeepWord[479];
		Inpulse_dengdai1[2] = g_InteralMemory.KeepWord[480];
		Inpulse_dengdai1[3] = g_InteralMemory.KeepWord[481];
		
		Chongkoushu = g_InteralMemory.KeepWord[459];
		
		Chongkou_on_off = 1;
	//	chongkou_last_length = g_InteralMemory.KeepWord[60];
	////////////////////////////////////////////
		g_InteralMemory.Word[501]=0;
		g_InteralMemory.Word[502]=0;
		g_InteralMemory.Word[503]=0;
		g_InteralMemory.Word[504]=0;
		g_InteralMemory.Word[505]=0;
		g_InteralMemory.Word[506]=0;
		g_InteralMemory.Word[507]=0;
		g_InteralMemory.Word[508]=0;
		g_InteralMemory.Word[509]=0;////��������������
		g_InteralMemory.Word[511]=0;
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		chongkou_current = 0;
		dingchang_current = 0;
		current_qieduan = 0;
		need_qieduan = 0;
		pulseout_qieduan1 = 0;
		pulseout_qieduan2 = 0;
		qieduan_enable = 0;
		cut_all = 0;
		
		for(j=0; j<10; j++)
			Enable_qieduan[j]=0;
			
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[450] +g_InteralMemory.KeepWord[452]+g_InteralMemory.KeepWord[454]+g_InteralMemory.KeepWord[456];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=Chongkoushu;
			
		/////////////////////////////////////////////////////
		g_InteralMemory.KeepBit[10] = 0;
		g_InteralMemory.KeepBit[12] = 0;
		
		if(g_InteralMemory.Bit[27]==1)///ϵͳ����
			InPulse1_Start();
	
	}
	
	///////////////////////////////////////////////
	//////////*************************************
	/////��������1
	if(g_InteralMemory.Bit[45] == 1)
	{
		g_InteralMemory.Bit[45] = 0;
		
		
		///��������
		 Pianxinju_2 = (double)g_InteralMemory.KeepWord[0] / 10.0 ;/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
		
		Dingchang = (double)(g_InteralMemory.KeepWord[139]) * g_InteralMemory.KeepWord[140] / 1000 ;
		
			Gunzhijing= (double)g_InteralMemory.KeepWord[2] / 10.0 ;
			   Houdu  = (double)g_InteralMemory.KeepWord[141] ;
		 
			/**************Bug�޸� by����ΰ*********************/   
			if(Houdu >= Pianxinju_2)
			Houdu = Pianxinju_2 - 1.0; 
			/**************Bug�޸� by����ΰ*********************/ 
		   
			i = (Pianxinju_2 - Houdu) / Pianxinju_2 ;
			a=acos(i);//�жϹ��̵�ת���ĽǶ�
			
			g_InteralMemory.KeepWord[142] = onecircle_pulse * a / 2.0 / PI ;//�жϹ�����Ҫ������������P_KW5
			
			g_InteralMemory.KeepWord[143] = onecircle_pulse - g_InteralMemory.KeepWord[142] - g_InteralMemory.KeepWord[4] ;//�µ����̷�����������
			
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
			g_InteralMemory.KeepWord[144] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / i ;//A��KֵK_A2
			
			g_InteralMemory.KeepWord[145] = Dingchang * Encoder_Pulse / PI / Gunzhijing ;////����������������
			g_InteralMemory.KeepWord[146] = 10000 * g_InteralMemory.KeepWord[143] / g_InteralMemory.KeepWord[144];////�µ����̱�����������
				
			g_InteralMemory.KeepWord[147] = Pianxinju_2 * sin(a) * Encoder_Pulse / PI / Gunzhijing ;////�жϹ��̱�����������
			
			g_InteralMemory.KeepWord[148] = 10000 * g_InteralMemory.KeepWord[4] / g_InteralMemory.KeepWord[144];////̧�����̱�����������
		
			g_InteralMemory.KeepWord[149] = g_InteralMemory.KeepWord[145] - g_InteralMemory.KeepWord[146] - g_InteralMemory.KeepWord[147] - g_InteralMemory.KeepWord[148];////ԭ��ȴ����̱�����������
			////ע�⣺�жϹ��̱����������� �д�ʵ����תʱ������
		///////////////////////
		/*
			temp1 = a / g_InteralMemory.KeepWord[142];
			for(j=0;j<g_InteralMemory.KeepWord[142];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0.0)
					K_A2_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / cos(temp2) ;
				else
					K_A2_index[j]=g_InteralMemory.KeepWord[144];
			}*/
		/////
		/////��ڲ���
			Pianxinju_1 = (double)g_InteralMemory.KeepWord[41] / 10.0 ;
			Duanchang1  = (double)(g_InteralMemory.KeepWord[135]) * g_InteralMemory.KeepWord[136] / 1000  ;
			Duanchang2  = (double)(g_InteralMemory.KeepWord[137]) * g_InteralMemory.KeepWord[138] / 1000  ;	
			
			
			Daojianju   = (double)g_InteralMemory.KeepWord[42] / 10.0 ;
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_1)   
			Houdu = Pianxinju_1 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 

			i = (Pianxinju_1 - Houdu) / Pianxinju_1 ;
			a=acos(i);//��ڹ��̵�ת���ĽǶ�
			
			g_InteralMemory.KeepWord[150] = onecircle_pulse * a / 2.0 / PI ;//��ڹ�����Ҫ������������
			g_InteralMemory.KeepWord[151] = onecircle_pulse - g_InteralMemory.KeepWord[150] - g_InteralMemory.KeepWord[44] ;//�µ����̷�����������
		
			g_InteralMemory.KeepWord[152] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / i * g_InteralMemory.KeepWord[260] / 1000 ;//A��KֵK_A1*10000
			/*	
			temp1 = a / g_InteralMemory.KeepWord[150];
			for(j=0;j<=g_InteralMemory.KeepWord[150];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0)
					K_A1_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / cos(temp2) ;
				else
					K_A1_index[j]=g_InteralMemory.KeepWord[152];
			}*/
		
		
		
			
			g_InteralMemory.KeepWord[153] = Duanchang1 * Encoder_Pulse / PI / Gunzhijing ;////�γ�1������������
			g_InteralMemory.KeepWord[154] = Duanchang2 * Encoder_Pulse / PI / Gunzhijing ;////�γ�2������������
			
	
			g_InteralMemory.KeepWord[155] = 10000 * g_InteralMemory.KeepWord[151] / g_InteralMemory.KeepWord[152];////����µ����̱�����������
			
			g_InteralMemory.KeepWord[156] = Pianxinju_1 * sin(a) * Encoder_Pulse / PI / Gunzhijing * 1000 / g_InteralMemory.KeepWord[260];////��ڹ��̱�����������
			
			g_InteralMemory.KeepWord[157] = g_InteralMemory.KeepWord[156] + 10000 * (g_InteralMemory.KeepWord[44]-g_InteralMemory.KeepWord[150]) / g_InteralMemory.KeepWord[152];////���̧�����̱�����������
			
			
			tmp = g_InteralMemory.KeepWord[155] + g_InteralMemory.KeepWord[156] + g_InteralMemory.KeepWord[157];
			
			g_InteralMemory.KeepWord[158] = g_InteralMemory.KeepWord[154] - tmp ;///��1�γ����ɺ�ĵȴ�������
			
			
				g_InteralMemory.KeepWord[613]= g_InteralMemory.KeepWord[104] +g_InteralMemory.KeepWord[106]+g_InteralMemory.KeepWord[108]+g_InteralMemory.KeepWord[110];/////������������ʾ�����ͳ����
	
		/////
		///������ݴ������
		
		if(g_InteralMemory.KeepBit[200] == 0)
			g_InteralMemory.Word[PING_CTRL] = 21;////���ݼ�����ɺ��Զ����ز�Ʒ�������ý���
		else
			g_InteralMemory.Word[PING_CTRL] = 41;
	}
	if(g_InteralMemory.Bit[48] == 1)
	{
		InPulse1_Stop();//�л���Ʒǰ����ͣ�������� ������ȫ��������ٿ�����������
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		g_InteralMemory.Bit[48]=0;
		g_InteralMemory.Bit[43] =1 ;
		g_InteralMemory.Bit[8] = 0;
		g_InteralMemory.Bit[9] = 0;
		g_InteralMemory.Bit[10] = 0;
		g_InteralMemory.Bit[30] = 0;
		g_InteralMemory.Bit[31] = 0;
		g_InteralMemory.Bit[32] = 0;
		g_InteralMemory.Bit[38] = 0;
		g_InteralMemory.Bit[42] = 0;
		////////
		//g_InteralMemory.Bit[82] = 1;
		g_InteralMemory.KeepWord[10] = 0;
		g_InteralMemory.KeepBit[13] = 11;
		
		g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =1;
		
		g_InteralMemory.Bit[86] =0;
		g_InteralMemory.Bit[88] =0;
		
		g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =0;
		////��������
		
		 Pianxinju_2 = (double)g_InteralMemory.KeepWord[0] / 10.0 ;/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
			  Houdu  = (double)g_InteralMemory.KeepWord[141] ;		 
			
			/**************Bug�޸� by����ΰ*********************/  
			if(Houdu >= Pianxinju_2)  
			Houdu = Pianxinju_2 - 1.0;   
			/**************Bug�޸� by����ΰ*********************/
			
			i = (Pianxinju_2 - Houdu) / Pianxinju_2 ;
			
			a=acos(i);//�жϹ��̵�ת���ĽǶ�
			temp1 = a / g_InteralMemory.KeepWord[142];
			for(j=0;j<g_InteralMemory.KeepWord[142];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0.0)
					K_A2_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / cos(temp2) ;
				else
					K_A2_index[j]=g_InteralMemory.KeepWord[144];
			}
			
		Pianxinju_1 = (double)g_InteralMemory.KeepWord[41] / 10.0 ;
		
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_1)   
			Houdu = Pianxinju_1 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 

		i = (Pianxinju_1 - Houdu) / Pianxinju_1 ;
			a=acos(i);//��ڹ��̵�ת���ĽǶ�
				
			temp1 = a / g_InteralMemory.KeepWord[150];
			for(j=0;j<=g_InteralMemory.KeepWord[150];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0)
					K_A1_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / cos(temp2) * g_InteralMemory.KeepWord[260] / 1000 ;
				else
					K_A1_index[j]=g_InteralMemory.KeepWord[152];
			}
		///////////////////////////////////////
	//////��Ʒ����
	////���������K_A2,dK2,Inpulse_xiadao2,Inpulse_qieduan2,Inpulse_taidao2,Inpulse_dengdai2,Outpulse_xiadao2,Outpulse_qieduan2,Outpulse_taidao2
		K_A2 = g_InteralMemory.KeepWord[144];
		
		Inpulse_xiadao2 = g_InteralMemory.KeepWord[146];
		Inpulse_qieduan2 = g_InteralMemory.KeepWord[147];
		Inpulse_taidao2 = g_InteralMemory.KeepWord[148];
	
		Inpulse_dengdai2[0] = g_InteralMemory.KeepWord[149];
		Inpulse_dengdai2[1] = g_InteralMemory.KeepWord[149];
		
		Outpulse_xiadao2 = g_InteralMemory.KeepWord[143];
		Outpulse_qieduan2 = g_InteralMemory.KeepWord[142];
		Outpulse_taidao2 = g_InteralMemory.KeepWord[4];
		
		Dingchang_on_off = 1;
	
	//////////////////////
	////������
	// K_A1,dK1,Inpulse_xiadao1,Inpulse_qieduan1,Inpulse_taidao1,Outpulse_xiadao1,Outpulse_qieduan1,Outpulse_taidao1;////������ȫ�ֱ���
	// Inpulse_dengdai1[13],Inpulse_all1[13];
		
		Inpulse_xiadao1 = g_InteralMemory.KeepWord[155];
		Inpulse_qieduan1 = g_InteralMemory.KeepWord[156];
		Inpulse_taidao1 = g_InteralMemory.KeepWord[157];
		Outpulse_xiadao1 =  g_InteralMemory.KeepWord[151];
		Outpulse_qieduan1 =  g_InteralMemory.KeepWord[150];
		Outpulse_taidao1 =  g_InteralMemory.KeepWord[44];
		
		K_A1 = g_InteralMemory.KeepWord[152];
	
		Inpulse_dengdai1[0] =g_InteralMemory.KeepWord[153] - Inpulse_xiadao1-Inpulse_qieduan1-Inpulse_taidao1;
		
		
	
		Chongkoushu =  (g_InteralMemory.KeepWord[139]-g_InteralMemory.KeepWord[135]) /  g_InteralMemory.KeepWord[137] +1;
		
		Inpulse_dengdai1[0] =g_InteralMemory.KeepWord[153]  + g_InteralMemory.KeepWord[145]-g_InteralMemory.KeepWord[154] * (Chongkoushu-1)-g_InteralMemory.KeepWord[153]-Inpulse_xiadao1-Inpulse_qieduan1-Inpulse_taidao1;
		
		for(j=1;j<=Chongkoushu;j++)
			Inpulse_dengdai1[j] = g_InteralMemory.KeepWord[158];
		
		//Inpulse_dengdai1[99] = Dingchang-(Chongkoushu-1)*Duanchang2-Duanchang1;
		Chongkou_on_off = 1;
		chongkou_last_length = g_InteralMemory.KeepWord[145]-g_InteralMemory.KeepWord[154] * (Chongkoushu-1)-g_InteralMemory.KeepWord[153] ;
	////////////////////////////////////////////
		g_InteralMemory.Word[501]=0;
		g_InteralMemory.Word[502]=0;
		g_InteralMemory.Word[503]=0;
		g_InteralMemory.Word[504]=0;
		g_InteralMemory.Word[505]=0;
		g_InteralMemory.Word[506]=0;
		g_InteralMemory.Word[507]=0;
		g_InteralMemory.Word[508]=0;
		g_InteralMemory.Word[509]=0;////��������������
		g_InteralMemory.Word[511]=0;
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		chongkou_current = 0;
		dingchang_current = 0;
		current_qieduan = 0;
		need_qieduan = 0;
		pulseout_qieduan1 = 0;
		pulseout_qieduan2 = 0;
		qieduan_enable = 0;
		cut_all = 0;
		
		for(j=0; j<10; j++)
			Enable_qieduan[j]=0;
			
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[139];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=Chongkoushu;
			
		/////////////////////////////////////////////////////
		g_InteralMemory.KeepBit[10] = 0;
		g_InteralMemory.KeepBit[12] = 0;
		
		if(g_InteralMemory.Bit[27]==1)///ϵͳ����
			InPulse1_Start();
	
	}
	
	
	//////////*************************************
	/////��������2
	if(g_InteralMemory.Bit[96] == 1)
	{
		g_InteralMemory.Bit[96] = 0;
		
		
		///��������
		 Pianxinju_2 = (double)g_InteralMemory.KeepWord[0] / 10.0 ;/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
		
		Dingchang = (double)(g_InteralMemory.KeepWord[505]) * g_InteralMemory.KeepWord[506] / 1000 ;
		
			Gunzhijing= (double)g_InteralMemory.KeepWord[2] / 10.0 ;
			   Houdu  = (double)g_InteralMemory.KeepWord[507] ;
		 
			/**************Bug�޸� by����ΰ*********************/   
			if(Houdu >= Pianxinju_2)
			Houdu = Pianxinju_2 - 1.0; 
			/**************Bug�޸� by����ΰ*********************/   
			
			i = (Pianxinju_2 - Houdu) / Pianxinju_2 ;
			a=acos(i);//�жϹ��̵�ת���ĽǶ�
			
			g_InteralMemory.KeepWord[509] = onecircle_pulse * a / 2.0 / PI ;//�жϹ�����Ҫ������������P_KW5
			
			g_InteralMemory.KeepWord[510] = onecircle_pulse - g_InteralMemory.KeepWord[509] - g_InteralMemory.KeepWord[4] ;//�µ����̷�����������
			
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
			g_InteralMemory.KeepWord[511] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / i ;//A��KֵK_A2
			
			g_InteralMemory.KeepWord[512] = Dingchang * Encoder_Pulse / PI / Gunzhijing ;////����������������
			g_InteralMemory.KeepWord[513] = 10000 * g_InteralMemory.KeepWord[510] / g_InteralMemory.KeepWord[511];////�µ����̱�����������
				
			g_InteralMemory.KeepWord[514] = Pianxinju_2 * sin(a) * Encoder_Pulse / PI / Gunzhijing ;////�жϹ��̱�����������
			
			g_InteralMemory.KeepWord[515] = 10000 * g_InteralMemory.KeepWord[4] / g_InteralMemory.KeepWord[511];////̧�����̱�����������
		
			g_InteralMemory.KeepWord[516] = g_InteralMemory.KeepWord[512] - g_InteralMemory.KeepWord[513] - g_InteralMemory.KeepWord[514] - g_InteralMemory.KeepWord[515];////ԭ��ȴ����̱�����������
			////ע�⣺�жϹ��̱����������� �д�ʵ����תʱ������
		///////////////////////
		/*
			temp1 = a / g_InteralMemory.KeepWord[509];
			for(j=0;j<g_InteralMemory.KeepWord[509];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0.0)
					K_A2_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / cos(temp2) ;
				else
					K_A2_index[j]=g_InteralMemory.KeepWord[511];
			}*/
		/////
		/////��ڲ���
			Pianxinju_1 = (double)g_InteralMemory.KeepWord[41] / 10.0 ;
			Duanchang1  = (double)(g_InteralMemory.KeepWord[501]) * g_InteralMemory.KeepWord[502] / 1000  ;
			Duanchang2  = (double)(g_InteralMemory.KeepWord[503]) * g_InteralMemory.KeepWord[504] / 1000  ;	
			
			
			Daojianju   = (double)g_InteralMemory.KeepWord[42] / 10.0 ;
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_1)   
			Houdu = Pianxinju_1 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 

			i = (Pianxinju_1 - Houdu) / Pianxinju_1 ;
			a=acos(i);//��ڹ��̵�ת���ĽǶ�
			
			g_InteralMemory.KeepWord[517] = onecircle_pulse * a / 2.0 / PI ;//��ڹ�����Ҫ������������
			g_InteralMemory.KeepWord[518] = onecircle_pulse - g_InteralMemory.KeepWord[517] - g_InteralMemory.KeepWord[44] ;//�µ����̷�����������
		
			g_InteralMemory.KeepWord[519] = 10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / i * g_InteralMemory.KeepWord[260] / 1000 ;//A��KֵK_A1*10000
			/*	
			temp1 = a / g_InteralMemory.KeepWord[517];
			for(j=0;j<=g_InteralMemory.KeepWord[517];j++)
			{
				temp2 = a-j*temp1;
				if(temp2 >= 0)
					K_A1_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / cos(temp2) ;
				else
					K_A1_index[j]=g_InteralMemory.KeepWord[519];
			}*/
		
		
		
			
			g_InteralMemory.KeepWord[520] = Duanchang1 * Encoder_Pulse / PI / Gunzhijing ;////�γ�1������������
			g_InteralMemory.KeepWord[521] = Duanchang2 * Encoder_Pulse / PI / Gunzhijing ;////�γ�2������������
			
	
			g_InteralMemory.KeepWord[522] = 10000 * g_InteralMemory.KeepWord[518] / g_InteralMemory.KeepWord[519];////����µ����̱�����������
			
			g_InteralMemory.KeepWord[523] = Pianxinju_1 * sin(a) * Encoder_Pulse / PI / Gunzhijing * 1000 / g_InteralMemory.KeepWord[260] ;////��ڹ��̱�����������
			
			g_InteralMemory.KeepWord[524] = g_InteralMemory.KeepWord[523] + 10000 * (g_InteralMemory.KeepWord[44]-g_InteralMemory.KeepWord[517]) / g_InteralMemory.KeepWord[519];////���̧�����̱�����������
			
			
			tmp = g_InteralMemory.KeepWord[522] + g_InteralMemory.KeepWord[523] + g_InteralMemory.KeepWord[524];
			
			g_InteralMemory.KeepWord[525] = g_InteralMemory.KeepWord[521] - tmp ;///��1�γ����ɺ�ĵȴ�������
			
			
			
		/////
		///������ݴ������
		if(g_InteralMemory.KeepBit[200] == 0)
			g_InteralMemory.Word[PING_CTRL] = 26;////���ݼ�����ɺ��Զ����ز�Ʒ�������ý���
		else
			g_InteralMemory.Word[PING_CTRL] = 43;
	}
	if(g_InteralMemory.Bit[87] == 1)
	{
		InPulse1_Stop();//�л���Ʒǰ����ͣ�������� ������ȫ��������ٿ�����������
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		g_InteralMemory.Bit[87]=0;
		
		g_InteralMemory.Bit[8] = 0;
		g_InteralMemory.Bit[9] = 0;
		g_InteralMemory.Bit[10] = 0;
		g_InteralMemory.Bit[30] = 0;
		g_InteralMemory.Bit[31] = 0;
		g_InteralMemory.Bit[32] = 0;
		g_InteralMemory.Bit[38] = 0;
		g_InteralMemory.Bit[42] = 0;
		g_InteralMemory.Bit[43] = 0;
		////////
		//g_InteralMemory.Bit[82] = 1;
		g_InteralMemory.KeepWord[10] = 0;
		g_InteralMemory.KeepBit[13] = 12;
		
		g_InteralMemory.Bit[38] =0;
		g_InteralMemory.Bit[39] =0;
		g_InteralMemory.Bit[40] =0;
		g_InteralMemory.Bit[41] =0;
		g_InteralMemory.Bit[80] =0;
		g_InteralMemory.Bit[81] =0;
		g_InteralMemory.Bit[82] =0;
		
		g_InteralMemory.Bit[86] =0;
		g_InteralMemory.Bit[88] =1;
		
		g_InteralMemory.Bit[90] =0;
		g_InteralMemory.Bit[91] =0;
		g_InteralMemory.Bit[92] =1;
		////��������
		 Pianxinju_2 = (double)g_InteralMemory.KeepWord[0] / 10.0 ;/////���ݴ���ʼ�Ƚ�������ݻ�ԭ��ʵ��ֵ
		   Houdu  = (double)g_InteralMemory.KeepWord[507] ;
	    
		/**************Bug�޸� by����ΰ*********************/   
		if(Houdu >= Pianxinju_2)
		   Houdu = Pianxinju_2 - 1.0; 
		/**************Bug�޸� by����ΰ*********************/ 
		
		i = (Pianxinju_2 - Houdu) / Pianxinju_2 ;
		a=acos(i);//�жϹ��̵�ת���ĽǶ�
		
		
		temp1 = a / g_InteralMemory.KeepWord[509];
		for(j=0;j<g_InteralMemory.KeepWord[509];j++)
		{
			temp2 = a-j*temp1;
			if(temp2 >= 0.0)
				K_A2_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_2 / cos(temp2) ;
			else
				K_A2_index[j]=g_InteralMemory.KeepWord[511];
		}
			
		
			
		Pianxinju_1 = (double)g_InteralMemory.KeepWord[41] / 10.0 ;
			
	//	Daojianju   = (double)g_InteralMemory.KeepWord[42] / 10.0 ;
			/**************Bug�޸� by����ΰ*********************/    
			if(Houdu >= Pianxinju_1)   
			Houdu = Pianxinju_1 - 1.0;    
			/**************Bug�޸� by����ΰ*********************/ 

		i = (Pianxinju_1 - Houdu) / Pianxinju_1 ;
		a=acos(i);//��ڹ��̵�ת���ĽǶ�
		
		temp1 = a / g_InteralMemory.KeepWord[517];
		for(j=0;j<=g_InteralMemory.KeepWord[517];j++)
		{
			temp2 = a-j*temp1;
			if(temp2 >= 0)
				K_A1_index[j]=10000 * onecircle_pulse * Gunzhijing / 5000.0 / Pianxinju_1 / cos(temp2) * g_InteralMemory.KeepWord[260] / 1000 ;
			else
				K_A1_index[j]=g_InteralMemory.KeepWord[519];
		}
		///////////////////////////////////////
	//////��Ʒ����
	////���������K_A2,dK2,Inpulse_xiadao2,Inpulse_qieduan2,Inpulse_taidao2,Inpulse_dengdai2,Outpulse_xiadao2,Outpulse_qieduan2,Outpulse_taidao2
		K_A2 = g_InteralMemory.KeepWord[511];
		
		Inpulse_xiadao2 = g_InteralMemory.KeepWord[513];
		Inpulse_qieduan2 = g_InteralMemory.KeepWord[514];
		Inpulse_taidao2 = g_InteralMemory.KeepWord[515];
	
		Inpulse_dengdai2[0] = g_InteralMemory.KeepWord[516];
		Inpulse_dengdai2[1] = g_InteralMemory.KeepWord[516];
		
		Outpulse_xiadao2 = g_InteralMemory.KeepWord[510];
		Outpulse_qieduan2 = g_InteralMemory.KeepWord[509];
		Outpulse_taidao2 = g_InteralMemory.KeepWord[4];
		
		Dingchang_on_off = 1;
	
	//////////////////////
	////������
	// K_A1,dK1,Inpulse_xiadao1,Inpulse_qieduan1,Inpulse_taidao1,Outpulse_xiadao1,Outpulse_qieduan1,Outpulse_taidao1;////������ȫ�ֱ���
	// Inpulse_dengdai1[13],Inpulse_all1[13];
		
		Inpulse_xiadao1 = g_InteralMemory.KeepWord[522];
		Inpulse_qieduan1 = g_InteralMemory.KeepWord[523];
		Inpulse_taidao1 = g_InteralMemory.KeepWord[524];
		Outpulse_xiadao1 =  g_InteralMemory.KeepWord[518];
		Outpulse_qieduan1 =  g_InteralMemory.KeepWord[517];
		Outpulse_taidao1 =  g_InteralMemory.KeepWord[44];
		
		K_A1 = g_InteralMemory.KeepWord[519];
	
	
		Chongkoushu =  (g_InteralMemory.KeepWord[505]-g_InteralMemory.KeepWord[501]) /  g_InteralMemory.KeepWord[503] +1;
		
		Inpulse_dengdai1[0] =g_InteralMemory.KeepWord[520]  + g_InteralMemory.KeepWord[512]-g_InteralMemory.KeepWord[521] * (Chongkoushu-1)-g_InteralMemory.KeepWord[520]-Inpulse_xiadao1-Inpulse_qieduan1-Inpulse_taidao1;
		
		for(j=1;j<=Chongkoushu;j++)
			Inpulse_dengdai1[j] = g_InteralMemory.KeepWord[525];
		
		
		Chongkou_on_off = 1;
		chongkou_last_length = g_InteralMemory.KeepWord[512]-g_InteralMemory.KeepWord[521] * (Chongkoushu-1)-g_InteralMemory.KeepWord[520] ;
	////////////////////////////////////////////
		g_InteralMemory.Word[501]=0;
		g_InteralMemory.Word[502]=0;
		g_InteralMemory.Word[503]=0;
		g_InteralMemory.Word[504]=0;
		g_InteralMemory.Word[505]=0;
		g_InteralMemory.Word[506]=0;
		g_InteralMemory.Word[507]=0;
		g_InteralMemory.Word[508]=0;
		g_InteralMemory.Word[509]=0;////��������������
		g_InteralMemory.Word[511]=0;
		InPulseCount[1] = 0;
		InPulseCount[3] = 0;
		
		chongkou_current = 0;
		dingchang_current = 0;
		current_qieduan = 0;
		need_qieduan = 0;
		pulseout_qieduan1 = 0;
		pulseout_qieduan2 = 0;
		qieduan_enable = 0;
		cut_all = 0;
		
		for(j=0; j<10; j++)
			Enable_qieduan[j]=0;
			
		g_InteralMemory.Word[514]= g_InteralMemory.KeepWord[505];/////������������ʾ�����ͳ����
		g_InteralMemory.Word[515]=Chongkoushu;
			
		/////////////////////////////////////////////////////
		g_InteralMemory.KeepBit[10] = 0;
		g_InteralMemory.KeepBit[12] = 0;
		
		if(g_InteralMemory.Bit[27]==1)///ϵͳ����
			InPulse1_Start();
	
	}
	
	

	
}



