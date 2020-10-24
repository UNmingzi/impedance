#include "I2C.h"
#include "delay.h"
#include "impedanceRead.h"
void Init_I2c( void )      //初始化I2C
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//推挽输出
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_11|GPIO_Pin_12); 	//PC10,PC11 输出高  
	
  return;
}
void NOPS(void) 
{
	delay_us(20);
} 
void SDA_1( void )
{
//    SDA_OUT();		//将SDA设置为输出模式
    SDA=1;		//SDA管脚输出为高电平    
    NOPS();
    return;
}

void SDA_0 ( void )
{
//    SDA_OUT();		//将SDA设置为输出模式
    SDA=0;	//SDA管脚输出为低电平    
    NOPS();
    return;
}

void SCL_1( void )
{
     		//将SCL设置为输出模式
    SCL=1;		//SCL管脚输出为高电平    
    NOPS();
    return;
}

void SCL_0 ( void )
{
    		//将SCL设置为输出模式
    SCL=0;	//SCL管脚输出为低电平    
    NOPS();
    return;
}

void GetACK(void)
{   
	int ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	SDA=1;delay_us(Delay_IIC*1);	   
	SCL=1;delay_us(Delay_IIC*1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>500)
		{
			STOP();
			return;
		}
	}
	SCL=0;//时钟输出0 	
}

void SendNACK(void)
{
	SCL=0;
	SDA_OUT();
	SDA=1;
	delay_us(Delay_IIC*2);
	SCL=1;
	delay_us(Delay_IIC*2);
	SCL=0;
}

void START(void)    // 启动数据总线	
{
	SDA_OUT();     //sda线输出
	SDA=1;	  	  
	SCL=1;
	delay_us(Delay_IIC*4);
 	SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(Delay_IIC*4);
	SCL=0;//钳住I2C总线，准备发送或接收数据 
}

void STOP(void)
{
	SDA_OUT();//sda线输出
	SCL=0;
	SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(Delay_IIC*4);
	SCL=1; 
	SDA=1;//发送I2C总线结束信号
	delay_us(Delay_IIC*4);		
}

void SendByte(u8 txd)	// 发送一个字节数据子函数 
{
    u8 t;   
	SDA_OUT(); 	    
    SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(Delay_IIC*2);   //对TEA5767这三个延时都是必须的
		SCL=1;
		delay_us(Delay_IIC*2); 
		SCL=0;	
		delay_us(Delay_IIC*2);
    }	 
}

u8 ReadByte(void)  //读一个字节数据
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        SCL=0; 
        delay_us(Delay_IIC*2);
		SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(Delay_IIC*1); 
    }					 
		SendNACK();
    return receive;
}

void Write_Byte(char nAddr,uint nValue)//nAddr中写入字节nValue
{   
	int nTemp = 0x1A;      // AD5933的默认地址&写控制位（低）
	
  START(); 
	SendByte(nTemp);     // 发送地址	
	GetACK();   
	
	SendByte(nAddr);     // 发送地址字节
	GetACK();
	
	SendByte(nValue);	// 发送数据字节	
	GetACK();
		
	STOP();	// 停止总线			
    	return;
}

void SetPointer(char nAddr)  //   设置地址指针
{          
	int nTemp = 0x1A;      // AD5933的默认地址&写控制位（低）
	
  START(); 
	SendByte(nTemp);     // 发送地址	
	GetACK();     // 等待 ACK		

	SendByte(0xB0);     // 发送指针命令1101 0000
	GetACK();

	SendByte(nAddr);	// 发送地址指针	
	GetACK();	

	STOP();	// 停止总线			
   	return;
}

int Rece_Byte(char nAddr)//读取nAddr中的字节到返回值
{   
	int nTemp ;    
  SetPointer(nAddr);   //地址指针指向nAddr
  nTemp=0x1B;      // AD5933的默认地址&读控制位（高）
  START(); 

	SendByte(nTemp);     // 发送地址	
	GetACK();  

	nTemp=ReadByte();//读一个字节数据		
	SendNACK();//发送NO_ACK
	
	STOP();	// 停止总线			
	return nTemp;
}


