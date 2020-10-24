/**********************************************************
* @ File name -> led.h
* @ Version   -> V1.0
* @ Date      -> 10-31-2013
* @ Brief     -> LED控制相关函数头文件

 V1.*
* @ Revise    ->
**********************************************************/

#ifndef _LED_h_
#define _LED_h_

#include "sys.h"

/**********************************************************
                         定义接口
**********************************************************/

#define LED1					PBout(2)
#define LED2					PAout(15)

/**********************************************************
                       外部功能函数                      
**********************************************************/

void LED_Init(void);	//初始化LED接口

#endif
