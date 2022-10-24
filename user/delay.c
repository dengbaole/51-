#include "delay.h"

void Delay1ms(unsigned int xms)		//@12.000MHz
{
	unsigned char i, j;

	while(xms)
	{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);
		xms--;          //×¢ÒâÐèÒª°Ñxms¼õÉÙ
	}

}