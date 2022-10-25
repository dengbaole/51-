#include "uart.h"
#include <REGX52.H>

void uart_init(void)   //4800bps@12.000MHz
{
    SCON = 0x50;        //接收使能
    PCON |= 0x80;		//使能波特率倍速位SMOD


    //定时器配置（用作波特率发生器）
    TMOD = TMOD & 0x0F;   //低四位清零，高四位不变
    TMOD = TMOD | 0x20;   //定时器模式配置
    TF1 = 0;
    TR1 = 1;
    //赋初值和波特率有关
    TL1 = 0xF3;		//设定定时初值
	TH1 = 0xF3;		//设定定时器重装值

    //中断配置
    ET1 = 1;
    EA = 1 ;
    PT1 = 0;
}


void UART_SendByte(unsigned char Byte)
{
    SBUF=Byte;    //把数据写到缓冲器中，自动发送给电脑
    while(TI==0);
    TI=0;         //标志位需要软件复位
}