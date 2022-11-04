#ifndef _TIMER_H
#define _TIMER_H
#include <REGX52.H>

extern unsigned int T0Countms;
extern unsigned int T0Count_100ms;
extern unsigned char Compare;
void timer0_init(void);
void timer1_init(void);
void timer2_init(void);



#endif