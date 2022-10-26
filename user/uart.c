#include "uart.h"
#include <REGX52.H>


unsigned char recv_buf[MAX_REV_NUM];
unsigned char recv_cnt = 0;
unsigned char recv_length;
unsigned char start_timer=0;
unsigned char recv_timer_cnt;
unsigned char recv_flag = 0;

void uart_init(void)   //4800bps@12.000MHz
{
    SCON = 0x50;        //接收使能
    PCON |= 0x80;		//使能波特率倍速位SMOD


    //定时器配置（用作波特率发生器）
    TMOD = TMOD & 0x0F;   //低四位清零，高四位不变
    TMOD = TMOD | 0x20;   //定时器模式配置
    //赋初值和波特率有关
    TL1 = 0xF3;		//设定定时初值
	TH1 = 0xF3;		//设定定时器重装值

    //中断配置
    ET1 = 0;       //禁用定时器1中断
    TR1 = 1;       //启动定时器1

    EA = 1;
    ES = 1;
}

//发送1字节数据
void UART_SendByte(unsigned char Byte)
{
    SBUF=Byte;    //把数据写到缓冲器中，自动发送给电脑
    while(TI==0);
    TI=0;         //标志位需要软件复位
}

//打印字符串
void UART_SendString(unsigned char *buf)
{
    while (*buf != '\0')
    {
        UART_SendByte(*buf++);
    }
}

//printf重定向
char putchar(char c)
{
    UART_SendByte(c);
    return c;
}

void clr_recvbuffer(unsigned char *buf)
{
    unsigned char i;
    for(i=0;i<MAX_REV_NUM;i++)
    {
        buf[i]=0;
    }
}

//接收数据处理函数
void uart_service(unsigned char *buf)
{
    unsigned char recv_move_indev;
    if(recv_flag)
    {
        recv_flag = 0;
        start_timer = 0;
      
        //接收的数据要大于5个字节(包头，地址，数据类型，数据，校验)，
        while((recv_cnt>=5)&&(recv_move_indev<=recv_cnt))
        {
            //判断包头地址数据类型
            if((buf[recv_move_indev+0]==0x55)&&(buf[recv_move_indev+1]==0xAA)&&(buf[recv_move_indev+2]==0x55))
            {
                if((buf[recv_move_indev+3]==0x01)&&(buf[recv_move_indev+4]==0x02))
                {
                    printf("11111111111111111111\n");     //接收到数据{0x55 0xaa 0x55 0x01 0x02}亮灯
                    P2_1 = 0;
                }
                if((buf[recv_move_indev+3]==0x02)&&(buf[recv_move_indev+4]==0x03))
                {
                    printf("2222222222222222222\n");      //接收到数据{0x55 0xaa 0x55 0x02 0x03}灭灯
                    P2_1 = 1;
                }
                break;   //查找成功并执行判断后退出循环
            }
            recv_move_indev++;
        }
        UART_SendString(recv_buf);    //打印接收到的数据
        clr_recvbuffer(recv_buf);     //清除缓冲buff
        recv_cnt = 0;
    }
}




//中断函数名称自定义，函数后面加中断号
void UART_ISR() interrupt 4
{
    
    //判断是否是接收中断
    if(RI)
    {
        RI = 0;
        start_timer=1;                      //启动计数器
        //判断是否是结束符
        if(recv_cnt<MAX_REV_NUM)
        {
            recv_buf[recv_cnt++] = SBUF;   //接收到的数据放大缓冲区
        }
        else
        {
            recv_cnt=MAX_REV_NUM;
        }
        recv_timer_cnt = 0;               //接收完一帧数据后，定时器计数清零
    }
    // //发送数据中断
    // if(TI)
    // {
    //     TI=0;
    // }
}