#include <REGX52.H>
#include "delay.h"
#include "task.h"
#include "nixie.h"
#include "timer.h"
#include "uart.h"

void main()
{
	char ch = 1;
	taskinit();
	timer0_init();
	uart_init();   //波特率4800 8位无校验
    UART_SendByte(0x66);
	while(1)
	{
		fsmTask(ch);
	}
}