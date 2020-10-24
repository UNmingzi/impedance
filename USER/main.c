#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"

#include "string.h"	//�ڴ������غ�����
#include "math.h"	//��ѧ������غ�����

#include "I2C.h"
#include "KEY.h"
#include "LCD.h"
#include "LED.h"
#include "impedanceRead.h"

u8 Task_ID = 1;
char str[30];
extern u32 SysTimer;
extern u32 Keycode;
extern	u8 _return;

int a;
	
int main(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�����жϷ���
	delay_init(168);	//��ʼ����ʱ����
	TIM_Init(TIM3, 999, 83,1,1);//��ʱ����ʼ��
	LED_Init();	//��ʼ��LED�ӿ�
	key_init();	//������ʼ��
	Init_I2c(); //��ʼ��I2C
	UART4_Init(115200);

	while(1)
	{
//		Get_C(0,0);
		Get_Rs(0,0);
		
	}
}