u16 AD5933_Tempter(void)
{
 
// unsigned char Status;  //保存状态
 unsigned int Tm;   //保存实部，虚部，温度
 
 
//   //复位AD5933
//   Write_Byte(0x80,0XB1);
//   Write_Byte(0x81,0X00);
    
   //启动温度测量
   Write_Byte(0x80,0x93);
    
   //等待转换完成
//   do
//    {
// 
//     Status=Rece_Byte(0x8F);
//  
//}while(!(Status & 0x01));
   //读出温度，保存在Tm中
   Tm=Rece_Byte(0x92);
   Tm<<=8;
   Tm+=Rece_Byte(0x93);
   Tm<<=2;

		return Tm;
}

//float Scale_imp (uint SValue[3],uint IValue[3],uint NValue[2],uint CValue[2],float ki,int Ps);
float resistance[200];
float rads[200];
int 	AD5933_Dat_Re[200];
int 	AD5933_Dat_Im[200];


void Maopao_Paixu(float *dat, u16 leng)
{
	u16 i,j;
	float buf;
	for(j=0;j<leng-1;j++)
	for(i=0;i<leng-j-1;i++)
	if(dat[i]>dat[i+1]){
	buf=dat[i];
	dat[i]=dat[i+1];
	dat[i+1]=buf;
	}
}

float Get_resistance(u16 num)
{
	u16 i;
	float navle;
	Maopao_Paixu(resistance,num);
	navle=resistance[0];
	for(i=num/2-num/4;i<num/2+num/4;i++)
	{
		navle=(navle+resistance[i])/2;
	}
//	return (double)navle*AD5933_Correction*AD5933_Frequency*AD5933_Freq_Comp_Coefficient;
	return (double)navle*AD5933_Correction;
	
}
void Fre_To_Hex(float fre,u8 *buf)
{
	u32 dat;
	dat=(536870912/(double)(AD5933_MCLK*1000000))*fre;
	buf[0]=dat>>16;
	buf[1]=dat>>8;
	buf[2]=dat;

}
/*Fre_Begin起始频率，Fre_UP频率增量，UP_Num增量数，OUTPUT_Vatage输出电压，Gain增益系数，SWeep_Rep扫频为1重复为0*/

/*
Fre_Begin 		开始频率 （HZ）
Fre_UP				步进频率（HZ）
UP_Num				步进次数
OUTPUT_Vatage	输出电压
								AD5933_OUTPUT_2V
								AD5933_OUTPUT_1V
								AD5933_OUTPUT_400mV
								AD5933_OUTPUT_200mV
								
Gain					PGA增益			
							AD5933_Gain_1
							AD5933_Gain_5
SWeep_Rep			扫描模式
							AD5933_Fre_UP 	递增频率
							AD5933_Fre_Rep	重复频率
*/

//AD5933_Sweep(30000,200,200,AD5933_OUTPUT_2V,AD5933_Gain_1,AD5933_Fre_UP);
//	AD5933_Sweep(10000,1,40,AD5933_OUTPUT_2V,AD5933_Gain_1,AD5933_Fre_UP);

float AD5933_Sweep (float Fre_Begin,float Fre_UP,u16 UP_Num,u16 OUTPUT_Vatage,u16 Gain,u16 SWeep_Rep)
{
	u8 SValue[3], IValue[3], NValue[2], CValue[2];
	u16 buf=0;
	Fre_To_Hex(Fre_Begin,SValue);
	Fre_To_Hex(Fre_UP,IValue);
	NValue[0]=UP_Num>>8;
	NValue[1]=UP_Num;
#ifdef AD5933_MCLK_USE_OUT
	buf=OUTPUT_Vatage|Gain|SWeep_Rep|AD5933_OUT_MCLK;
#else
		buf=OUTPUT_Vatage|Gain|SWeep_Rep|AD5933_IN_MCLK;
#endif
	CValue[0]=buf>>8;
	CValue[1]=buf;
	
	Scale_imp(SValue,IValue,NValue,CValue);
	return 0;
}
/*SValue[3]起始频率，IValue[3]频率增量，NValue[2]增量数，CValue[2]控制字，ki增益系数，Ps扫频为1重复为0*/

