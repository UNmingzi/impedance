#include "I2C.h"
#include "delay.h"
#include "impedanceRead.h"
void Init_I2c( void )      //��ʼ��I2C
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//�������
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_11|GPIO_Pin_12); 	//PC10,PC11 �����  
	
  return;
}
void NOPS(void) 
{
	delay_us(20);
} 
void SDA_1( void )
{
//    SDA_OUT();		//��SDA����Ϊ���ģʽ
    SDA=1;		//SDA�ܽ����Ϊ�ߵ�ƽ    
    NOPS();
    return;
}

void SDA_0 ( void )
{
//    SDA_OUT();		//��SDA����Ϊ���ģʽ
    SDA=0;	//SDA�ܽ����Ϊ�͵�ƽ    
    NOPS();
    return;
}

void SCL_1( void )
{
     		//��SCL����Ϊ���ģʽ
    SCL=1;		//SCL�ܽ����Ϊ�ߵ�ƽ    
    NOPS();
    return;
}

void SCL_0 ( void )
{
    		//��SCL����Ϊ���ģʽ
    SCL=0;	//SCL�ܽ����Ϊ�͵�ƽ    
    NOPS();
    return;
}

void GetACK(void)
{   
	int ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	SCL=0;//ʱ�����0 	
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

void START(void)    // ������������	
{
	SDA_OUT();     //sda�����
	SDA=1;	  	  
	SCL=1;
	delay_us(Delay_IIC*4);
 	SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(Delay_IIC*4);
	SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}

void STOP(void)
{
	SDA_OUT();//sda�����
	SCL=0;
	SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(Delay_IIC*4);
	SCL=1; 
	SDA=1;//����I2C���߽����ź�
	delay_us(Delay_IIC*4);		
}

void SendByte(u8 txd)	// ����һ���ֽ������Ӻ��� 
{
    u8 t;   
	SDA_OUT(); 	    
    SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(Delay_IIC*2);   //��TEA5767��������ʱ���Ǳ����
		SCL=1;
		delay_us(Delay_IIC*2); 
		SCL=0;	
		delay_us(Delay_IIC*2);
    }	 
}

u8 ReadByte(void)  //��һ���ֽ�����
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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

void Write_Byte(char nAddr,uint nValue)//nAddr��д���ֽ�nValue
{   
	int nTemp = 0x1A;      // AD5933��Ĭ�ϵ�ַ&д����λ���ͣ�
	
  START(); 
	SendByte(nTemp);     // ���͵�ַ	
	GetACK();   
	
	SendByte(nAddr);     // ���͵�ַ�ֽ�
	GetACK();
	
	SendByte(nValue);	// ���������ֽ�	
	GetACK();
		
	STOP();	// ֹͣ����			
    	return;
}

void SetPointer(char nAddr)  //   ���õ�ַָ��
{          
	int nTemp = 0x1A;      // AD5933��Ĭ�ϵ�ַ&д����λ���ͣ�
	
  START(); 
	SendByte(nTemp);     // ���͵�ַ	
	GetACK();     // �ȴ� ACK		

	SendByte(0xB0);     // ����ָ������1101 0000
	GetACK();

	SendByte(nAddr);	// ���͵�ַָ��	
	GetACK();	

	STOP();	// ֹͣ����			
   	return;
}

int Rece_Byte(char nAddr)//��ȡnAddr�е��ֽڵ�����ֵ
{   
	int nTemp ;    
  SetPointer(nAddr);   //��ַָ��ָ��nAddr
  nTemp=0x1B;      // AD5933��Ĭ�ϵ�ַ&������λ���ߣ�
  START(); 

	SendByte(nTemp);     // ���͵�ַ	
	GetACK();  

	nTemp=ReadByte();//��һ���ֽ�����		
	SendNACK();//����NO_ACK
	
	STOP();	// ֹͣ����			
	return nTemp;
}


