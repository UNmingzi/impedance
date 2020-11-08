#include "impedanceRead.h"
#include "I2C.h"
#include "delay.h"
#include "KEY.h"
#include "stdio.h"
#include "usart.h"
/**********************************************************
* 函数功能 ---> 
* 入口参数 ---> none
* 返回参数 ---> none 
* 功能说明 ---> none
**********************************************************/
u8 Firt_In = 1;
u8 Task_Index = 0;
u8 _return=0;
extern char str[30];
extern float rads[200];
extern int 	AD5933_Dat_Re[200];
extern int 	AD5933_Dat_Im[200];
extern int time_ms;


void Get_Rs(u32 Key_Value, u8* Task_ID)
{
	float Rs;
	
	Rs=DA5933_Get_Rs();

	USART_OUT(UART4,(uint8_t *)"T:\t%d\t",time_ms);

	if(Rs>1000000)
	{
		sprintf(str,"%03.05f",Rs/1000000);
		USART_OUT(UART4,(uint8_t *)"%s\tMohm\r\n",str);

	}else	if(Rs>1000)
	{
		sprintf(str,"%03.05f",Rs/1000);
		USART_OUT(UART4,(uint8_t *)"%s\tKohm\r\n",str);
	}else if(Rs<1000)
	{
		sprintf(str,"%03.05f",Rs);
		USART_OUT(UART4,(uint8_t *)"%s\tohm\r\n",str);
	}
}
void Get_C(u32 Key_Value, u8* Task_ID)
{
	float Cap;

	Cap=DA5933_Get_C();
	
	if(Cap>1000000)
	{
		sprintf(str,"%03.05f",Cap/1000000);
		USART_OUT(UART4,(uint8_t *)"%s\tuF\r\n",str);
	}else	if(Cap>1000)
	{
		sprintf(str,"%03.05f",Cap/1000);
		USART_OUT(UART4,(uint8_t *)"%s\tnF\r\n",str);
	}else if(Cap<1000)
	{
	  sprintf(str,"%03.05f",Cap);
		USART_OUT(UART4,(uint8_t *)"%s\tpF\r\n",Cap);
	}
}
void Get_L(u32 Key_Value, u8* Task_ID)
{
	float L;
	
	L=DA5933_Get_L();
	
//	if(L>1000000)
//	{
//		sprintf(str,"%03.05f",L/1000000);
//		USART_OUT(UART4,(uint8_t *)"%s\tmH\r\n",str);
//	}else	if(L>1000)
//	{
//		sprintf(str,"%03.05f",L/1000);
//		USART_OUT(UART4,(uint8_t *)"%s\tuH\r\n",str);
//	}else if(L<1000)
//	{
//		sprintf(str,"%03.05f",L);
//		USART_OUT(UART4,(uint8_t *)"%s\tnH\r\n",L*10);
//	}
}
//读取阻抗值
float DA5933_Get_Rs(void)
{
	float Rs,re,im;
	
//	AD5933_Sweep(30000,200,2,AD5933_OUTPUT_2V,AD5933_Gain_1,AD5933_Fre_Rep);
	AD5933_Sweep((float)AD5933_Frequency,1,1,AD5933_OUTPUT_2V,AD5933_Gain_1,AD5933_Fre_Rep);

//	Rs=Get_resistance(40);
	Rs=Get_resistance(1);
	re=Rs*cos(rads[0]);
	im=Rs*sin(rads[0]);
	return Rs;
}
float DA5933_Dat_Cap(float Fre)
{
	float pp;
//	float dat=3.1415926*2*Fre;
	float dat=1;
	pp=1000000*23.9999992/(AD5933_Dat_Re[0]*dat);
	return pp;
}

float DA5933_Get_C(void)
{	
	float Rs,re,im,cap;
	
//	AD5933_Sweep(30000,200,2,AD5933_OUTPUT_2V,AD5933_Gain_1,AD5933_Fre_Rep);
	AD5933_Sweep(100000,1,20,AD5933_OUTPUT_2V,AD5933_Gain_1,AD5933_Fre_UP);
	
	Rs=Get_resistance(20);
	
	re=Rs*cos(rads[0]);
	im=Rs*sin(rads[0]);
	
	cap=0.9442/(2*3.1415926*im/10000000);
	return cap;
	
}
float DA5933_Get_L(void)
{
	float L;
	float Rs,re,im;
	
	AD5933_Sweep(100000,1,20,AD5933_OUTPUT_2V,AD5933_Gain_1,AD5933_Fre_UP);
	
	Rs=Get_resistance(20);
	
	re=Rs*cos(rads[0]);
	im=Rs*sin(rads[0]);
	
	L=im*1000/(2*3.1415926);	
	return L;
}
