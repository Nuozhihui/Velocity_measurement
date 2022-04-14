/******************************************************************************************
程序名：检查小车行使距离-LCD1602
编写人：Yayi
论坛：rtrobot.org　　　                                                
/******************************************************************************************/

#include <STC12C5A60S2.H>//头文件
#include <LCD1602.h>


unsigned int motor1=0;	 //计左电机码盘脉冲值
unsigned int motor2=0;	 //计右电机码盘脉冲值
unsigned int speed1=0;	 //计左电机码盘脉冲值
unsigned int speed2=0;	 //计右电机码盘脉冲值
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
//	EX1 = 1;
//	IT1	= 1;
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


/*********************************************************************************************
主程序
/********************************************************************************************/
void main(void)
{		
	LCD1602_Init();
	LCD1602_Frist();
	INT_init();
	T0_init();

	while (1)
	{
		print(line_one,0,'M');
		print(line_one,1,'o');
		print(line_one,2,'t');
		print(line_one,3,'o');
		print(line_one,4,'r');
		print(line_one,5,'1');
		print(line_one,6,':');
		print(line_one,7,speed1/1000+0x30);
		print(line_one,8,speed1/100%10+0x30);
		print(line_one,9,speed1/10%10+0x30);
		print(line_one,10,speed1%10+0x30);
		print(line_one,11,'R');
		print(line_one,12,'/');
		print(line_one,13,'M');
		print(line_one,14,'I');
		print(line_one,15,'N');

		print(line_two,0,'M');
		print(line_two,1,'o');
		print(line_two,2,'t');
		print(line_two,3,'o');
		print(line_two,4,'r');
		print(line_two,5,'2');
		print(line_two,6,':');
		print(line_two,7,speed2/1000+0x30);
		print(line_two,8,speed2/100%10+0x30);
		print(line_two,9,speed2/10%10+0x30);
		print(line_two,10,speed2%10+0x30);
		print(line_two,11,'R');
		print(line_two,12,'/');
		print(line_two,13,'M');
		print(line_two,14,'I');
		print(line_two,15,'N');
				DELAY_MS(250);
		LCD1602_WriteCMD(CMD_clear);
		}
}

/*********************************************************************************************
外部中断INT0计算电机1的脉冲
/********************************************************************************************/
void intersvr1(void) interrupt 0 using 1
{
	motor1++;		
}
/*********************************************************************************************
外部中断INT1计算电机2的脉冲
/********************************************************************************************/
void intersvr2(void) interrupt 2 using 3
{
	motor2++;
}



/********************************************************************************************
定时器0中断函数
注意：这里的speed为简单书写，逻辑公式过程应该为
speed2=小车轮子周长/码盘格式*1秒的码盘脉冲格数
定时器做出的效果为算出一秒内的距离
/********************************************************************************************/ 
void T0 (void) interrupt 1  using 2
{
	TH0=(65536-1000)/256;	//16位计数寄存器T0高8位，尝试修改1000成其他值
	TL0=(65536-1000)%256;	//16位计数寄存器T0低8位，尝试修改1000成其他值
	k++;			//k自加1
	if(k==1000)		
	{
		k=0;			//重新定义k的值
		speed1=motor1*3;
		speed2= moving_average_filtre1( moving_average_filtre(motor1)*3);			// 先除20(光栅数) *60(数据是测的1s)
		motor1=0;	 	//重新定义motor1的值
		motor2=0;		//重新定义motor1的值
	}		
}
