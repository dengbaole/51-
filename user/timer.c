#include "timer.h"
#include <REGX52.H>
#include "uart.h"
#include "nixie.h"
#include "beep.h"


/****************************************************************************************************
//说明:51单片机定时器使用说明
//接口定义:
//位寻址: 
*****************************************************************************************************/
unsigned int T0Countms;
unsigned int T0Count_100ms;


void timer0_init(void)
{
    //定时器配置
    //TMOD = 0x01;   //0000 0001
    TMOD = TMOD & 0xF0;   //低四位清零，高四位不变
    TMOD = TMOD | 0x01;          
    //赋初值65535-64535=1000us=1ms
    TH0 = 64535/256;   //取出高八位
    TL0 = 64535%256+1; //取出低八位
    TF0 = 0;           //清除TF1标志
    TR0 = 1;           //定时器开始计时

    //中断配置
    ET0 = 1;
    EA = 1 ;
    PT0=0;
}

/**
  * @brief  定时器1中断（被串口占用）
  * @param  无
  * @retval 无
  */
void timer1_init(void)   //1000微秒@12.000MHz
{
	TMOD &= 0x0F;		//设置定时器模式
    TMOD |= 0x10;
	TL1 = 0x20;		//设置定时初值
	TH1 = 0xD1;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时

    //中断配置
    ET1 = 1;
    EA = 1 ;
    PT1=0;
}



void timer2_init(void)  //1微秒@12.000MHz
{
    T2MOD = 0;		    //初始化模式寄存器
	T2CON = 0;		    //初始化控制寄存器
    TL2 = 64535/256;	//设置定时初值
	TH2 = 64535%256;	//设置定时初值
	TR2 = 1;		    //定时器2开始计时

    //中断配置
    ET2 = 1;
    EA = 1 ;
    PT2=0;
}

//定时器0中断函数(每1ms进入一次中断函数)
void Timer0_ISR() interrupt 1
{
    TR0 = 0;   //暂停定时器
    //重新赋初值
    TH0 = 64535/256;     //取出高八位
    TL0 = 64535%256+1;   //取出低八位
    T0Countms++;
    if(T0Countms>=100)
    {
        T0Count_100ms++;
    }


    // //串口定时器处理数据
    // if(start_timer == 1)              //需要在主循环中关闭定时
    // {
    //     recv_timer_cnt++;
    //     if(recv_timer_cnt>MAX_REV_TIME)
    //     {
    //         recv_timer_cnt = 0;        //清除定时器 处理数据 清除buffer(用完之后再清除)
    //         //recv_cnt = 0;
    //         recv_flag = 1;             //说明数据已经接收完成，在主循环中关闭
    //     }
    // }

    TR0 =1;
}


void Timer1_ISR() interrupt 3
{
    //
}


void Timer2_ISR() interrupt 5
{
    TF2 = 0;
    //设置蜂鸣器频率
//   if(FreqTable[FreqSelect])	//如果不是休止符
//	{
//		/*取对应频率值的重装载值到定时器*/
//		TL0 = FreqTable[FreqSelect]%256;		//设置定时初值
//		TH0 = FreqTable[FreqSelect]/256;		//设置定时初值
//		//Buzzer=!Buzzer;	                  //翻转蜂鸣器IO口
//	}
}