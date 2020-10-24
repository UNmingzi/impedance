/**********************************************************
* @ File name -> impedanceRead.h
* @ Version   -> V1.0
* @ Date      -> 10-31-2013
* @ Brief     -> LED控制相关函数头文件

 V1.*
* @ Revise    ->
**********************************************************/

#ifndef _impedanceRead_h_
#define _impedanceRead_h_

#include "sys.h"

/**********************************************************
                         定义接口
**********************************************************/
#define Delay_IIC 1
#define AD5933 1
#define AD5933_MCLK 16.776  //=536870912/MCLK;
//#define	AD5933_MCLK_USE_OUT	1	//0内部时钟  1外部时钟
#define AD5933_Correction 101615461.47044108
#define AD5933_Frequency 100000
#define AD5933_Freq_Comp_Coefficient 1


/**********************************************************
                       外部功能函数                      
**********************************************************/
void Get_Rs(u32 Key_Value, u8* Task_ID);
void Get_C(u32 Key_Value, u8* Task_ID);
void Get_L(u32 Key_Value, u8* Task_ID);
float DA5933_Get_Rs(void);
float DA5933_Get_C(void);
float DA5933_Get_L(void);

#endif
