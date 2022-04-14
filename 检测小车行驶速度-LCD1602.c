/******************************************************************************************
�����������С����ʹ����-LCD1602
��д�ˣ�Yayi
��̳��rtrobot.org������                                                
/******************************************************************************************/

#include <REG52.H>//ͷ�ļ�
#include <OLED.h>


#define GRATING_NUM 20			//���ù�դ���ӵ�����
unsigned int motor1=0;	 //��������������ֵ
unsigned int speed1=0;	 //��������������ֵ
unsigned int k=0;


/********************************************************************************************
��ʱ��0��ʼ��
/********************************************************************************************/
void T0_init (void)
{
	TMOD = 0x01; 	//��4λ����T1����4λ����T0
	EA = 1;		//�ж��ܿ���
	TH0 = 0; 	//16λ�����Ĵ���T0��8λ
	TL0 = 0;	//16λ�����Ĵ���T0��8λ
	ET0 = 1; 	//T0�жϿ���
	TR0 = 1; 	//T0��������
}

/*********************************************************************************************
�ⲿ�ж�INT0��INT1��ʼ������
/********************************************************************************************/
void INT_init (void)
{
	EA = 1;			//�ж��ܿ���  
	EX0 = 1; 		//�����ⲿ�ж�0�ж�
	IT0 = 1; 		//1�����ش���  0���͵�ƽ����
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
	//��ʼ��
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
	//��ʼ��
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
������
/********************************************************************************************/
void main(void)
{		
	
	OLED_Init();			//��ʼ��OLED  
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
�ⲿ�ж�INT0������1������
/********************************************************************************************/
void intersvr1(void) interrupt 0 using 1
{
	motor1++;		
}

/********************************************************************************************
��ʱ��0�жϺ���
ע�⣺�����speedΪ����д���߼���ʽ����Ӧ��Ϊ
speed2=С�������ܳ�/���̸�ʽ*1��������������
��ʱ��������Ч��Ϊ���һ���ڵľ���
/********************************************************************************************/ 
void T0_CNT (void) interrupt 1  using 2
{
	TH0=(65536-1000)/256;	//16λ�����Ĵ���T0��8λ�������޸�1000������ֵ
	TL0=(65536-1000)%256;	//16λ�����Ĵ���T0��8λ�������޸�1000������ֵ
	k++;			//k�Լ�1
	if(k==1000)		
	{
		k=0;			//���¶���k��ֵ
		speed1=moving_average_filtre1( moving_average_filtre(motor1)*3);			// �ȳ�20(��դ��) *60(�����ǲ��1s)
		motor1=0;	 	//���¶���motor1��ֵ

	}		
}
