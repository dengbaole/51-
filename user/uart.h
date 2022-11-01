#ifndef _UART_H
#define _UART_H
#include <stdio.h>

#define MAX_REV_NUM 10       //缓冲区最大数值
#define MAX_REV_TIME  5      //5个毫秒(可以根据波特率修改1000/4800)


extern unsigned char recv_buf[MAX_REV_NUM];
extern unsigned char recv_cnt;
extern unsigned char recv_flag;
extern unsigned int led_data;
extern unsigned int led_cnt;
extern unsigned int beep_data;
extern unsigned int beep_cnt;



void uart_init(void);
void UART_SendByte(unsigned char Byte);
void UART_SendString(unsigned char *buf);
char putchar(char c);
void clr_recvbuffer(unsigned char *buf);
// void uart_service(unsigned char *buf);
#endif 