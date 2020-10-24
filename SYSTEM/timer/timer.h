#ifndef __timer_h
#define __timer_h

#include "stm32f4xx_tim.h"

void TIM_Init(TIM_TypeDef * TIMx, uint16_t arr, uint16_t psr,uint16_t prepri,uint16_t subpri); 
void TIM3_IRQHandler(void);

#endif
