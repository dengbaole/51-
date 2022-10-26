#include "timer.h"
#include <REGX52.H>
#include "uart.h"


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
    TF0 = 0;
    TR0 = 1;           //定时器开始计时

    //中断配置
    ET0 = 1;
    EA = 1 ;
    PT0=0;
}

//被串口占用
void timer1_init(void)
{
    

}



void timer2_init(void)
{
    

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

    //串口定时器处理数据
    if(start_timer == 1)              //需要在主循环中关闭定时
    {
        recv_timer_cnt++;
        if(recv_timer_cnt>MAX_REV_TIME)
        {
            recv_timer_cnt = 0;        //清除定时器 处理数据 清除buffer(用完之后再清除)
            //recv_cnt = 0;
            recv_flag = 1;             //说明数据已经接收完成，在主循环中关闭
        }
    }

    TR0 =1;
}