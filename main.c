#include <REGX52.H>
#include "delay.h"
#include "task.h"
#include "nixie.h"
#include "timer.h"
#include "uart.h"
#include "matrixkey.h"
//#include "log.h"



void main()
{
	unsigned char button;
	char ch = 1;
	unsigned char KeyNum;
	task_init();
	timer0_init();
	uart_init();   //波特率4800 8位无校验
	//printf("51_code version1.0.0\n");
	//EMlog(LOG_DEBUG,"HELLO\n");

	while(1)
	{
		//矩阵按键
		
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
			P2_0 = 0;
			Delay1ms(200);
			P2_0 = 1;
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