float Scale_imp (u8 *SValue,u8 *IValue,u8 *NValue,u8 *CValue)
{
	int i,j,AddrTemp;
	u8 Gain=((~CValue[0])&0x01)?5:1;
	u8 SWeep_Rep=((CValue[0]&0xF0)==(AD5933_Fre_UP>>8))?1:0;
	u8 Mode=CValue[0]&0x0f;
	long ReadTemp,realArr[3],imageArr[3];
                float magnitude;       	
//                uint start_f[3]={0X33,0X26,0X17};
//                uint inc_f[3]={0,0,0X21};
//                uint num_f[2]={0,0XC8};
//                uint control[2]={0XB1,0X00};
//								CValue[0]=Mode|AD5933_Standby;
								j=0;
                Init_I2c();    //初始化I2C
   
                AddrTemp=0X82; //初始化起始频率寄存器 0x82 0x83 0x84
                for(i = 0;i <3;i++)
                {
    	Write_Byte(AddrTemp,SValue[i]);
                AddrTemp++;
                }               
                AddrTemp=0X85; //初始化频率增量寄存器 0x85 0x86 0x87
                for(i = 0;i <3;i++)
                {
    	Write_Byte(AddrTemp,IValue[i]);
                AddrTemp++;
                } 
                AddrTemp=0X88; //初始化频率点数寄存器
                for(i = 0;i <2;i++)
                {
    	Write_Byte(AddrTemp,NValue[i]);
                AddrTemp++;
                } 
    //初始化控制寄存器，1011 0001 0000 0000待机模式，2V，一倍放大，内部时钟                                  
                AddrTemp=0X80; 
//                for(i = 0;i <2;i++)
                {
								Write_Byte(AddrTemp,Mode|(AD5933_Standby>>8));
                AddrTemp++;
								Write_Byte(AddrTemp,CValue[1]);
                AddrTemp++;
                }
                 
                Write_Byte(0x80,Mode|(AD5933_SYS_Init>>8));//控制寄存器写入初始化频率扫描命令
								delay_ms(10);//fix me
                Write_Byte(0X80,Mode|(AD5933_Begin_Fre_Scan>>8));//控制寄存器写入开始频率扫描命令
 while(1)
{
                while(1)
                {
                ReadTemp=Rece_Byte(0x8F);  //读取状态寄存器检查DFT是否完成
//							ReadTemp=ReadTemp&0x07;
                if (ReadTemp&0x02)
                break;
                }                  
                realArr[0]=Rece_Byte(0x94);
                realArr[1]=Rece_Byte(0x95);
                realArr[2]=realArr[0]*0x100+realArr[1];
                
                imageArr[0]=Rece_Byte(0x96);
                imageArr[1]=Rece_Byte(0x97);
                imageArr[2]=imageArr[0]*0x100+imageArr[1];      
								
								rads[j]=atan2(imageArr[2],realArr[2])-0.00143485062;

                
	if (realArr[2]>=0x8000)  //计算实部的原码(除符号位外，取反加一)
	{
                realArr[2]^=0xFFFF; 
                realArr[2]^=0x8000; 
								realArr[2]+=1;
                realArr[2]^=0x8000;
	}
	if (imageArr[2]>=0x8000)  //计算虚部的原码(除符号位外，取反加一)
	{
                imageArr[2]^=0xFFFF; 
                imageArr[2]^=0x8000; 
								imageArr[2]+=1;
                imageArr[2]^=0x8000;
	}
								AD5933_Dat_Re[j]=realArr[2];
								AD5933_Dat_Im[j]=imageArr[2];
                magnitude=sqrt(realArr[2]*realArr[2]+imageArr[2]*imageArr[2]);  //模值计算              
                resistance[j++]=1/(magnitude*Gain);		 //阻抗计算
								
                ReadTemp=Rece_Byte(0x8F);  //读取状态寄存器检查频率扫描是否完成
                if (ReadTemp&0x04)
                break;
                if (SWeep_Rep==1)
                Write_Byte(0X80,CValue[0]);	//控制寄存器写入增加频率（跳到下一个频率点)的命令
								else
								Write_Byte(0X80,CValue[0]);	//控制寄存器写入重复当前频率点扫描	
}
                Write_Byte(0X80,0XA1);	//进入掉电模式 Standby mode 2.0Vp-p typical PGA gain = 1
return magnitude;
}