u16 AD5933_Tempter(void)
{
 
// unsigned char Status;  //����״̬
 unsigned int Tm;   //����ʵ�����鲿���¶�
 
 
//   //��λAD5933
//   Write_Byte(0x80,0XB1);
//   Write_Byte(0x81,0X00);
    
   //�����¶Ȳ���
   Write_Byte(0x80,0x93);
    
   //�ȴ�ת�����
//   do
//    {
// 
//     Status=Rece_Byte(0x8F);
//  
//}while(!(Status & 0x01));
   //�����¶ȣ�������Tm��
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
/*Fre_Begin��ʼƵ�ʣ�Fre_UPƵ��������UP_Num��������OUTPUT_Vatage�����ѹ��Gain����ϵ����SWeep_RepɨƵΪ1�ظ�Ϊ0*/

/*
Fre_Begin 		��ʼƵ�� ��HZ��
Fre_UP				����Ƶ�ʣ�HZ��
UP_Num				��������
OUTPUT_Vatage	�����ѹ
								AD5933_OUTPUT_2V
								AD5933_OUTPUT_1V
								AD5933_OUTPUT_400mV
								AD5933_OUTPUT_200mV
								
Gain					PGA����			
							AD5933_Gain_1
							AD5933_Gain_5
SWeep_Rep			ɨ��ģʽ
							AD5933_Fre_UP 	����Ƶ��
							AD5933_Fre_Rep	�ظ�Ƶ��
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
/*SValue[3]��ʼƵ�ʣ�IValue[3]Ƶ��������NValue[2]��������CValue[2]�����֣�ki����ϵ����PsɨƵΪ1�ظ�Ϊ0*/

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
                Init_I2c();    //��ʼ��I2C
   
                AddrTemp=0X82; //��ʼ����ʼƵ�ʼĴ��� 0x82 0x83 0x84
                for(i = 0;i <3;i++)
                {
    	Write_Byte(AddrTemp,SValue[i]);
                AddrTemp++;
                }               
                AddrTemp=0X85; //��ʼ��Ƶ�������Ĵ��� 0x85 0x86 0x87
                for(i = 0;i <3;i++)
                {
    	Write_Byte(AddrTemp,IValue[i]);
                AddrTemp++;
                } 
                AddrTemp=0X88; //��ʼ��Ƶ�ʵ����Ĵ���
                for(i = 0;i <2;i++)
                {
    	Write_Byte(AddrTemp,NValue[i]);
                AddrTemp++;
                } 
    //��ʼ�����ƼĴ�����1011 0001 0000 0000����ģʽ��2V��һ���Ŵ��ڲ�ʱ��                                  
                AddrTemp=0X80; 
//                for(i = 0;i <2;i++)
                {
								Write_Byte(AddrTemp,Mode|(AD5933_Standby>>8));
                AddrTemp++;
								Write_Byte(AddrTemp,CValue[1]);
                AddrTemp++;
                }
                 
                Write_Byte(0x80,Mode|(AD5933_SYS_Init>>8));//���ƼĴ���д���ʼ��Ƶ��ɨ������
								delay_ms(10);//fix me
                Write_Byte(0X80,Mode|(AD5933_Begin_Fre_Scan>>8));//���ƼĴ���д�뿪ʼƵ��ɨ������
 while(1)
{
                while(1)
                {
                ReadTemp=Rece_Byte(0x8F);  //��ȡ״̬�Ĵ������DFT�Ƿ����
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

                
	if (realArr[2]>=0x8000)  //����ʵ����ԭ��(������λ�⣬ȡ����һ)
	{
                realArr[2]^=0xFFFF; 
                realArr[2]^=0x8000; 
								realArr[2]+=1;
                realArr[2]^=0x8000;
	}
	if (imageArr[2]>=0x8000)  //�����鲿��ԭ��(������λ�⣬ȡ����һ)
	{
                imageArr[2]^=0xFFFF; 
                imageArr[2]^=0x8000; 
								imageArr[2]+=1;
                imageArr[2]^=0x8000;
	}
								AD5933_Dat_Re[j]=realArr[2];
								AD5933_Dat_Im[j]=imageArr[2];
                magnitude=sqrt(realArr[2]*realArr[2]+imageArr[2]*imageArr[2]);  //ģֵ����              
                resistance[j++]=1/(magnitude*Gain);		 //�迹����
								
                ReadTemp=Rece_Byte(0x8F);  //��ȡ״̬�Ĵ������Ƶ��ɨ���Ƿ����
                if (ReadTemp&0x04)
                break;
                if (SWeep_Rep==1)
                Write_Byte(0X80,CValue[0]);	//���ƼĴ���д������Ƶ�ʣ�������һ��Ƶ�ʵ�)������
								else
								Write_Byte(0X80,CValue[0]);	//���ƼĴ���д���ظ���ǰƵ�ʵ�ɨ��	
}
                Write_Byte(0X80,0XA1);	//�������ģʽ Standby mode 2.0Vp-p typical PGA gain = 1
return magnitude;
}


//GPIO ͨ������
//GPIOx:GPIOA~GPIOI.
//BITx:0X0000~0XFFFF,λ����,ÿ��λ����һ�� IO,
//�� 0 λ���� Px0,�� 1 λ���� Px1,��������.���� 0X0101,����ͬʱ���� Px0 �� Px8.
//MODE:0~3;ģʽѡ��,0,����(ϵͳ��λĬ��״̬);1,��ͨ���;2,���ù���;3,ģ������.
//OTYPE:0/1;�������ѡ��,0,�������;1,��©���.
//OSPEED:0~3;����ٶ�����,0,2Mhz;1,25Mhz;2,50Mhz;3,100Mh.
//PUPD:0~3:����������,0,����������;1,����;2,����;3,����.
//ע��:������ģʽ(��ͨ����/ģ������)��,OTYPE �� OSPEED ������Ч!!

////IO��������
////00:����(��λ״̬) 00:��������
//#define SDA_IN()  {GPIOC->MODER|=0<<24;GPIOC->PUPDR|=0<<24;}
////01:ͨ����� 1:�������(��λ״̬) 10:50MHZ(����) 00:��������
//#define SDA_OUT() {GPIOC->MODER|=1<<24;GPIOC->OTYPER|=0<<12;GPIOC->MODER|=3<<24;GPIOC->PUPDR|=0<<24;}
void SDA_IN(void)
{
	RCC->AHB1ENR|=1<<2; //ʹ�� PORTC ʱ��
	
	GPIOC->MODER&=~(3<<(12*2)); //����� PC12 ԭ��������
	GPIOC->MODER|=0<<(12*2); //���� PC12 Ϊ����ģʽ
	
	GPIOC-> PUPDR&=~(3<<(12*2)); //����� PC12 ԭ��������
	GPIOC-> PUPDR|=1<<(12*2); //���� PC12 ����

}

void SDA_OUT(void)
{
	RCC->AHB1ENR|=1<<2; //ʹ�� PORTC ʱ��
	
	GPIOC->MODER&=~(3<<(12*2)); //����� PC12 ԭ��������
	GPIOC->MODER|=1<<(12*2); //���� PC12 Ϊ���ģʽ
	
	GPIOC->OTYPER&=~(1<<12) ; //��� PC12 ԭ��������
	GPIOC->OTYPER|=0<<12; //���� PC12 Ϊ�������
	
	GPIOC-> OSPEEDR&=~(3<<(12*2)); //����� PC12 ԭ��������
	GPIOC-> OSPEEDR|=3<<(12*2); //���� PC12 ����ٶ�Ϊ 50Mhz
	
	GPIOC-> PUPDR&=~(3<<(12*2)); //����� PC12 ԭ��������
	GPIOC-> PUPDR|=0<<(12*1); //���� PC12 ����������
	
	GPIOC->ODR|=1<<12; //���� PC12 ��� 1���ߵ�ƽ��
}
