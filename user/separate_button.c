#include "separate_button.h"
#include "delay.h"



//获取独立按键键码
unsigned char key_state()
{
    unsigned char KeyNumber = 0;
    if(KEY1==0){Delay1ms(20);while(KEY1==0);Delay1ms(20);KeyNumber=1;}
    if(KEY2==0){Delay1ms(20);while(KEY2==0);Delay1ms(20);KeyNumber=2;}
    if(KEY3==0){Delay1ms(20);while(KEY3==0);Delay1ms(20);KeyNumber=3;}
    if(KEY4==0){Delay1ms(20);while(KEY4==0);Delay1ms(20);KeyNumber=4;}

    return KeyNumber;
}