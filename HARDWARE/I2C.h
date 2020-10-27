/**********************************************************
* @ File name -> I2C.h
* @ Version   -> V1.0
* @ Date      -> 10-31-2013
* @ Brief     -> I2C控制相关函数头文件

 V1.*
* @ Revise    ->
**********************************************************/

#ifndef _I2C_h_
#define _I2C_h_

#include "sys.h"
#include <math.h>
#define uint unsigned int
/**********************************************************
                         定义接口
**********************************************************/

//IO操作函数	 
#define SCL    PCout(11) //SCL
#define SDA    PCout(12) //SDA
#define READ_SDA   PCin(12)  //输入SDA 


#define AD5933_SYS_Init					(1)<<12
#define AD5933_Begin_Fre_Scan		(2)<<12
#define AD5933_Fre_UP						(3)<<12
#define AD5933_Fre_Rep					(4)<<12
#define AD5933_Get_Temp					(9)<<12
#define AD5933_Sleep						(10)<<12
#define AD5933_Standby					(11)<<12

#define AD5933_OUTPUT_2V				(0)<<9
#define AD5933_OUTPUT_1V				(3)<<9
#define AD5933_OUTPUT_400mV			(2)<<9
#define AD5933_OUTPUT_200mV			(1)<<9

#define AD5933_Gain_1						(1)<<8
#define AD5933_Gain_5						(0)<<8

#define AD5933_IN_MCLK					(0)<<3
#define AD5933_OUT_MCLK					(1)<<3

#define AD5933_Reset						(1)<<4

/**********************************************************
                       外部功能函数                      
**********************************************************/
//定义函数
void Init_I2c(void);
void SDA_1(void);
void SDA_0 (void);
void SCL_1(void);
void SCL_0 (void);
void GetACK(void);
void SendNACK(void);
void START(void);
void STOP(void);
void SendByte(u8 txd);	// 发送一个字节数据子函数 
u8 ReadByte(void);  //读一个字节数据
void Write_Byte(char nAddr,uint nValue);
void SetPointer(char nAddr);
int Rece_Byte(char nAddr);
void Delay_ms(unsigned long nValue);
float Scale_Imp (u8 *SValue,u8 *IValue,u8 *NValue,u8 *CValue);
void Scale_Imp_Init (u8 *SValue,u8 *IValue,u8 *NValue,u8 *CValue);
u16 AD5933_Tempter(void);
float Get_resistance(u16 num);
float AD5933_Sweep (float Fre_Begin,float Fre_UP,u16 UP_Num,u16 OUTPUT_Vatage,u16 Gain,u16 SWeep_Rep);


void SDA_IN(void);
void SDA_OUT(void);

#endif
