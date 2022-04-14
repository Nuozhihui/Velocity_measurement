#ifndef _LCD1602_H_
#define _LCD1602_H_

/********************************************************************************************
引脚定义，扩展板链接引脚号请查阅Bobcats说明
/********************************************************************************************/
#define		LCD1602_DB0_DB7		P0			// 定义LCD1602的数据总线
sbit LCD1602_RS   = P3 ^ 5;					// 定义LCD1602的RS控制线
sbit LCD1602_RW   = P3 ^ 6;					// 定义LCD1602的RW控制线
sbit LCD1602_E    = P3 ^ 4;					// 定义LCD1602的E控制线

/********************************************************************************************
定义LCD1602指令集 （详细请见查看1602指令集手册）
/********************************************************************************************/
#define			CMD_clear		0x01             // 清除屏幕
#define			CMD_back		0x02             // DDRAM回零位
#define			CMD_ac			0x06             // 读入后AC（指针）加1，向右写		
#define			CMD_display		0x0c             // 开显示_关光标_关光标闪烁
#define			CMD_set			0x38             // 8位总线_2行显示
#define			line_one			0x80             // 第一行写入地址
#define			line_two			0xc0             // 第二行写入地址
				   
/********************************************************************************************
LCD1602测忙，若LCD1602处于忙状态，本函数将等待至非忙状态 //
/********************************************************************************************/
void LCD1602_TestBusy(void)
{
   	LCD1602_DB0_DB7 = 0xff;			//设备读状态
   	LCD1602_RS = 0;
   	LCD1602_RW = 1;
   	LCD1602_E = 1;
   	while(LCD1602_DB0_DB7&0x80);		//等待LCD空闲
   	LCD1602_E = 0;				
}

/********************************************************************************************
// 写指令程序 //
/********************************************************************************************/
void LCD1602_WriteCMD(unsigned char LCD1602_command)
{ 
  	LCD1602_TestBusy();
  	LCD1602_DB0_DB7 = LCD1602_command;
  	LCD1602_RS = 0;
  	LCD1602_RW = 0;
  	LCD1602_E = 1;
  	LCD1602_E = 0;
}

/********************************************************************************************
向LCD1602写数据 //
/********************************************************************************************/
void LCD1602_WriteData(unsigned char  LCD1602_data)
{ 
    LCD1602_TestBusy();
	LCD1602_DB0_DB7 = LCD1602_data;
  	LCD1602_RS = 1;
  	LCD1602_RW = 0;
  	LCD1602_E = 1;
  	LCD1602_E = 0;
}

/********************************************************************************************
打印字符程序
向LCD发送一个字符,
line为行数（使用line_one,line_two定义使用），num为位置，data为打印字符
/********************************************************************************************/
void print (unsigned char line,unsigned char num,unsigned char date)
{
		LCD1602_WriteCMD(line+num);
		LCD1602_WriteData(date);
}

/********************************************************************************************
LCD1602初始化 
/********************************************************************************************/
void LCD1602_Init(void)
{
  	LCD1602_WriteCMD(CMD_set);	//  显示模式设置：显示2行，每个字符为5*7个像素
  	LCD1602_WriteCMD(CMD_clear);	//  显示清屏
  	LCD1602_WriteCMD(CMD_back);	//  数据指针指向第1行第1个字符位置
  	LCD1602_WriteCMD(CMD_ac);	//  显示光标移动设置：文字不动，光标右移
  	LCD1602_WriteCMD(CMD_display); 	//  显示开及光标设置：显示开，光标开，闪烁开
}

/********************************************************************************************
延迟函数
/********************************************************************************************/
void DELAY_MS (unsigned int a)
{
	unsigned int i;
	while( a-- != 0)
	{
		for(i = 0; i < 600; i++);
	}
}

void LCD1602_Frist(void)
{
	print(line_one,3,'C');			
	print(line_one,4,'a');		
	print(line_one,5,'r');	
	print(line_one,6,' ');	
	print(line_one,7,'S');	
	print(line_one,8,'p');	
	print(line_one,9,'e');	
	print(line_one,10,'e');	
	print(line_one,11,'d');					
	print(line_two,1,'P');	
	print(line_two,2,'r');	
	print(line_two,3,'o');	
	print(line_two,4,'g');	
	print(line_two,5,'r');	
	print(line_two,6,'a');
	print(line_two,7,'m');
	print(line_two,8,' ');
	print(line_two,9,'s');
	print(line_two,10,'t');
	print(line_two,11,'a');
	print(line_two,12,'r');
	print(line_two,13,'t');	
	DELAY_MS(1000);	
	LCD1602_WriteCMD(CMD_clear);		
}

#endif