//GPIO 通用设置
//GPIOx:GPIOA~GPIOI.
//BITx:0X0000~0XFFFF,位设置,每个位代表一个 IO,
//第 0 位代表 Px0,第 1 位代表 Px1,依次类推.比如 0X0101,代表同时设置 Px0 和 Px8.
//MODE:0~3;模式选择,0,输入(系统复位默认状态);1,普通输出;2,复用功能;3,模拟输入.
//OTYPE:0/1;输出类型选择,0,推挽输出;1,开漏输出.
//OSPEED:0~3;输出速度设置,0,2Mhz;1,25Mhz;2,50Mhz;3,100Mh.
//PUPD:0~3:上下拉设置,0,不带上下拉;1,上拉;2,下拉;3,保留.
//注意:在输入模式(普通输入/模拟输入)下,OTYPE 和 OSPEED 参数无效!!

////IO方向设置
////00:输入(复位状态) 00:无上下拉
//#define SDA_IN()  {GPIOC->MODER|=0<<24;GPIOC->PUPDR|=0<<24;}
////01:通用输出 1:输出推挽(复位状态) 10:50MHZ(快速) 00:无上下拉
//#define SDA_OUT() {GPIOC->MODER|=1<<24;GPIOC->OTYPER|=0<<12;GPIOC->MODER|=3<<24;GPIOC->PUPDR|=0<<24;}
void SDA_IN(void)
{
	RCC->AHB1ENR|=1<<2; //使能 PORTC 时钟
	
	GPIOC->MODER&=~(3<<(12*2)); //先清除 PC12 原来的设置
	GPIOC->MODER|=0<<(12*2); //设置 PC12 为输入模式
	
	GPIOC-> PUPDR&=~(3<<(12*2)); //先清除 PC12 原来的设置
	GPIOC-> PUPDR|=1<<(12*2); //设置 PC12 下拉

}

void SDA_OUT(void)
{
	RCC->AHB1ENR|=1<<2; //使能 PORTC 时钟
	
	GPIOC->MODER&=~(3<<(12*2)); //先清除 PC12 原来的设置
	GPIOC->MODER|=1<<(12*2); //设置 PC12 为输出模式
	
	GPIOC->OTYPER&=~(1<<12) ; //清除 PC12 原来的设置
	GPIOC->OTYPER|=0<<12; //设置 PC12 为推挽输出
	
	GPIOC-> OSPEEDR&=~(3<<(12*2)); //先清除 PC12 原来的设置
	GPIOC-> OSPEEDR|=3<<(12*2); //设置 PC12 输出速度为 50Mhz
	
	GPIOC-> PUPDR&=~(3<<(12*2)); //先清除 PC12 原来的设置
	GPIOC-> PUPDR|=0<<(12*1); //设置 PC12 不带上下拉
	
	GPIOC->ODR|=1<<12; //设置 PC12 输出 1（高电平）
}
