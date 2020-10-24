#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"

#include "string.h"	//内存操作相关函数库
#include "math.h"	//数学运算相关函数库

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
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置中断分组
	delay_init(168);	//初始化延时函数
	TIM_Init(TIM3, 999, 83,1,1);//定时器初始化
	LED_Init();	//初始化LED接口
	key_init();	//按键初始化
	Init_I2c(); //初始化I2C
	UART4_Init(115200);

	while(1)
	{
//		Get_C(0,0);
		Get_Rs(0,0);
		
	}
}
