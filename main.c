#include <REGX52.H>
#include "delay.h"
#include "task.h"
#include "nixie.h"

void main()
{
	char ch = 1;
	taskinit();
	while(1)
	{
		fsmTask(ch);
	}
}