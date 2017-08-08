#include <STC15W408AS.h>					//8.7���ԣ����ڽ��ճ��ַ��������ж�
#include <intrins.h>
#include<stdio.h>
unsigned char message;
char s1[]="Swadia";
char s2[]="Rhodok";
char u1[]="Knight";
char u2[]="Pikeman";
char u3[]="Error!";
char rec[30];
int length=0;
void init_serial()    					//��ʼ������
{  				                     //��ʱ��T2ʹ�ù�����ʽ1��������9600��������վ���11.0592
	SCON=0X50;                   //8λ���ݣ��ɱ䲨����
	AUXR |= 0x01;		           //����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR|=0X04;                //��ʱ��2ʱ��ΪFosc����1T
	T2L=0xE0;     					 //���ö�ʱ������  110592��9600
	T2H=0xfE;               //���ö�ʱ������  110592��9600
	AUXR|=0X10;            //������ʱ��2
	TI=1;
	EA=1;
	IE=0; 	//������ж
}
void delayMS(unsigned int ms)   //@11.0592MHz   //��ʱ1ms
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

void SendOneByte(unsigned char dat)								//���͵����ֽں���
{           
		SBUF=dat;    //��������
		while(!TI);   //�ȴ��������
		TI=0;         //�������.
}
void Receive()
{
	if(RI==1)    						//����Ƿ������ݽ���
	{
		message=SBUF;     			//��������
		rec[length]=message;         //�洢����
		RI=0;							//�����־λ
		length=length+1;
		
	}
}
  
int match(unsigned char strrec[],unsigned char strmatch[])   //�ж��ַ�������
{
	int i=0;
	for(i=0;i<=5;i++)
	{
		if(strrec[i]!=strmatch[i]) return 0;
	}
	return 1;
}
void Send(unsigned char str[])  //�����ַ�������
{
      int i=1;
	do
	{
		SendOneByte(str[i]);
		i++;
	}while(str[i]!='\0');
}
void main()
{
	int i;
	init_serial();						//���ڳ�ʼ��							
    while(1)
	{
		    Receive();
			if(message=='#')         //Journal:1:������������delay   2��rec�ַ�������ʹ
			{
				Send(rec);
			if(match(rec,s1))
			{
				Send(u1);
			}
			else if (match(rec,s2))
			{
				Send(u2);
			}
			else
			{
				Send(u3);
			}
			for(i=0;i<length;i++)   //����洢����
			{
				rec[i]='\0';
			}
			length=0;message='\0';
			delayMS(5000);
			}
	}
}