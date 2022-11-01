#include "beep.h"
//钢琴按键频率装载值

typedef enum
{
    P = 0,
    L1,
    L1_,
    L2,
    L2_,
    L3,
    L4,
    L4_,
    L5,
    L5_,
    L6,
    L6_,
    L7,
    M1,
    M1_,
    M2,
    M2_,
    M3,
    M4,
    M4_,
    M5,
    M5_,
    M6,
    M6_,
    M7,
    H1,
    H1_,
    H2,
    H2_,
    H3,
    H4,
    H4_,
    H5,
    H5_,
    H6,
    H6_,
    H7
}_Music_index;



unsigned int FreqTable[] = {0,
                            63620,63731,63835,63928,64016,64103,64185,64260,64331,64400,64463,64524,
                            64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65029,
                            65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283};
unsigned char FreqSelect;
//小星星
unsigned char Music_xiaoxingxing[] = {  13,4,
                                        13,4,
                                        20,4,
                                        20,4,
                                        22,4,
                                        22,4,
                                        20,4+4,
                                        18,4,
                                        18,4,
                                        17,4,
                                        17,4,
                                        15,4,
                                        15,4,
                                        13,4+4,
                                        
                                        0xFF  //终止符
                                        };

void play_music(unsigned char *Music)
{
    	if(Music[MusicSelect]!=0xFF)	//如果不是停止标志位
		{
			FreqSelect=Music[MusicSelect];	//选择音符对应的频率(在中断中的频率)
			MusicSelect++;
			Delay(SPEED/4*Music[MusicSelect]);	//选择音符对应的时值（响应时间）
			MusicSelect++;
			TR0=0;
			Delay(5);	//音符间短暂停顿
			TR0=1;
		}
		else	//如果是停止标志位
		{
			TR0=0;
			while(1);
		}
}