#include "LED.h"

/**********************************************************
* 函数功能 ---> LED接口初始化
* 入口参数 ---> none
* 返回参数 ---> none 
* 功能说明 ---> none
**********************************************************/
//LED0 LED1 PF9 PF10
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	//开启GPIOC外设时钟	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 + GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_SetBits(GPIOF, GPIO_Pin_9);	//输出高电平，熄灭灯
	GPIO_SetBits(GPIOF, GPIO_Pin_10);	//输出高电平，熄灭灯
}
