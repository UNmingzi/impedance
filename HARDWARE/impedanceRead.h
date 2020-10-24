/**********************************************************
* @ File name -> impedanceRead.h
* @ Version   -> V1.0
* @ Date      -> 10-31-2013
* @ Brief     -> LED������غ���ͷ�ļ�

 V1.*
* @ Revise    ->
**********************************************************/

#ifndef _impedanceRead_h_
#define _impedanceRead_h_

#include "sys.h"

/**********************************************************
                         ����ӿ�
**********************************************************/
#define Delay_IIC 1
#define AD5933 1
#define AD5933_MCLK 16.776  //=536870912/MCLK;
//#define	AD5933_MCLK_USE_OUT	1	//0�ڲ�ʱ��  1�ⲿʱ��
#define AD5933_Correction 101615461.47044108
#define AD5933_Frequency 100000
#define AD5933_Freq_Comp_Coefficient 1


/**********************************************************
                       �ⲿ���ܺ���                      
**********************************************************/
void Get_Rs(u32 Key_Value, u8* Task_ID);
void Get_C(u32 Key_Value, u8* Task_ID);
void Get_L(u32 Key_Value, u8* Task_ID);
float DA5933_Get_Rs(void);
float DA5933_Get_C(void);
float DA5933_Get_L(void);

#endif
