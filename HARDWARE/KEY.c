#include "KEY.h"



/**
  * @brief  Configures the TIMx's interrupt time.
  * @param  TIMx: where x can be 1-14. 
			@note TIM1 and TIM 9,10,11;TIM2 and TIM12,13,14 have some sharing IRQ handle functions.
  * @param  arr: the period value to be loaded into the active
                 Auto-Reload Register at the next update event, range from 1 to 65535
  * @param  psr: the prescaler value used to divide the TIM clock, range from 1 to 65535
  * @note   1.the TIMx's PreemptionPriority and SubPriority has been predefined in this function,
			change them according to your need.
			2.Tout= ((arr+1)*(psc+1))/Tclk s, 
			  for TIM2-7 and TIM12-14, Tclk=84M
			  for TIM1,8,9,10,11 Tclk=168M
			  exp: for TIM2, if arr=999, psc=83, then Tout=(1000*84)/84M=1ms
  * @retval None
  * @author Calcus Lee
  */
//KEY0 KEY1 KEY2 	PE4 PE3 PE2
void key_init(void)
{
	GPIO_InitTypeDef GPIOStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); //

	GPIOStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIOStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOStructure.GPIO_Pin =GPIO_Pin_2 + GPIO_Pin_3 + GPIO_Pin_4;
	GPIOStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOE, &GPIOStructure);
}
//重启开关检查
void ResetCheck(void)
{
	//RESET_KEYSWITCH连续为1次数
	static uint8_t pTrigerTimes = 0;
	//RESET_KEYSWITCH连续为0次数
	static uint8_t nTrigerTimes = 0;

	if(KEYSWITCH_PD6)
	{
		nTrigerTimes = 0;
		
		pTrigerTimes++;
		pTrigerTimes=pTrigerTimes>100?100:pTrigerTimes;
	}
	else if(!KEYSWITCH_PD6)
	{
		pTrigerTimes = 0;
		
		nTrigerTimes++;
		nTrigerTimes=nTrigerTimes>100?100:nTrigerTimes;
	}

	if(pTrigerTimes > 10)
	{
	}
		
	if(nTrigerTimes > 10)
	{		
	}
}

