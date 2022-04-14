/******************************************************************************************
程序名：检查小车行使距离-LCD1602
编写人：Yayi
论坛：rtrobot.org　　　                                                
/******************************************************************************************/

#include <REG52.H>//头文件
#include <OLED.h>


#define GRATING_NUM 20			//配置光栅格子的数量
unsigned int motor1=0;	 //计左电机码盘脉冲值
unsigned int speed1=0;	 //计左电机码盘脉冲值
unsigned int k=0;


/********************************************************************************************
定时器0初始化
/********************************************************************************************/
void T0_init (void)
{
	TMOD = 0x01; 	//高4位控制T1，低4位控制T0
	EA = 1;		//中断总开关
	TH0 = 0; 	//16位计数寄存器T0高8位
	TL0 = 0;	//16位计数寄存器T0低8位
	ET0 = 1; 	//T0中断开关
	TR0 = 1; 	//T0启动开关
}

/*********************************************************************************************
外部中断INT0、INT1初始化函数
/********************************************************************************************/
void INT_init (void)
{
	EA = 1;			//中断总开关  
	EX0 = 1; 		//允许外部中断0中断
	IT0 = 1; 		//1：下沿触发  0：低电平触发
}
#define MVF_LENGTH 5
float moving_average_filtre(float xn)
{
  static int index = -1;
  static float buffer[MVF_LENGTH];
  static float sum = 0;
  float yn = 0;
  int i = 0;
  if(index == -1)
  {
	//初始化
    for(i = 0; i <MVF_LENGTH; i++)
    {
      buffer[i] = xn;
    }
    sum = xn*MVF_LENGTH;
    index = 0;
  }
  else
  {
    sum -= buffer[index];
    buffer[index] = xn;
    sum += xn;
    index++;
    if(index >= MVF_LENGTH)
    {
      index = 0;
    }
  }
  yn = sum/MVF_LENGTH;
  return yn;
}


float moving_average_filtre1(float xn)
{
  static int index = -1;
  static float buffer[MVF_LENGTH];
  static float sum = 0;
  float yn = 0;
  int i = 0;
  if(index == -1)
  {
	//初始化
    for(i = 0; i <MVF_LENGTH; i++)
    {
      buffer[i] = xn;
    }
    sum = xn*MVF_LENGTH;
    index = 0;
  }
  else
  {
    sum -= buffer[index];
    buffer[index] = xn;
    sum += xn;
    index++;
    if(index >= MVF_LENGTH)
    {
      index = 0;
    }
  }
  yn = sum/MVF_LENGTH;
  return yn;
}

int a=123;
/*********************************************************************************************
主程序
/********************************************************************************************/
void main(void)
{		
	
	OLED_Init();			//初始化OLED  
	OLED_Clear()  	;
	
	INT_init();
	T0_init();
	
	OLED_ShowString(5,0,"Measuring Speed",16);  
		OLED_ShowString(85,4,"R/MIN",16);  

	OLED_ShowString(0,4,"Speed:",16); 
	while (1)
	{

	OLED_ShowNum(50,4,speed1,4,16);

	}
}

/*********************************************************************************************
外部中断INT0计算电机1的脉冲
/********************************************************************************************/
void intersvr1(void) interrupt 0 using 1
{
	motor1++;		
}

/********************************************************************************************
定时器0中断函数
注意：这里的speed为简单书写，逻辑公式过程应该为
speed2=小车轮子周长/码盘格式*1秒的码盘脉冲格数
定时器做出的效果为算出一秒内的距离
/********************************************************************************************/ 
void T0_CNT (void) interrupt 1  using 2
{
	TH0=(65536-1000)/256;	//16位计数寄存器T0高8位，尝试修改1000成其他值
	TL0=(65536-1000)%256;	//16位计数寄存器T0低8位，尝试修改1000成其他值
	k++;			//k自加1
	if(k==1000)		
	{
		k=0;			//重新定义k的值
		speed1=moving_average_filtre1( moving_average_filtre(motor1)*3);			// 先除20(光栅数) *60(数据是测的1s)
		motor1=0;	 	//重新定义motor1的值

	}		
}
