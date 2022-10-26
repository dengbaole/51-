#include <REGX52.H>
#include "delay.h"
#include "task.h"
#include "nixie.h"
#include "timer.h"
#include "uart.h"
//#include "log.h"

void main()
{
	char ch = 1;
	task_init();
	timer0_init();
	uart_init();   //波特率4800 8位无校验
	//printf("51_code version1.0.0\n");
	//EMlog(LOG_DEBUG,"HELLO\n");
	P2_3 = 0;
	while(1)
	{
		uart_service(recv_buf);
		fsmTask(ch);
	}
}