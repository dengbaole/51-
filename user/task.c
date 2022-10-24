#include "task.h"
#include "nixie.h"
#include "separate_button.h"


static void MyTask(char ch);

typedef enum
{
	STATE_IDLE, 									//����״̬
	STATE_NIXIE,									//�ϴ�����״̬
	STATE_SECOND, 								//��������״̬
	STATE_ENROLL,									//ע��״̬
	STATE_MATCH,									//ƥ��״̬
	STATE_DELETE,									//ɾ��״̬
	STATE_STANDBY,
	STATE_WRITEFLASH,
	STATE_USBKEY,
	STATE_USER_TEST,
	STATE_ENROLL_NOCHECK_FINGER,
	STATE_MAX
} _FsmState; 



_FsmState get_state;
int get_task_state(void)
{
	return get_state;
}




void fsmTask(char ch)
{
	MyTask(ch);
}
void taskinit(void)
{
	get_state = STATE_IDLE;
}

static void MyTask(char ch)
{
    switch(get_state)
    {
        case STATE_IDLE:
        if(KEY1==0)
		{
			get_state=STATE_NIXIE;
		}
        break;
        case STATE_NIXIE:
		
		Nixie(1,1);
	    Nixie(2,2);
		get_state=STATE_IDLE;
        break;
		case STATE_SECOND:
        default:
        break;
    }

}