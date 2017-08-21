#include <STC15W408AS.h>					//8.21调试，单片机与树莓派，蓝牙交互
#include <intrins.h>
#include<stdio.h>
unsigned char message;
char headclient[]="ANDR";
char headserver[]="RAPI";
char output1[]="Frog";
char output2[]="Naive";
char output3[]="Error";
char rec[50];
int length=0;
void init_serial()    					//初始化串口
{  				                     //定时器T2使用工作方式1，波特率9600，允许接收晶振11.0592
	SCON=0X50;                   //8位数据，可变波特率
	AUXR |= 0x01;		           //串口1选择定时器2为波特率发生器
	AUXR|=0X04;                //定时器2时钟为Fosc，即1T
	T2L=0xE0;     					 //设置定时器处置  110592》9600
	T2H=0xfE;               //设置定时器处置  110592》9600
	AUXR|=0X10;            //启动定时器2
	TI=1;
	EA=1;
	IE=0; 	//屏蔽种卸
}
void delayMS(unsigned int ms)   //@11.0592MHz   //延时1ms
{
	unsigned char y;
	while (ms--)
	{
		for (y = 0; y<100; y++)
		{
			_nop_();
            _nop_();
			_nop_();
			_nop_();
		}
	}
}	

void SendOneByte(unsigned char dat)								//发送单个字节函数
{           
		SBUF=dat;    //发送数据
		while(!TI);   //等待发送完毕
		TI=0;         //发送完毕.
}

void Receive()
{
	if(RI==1)    						//检测是否有数据接收
	{
		message=SBUF;     			//接收数据
		rec[length]=message;         //存储数据
		RI=0;							//清除标志位
		length=length+1;
		
	}
}
  
int match(unsigned char strrec[],unsigned char strmatch[])   //判断字符串函数
{
	int i;
	for(i=0;i<=3;i++)
	{
		if(strrec[i]!=strmatch[i]) return 0;
	}
	return 1;
}
int judge(unsigned char str[])
{
	if(str[4]=='0')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void Send(unsigned char str[])  //发送字符串函数
{
      int i=0;
	do
	{
		SendOneByte(str[i]);
		i++;
	}while(str[i]!='\0');
}
void Sendtoken(unsigned char str[])
{
	int i=4;
	SendOneByte('@');
	do
	{
		SendOneByte(str[i]);
		i++;
	}while(str[i]!='#');
}
void main()
{
	int i;
	init_serial();						//串口初始化
	delayMS(1000);
	Send(output2);
    while(1)
	{
		    Receive();
			if(message=='#')         //Journal:1:必须接收完才能delay   2：rec字符串不好使
			{
			if(match(rec,headclient))  //匹配安卓端
			{
				Sendtoken(rec);      //去头去尾发送token
			}
			else if (match(rec,headserver))   //匹配树莓派
			{
				if(judge(rec))   //判断是否正确
				{
					Send(output1);
				}
			}
			for(i=0;i<length;i++)   //清除存储数据
			{
				rec[i]='\0';
			}
			length=0;message='\0';
			}
	}
}
