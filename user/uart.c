#include "uart.h"
#include <REGX52.H>

unsigned char code recv_correct[] = {0x55,0xAA,0x80,0x0,0x80,0x80};
unsigned char code sum_check_error[] = {0x55,0xAA,0x81,0x0,0x81,0x81};
unsigned char code xor_check_error[] = {0x55,0xAA,0x82,0x0,0x82,0x82};
unsigned char recv_buf[MAX_REV_NUM];
unsigned char recv_cnt = 0;
unsigned char recv_flag = 0;
unsigned char machine_stap = 0;
unsigned int led_data;
unsigned int led_cnt;
unsigned int beep_data;
unsigned int beep_cnt;
unsigned char recv_length;

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

// //接收数据处理函数
// void uart_service(unsigned char *buf)
// {
//     unsigned char recv_move_indev;
//     if(recv_flag)
//     {
//         recv_flag = 0;
//         start_timer = 0;
      
//         //接收的数据要大于5个字节(包头，地址，数据类型，数据，校验)，
//         while((recv_cnt>=5)&&(recv_move_indev<=recv_cnt))
//         {
//             //判断包头地址数据类型
//             if((buf[recv_move_indev+0]==0x55)&&(buf[recv_move_indev+1]==0xAA)&&(buf[recv_move_indev+2]==0x55))
//             {
//                 if((buf[recv_move_indev+3]==0x01)&&(buf[recv_move_indev+4]==0x02))
//                 {
//                     printf("11111111111111111111\n");     //接收到数据{0x55 0xaa 0x55 0x01 0x02}亮灯
//                     P2_1 = 0;
//                 }
//                 if((buf[recv_move_indev+3]==0x02)&&(buf[recv_move_indev+4]==0x03))
//                 {
//                     printf("2222222222222222222\n");      //接收到数据{0x55 0xaa 0x55 0x02 0x03}灭灯
//                     P2_1 = 1;
//                 }
//                 break;   //查找成功并执行判断后退出循环
//             }
//             recv_move_indev++;
//         }
//         UART_SendString(recv_buf);    //打印接收到的数据
//         clr_recvbuffer(recv_buf);     //清除缓冲buff
//         recv_cnt = 0;
//     }
// }




//中断函数名称自定义，函数后面加中断号
void UART_ISR() interrupt 4
{
    unsigned char i;
    unsigned char recv_data;   //数据缓冲区
    static unsigned char sum_check;   //校验和
    static unsigned char xor_check;   //or校验
    //判断是否是接收中断
    if(RI)
    {
        RI = 0;
        recv_data = SBUF;
        switch(machine_stap)
        {
            //判断包头是否是 0x55 0xAA
            case 0: 
                if(recv_data==0x55)
                {
                    machine_stap = 1;
                }   
                else
                {
                    machine_stap = 0;
                }
                break;
            case 1: 
                if(recv_data == 0xAA)
                {
                    machine_stap = 2;
                    recv_cnt = 0;
                }
                else
                {
                    machine_stap = 0;
                }
                break;
            case 2:
                //校验和（地址开始计算）
                sum_check = recv_data;
                //xor校验
                xor_check = recv_data;
                recv_buf[recv_cnt] = recv_data;   //数据类型
                recv_cnt++;
                machine_stap = 3;
                break;
            case 3:
                //校验和（地址开始计算）
                sum_check += recv_data;
                //xor校验
                xor_check ^= recv_data;
                recv_length = recv_data;
                machine_stap = 4;
                break;
            case 4:
                //校验和（地址开始计算）
                sum_check += recv_data;
                //xor校验
                xor_check ^= recv_data;
                recv_buf[recv_cnt] = recv_data;   //具体数据
                if(recv_cnt==recv_length)
                {
                    machine_stap = 5;
                }
                else
                {
                    machine_stap = 4;
                }
                recv_cnt++;
                break;
            case 5:      
                //和校验是否正确
                if(sum_check == recv_data)
                {
                    machine_stap = 6;
                }
                else
                {
                    recv_flag = 1;
                    machine_stap = 0;
                    for(i = 0;i<6;i++)
                    {
                        UART_SendByte(sum_check_error[i]);
                    }
                }
                break;
            case 6:
                //xor校验正确
                if(xor_check == recv_data)
                {
                    recv_flag = 1;  //接收到一帧正确数据标志
                    for(i = 0;i<6;i++)
                    {
                        UART_SendByte(recv_correct[i]);
                    }
                }
                else
                {
                    recv_flag = 1;
                    for(i = 0;i<6;i++)
                    {
                        UART_SendByte(xor_check_error[i]);
                    }

                    //错误应答
                }
                machine_stap = 0;
                recv_cnt = 0;
                sum_check = 0;
                xor_check = 0;
             
                break;
            default:
                break;
        }
    }
    // //发送数据中断
    // if(TI)
    // {
    //     TI=0;
    // }
}