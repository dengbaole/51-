#include <REGX52.H>
#include "delay.h"
#include "task.h"
#include "nixie.h"
#include "timer.h"
#include "uart.h"
#include "matrixkey.h"
#include "beep.h"
#include "AT24C02.h"
#include "DS18B20.h"
//#include "log.h"


unsigned char KeyNum;
unsigned char button;
unsigned char number;

void main()
{
	
	char ch = 1;

	task_init();
	timer0_init();
	//timer2_init();
	uart_init();   //波特率4800 8位无校验
	//printf("51_code version1.0.0\n");
	//EMlog(LOG_DEBUG,"HELLO\n");


    //播放音乐
//	while(1)
//	{
//		if(Music[MusicSelect]!=0xFF)	//如果不是停止标志位
//		{
//			FreqSelect=Music[MusicSelect];	//选择音符对应的频率(在中断中的频率)
//			MusicSelect++;
//			Delay(SPEED/4*Music[MusicSelect]);	//选择音符对应的时值（响应时间）
//			MusicSelect++;
//			TR0=0;
//			Delay(5);	//音符间短暂停顿
//			TR0=1;
//		}
//		else	//如果是停止标志位
//		{
//			TR0=0;
//			while(1);
//		}
//	}
	
	while(1)
	{
		// //矩阵按键(和蜂鸣器冲突)
		KeyNum = MatrixKey();
		if(KeyNum)
		{
			printf("Keynum = %d",KeyNum);
			Static_Nixie(8,KeyNum);
		}
		
		
		
		
		//独立按键
		button = key_state();
		if(button)
		{
			//Static_Nixie(1,button);
			Buzzer_Time(100);
			//测试AT24C02是否正常
			if(button == 1)
			{
				DS18B20_ConvertT();   //温度转换
				DS18B20_ReadT();      //读取温度
				AT24C02_WriteByte(1,5);
			}
				
			if(button == 2)
			{
				number = AT24C02_ReadByte(1);
				printf("%c",number);
			}
		}
		

		//接收并处理完数据就清除[55 ,AA,]
		if(recv_flag)
		{
			recv_flag = 0;
			//处理数据(判断不同数据长度进行不同的处理)
			switch (recv_buf[0])
			{
				case 1:

					break;
				case 2:
					break;
				case 3:
				default:
					break;
			}
			UART_SendString(recv_buf);
			clr_recvbuffer(recv_buf);    //清空数据
		}
		fsmTask(ch);
	}
}