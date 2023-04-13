/*************************************************************************
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
* All rights reserved. All use of this software and documentation is     *
* subject to the License Agreement located at the end of this file below.*
**************************************************************************
* Description:                                                           *
* The following is a simple hello world program running MicroC/OS-II.The * 
* purpose of the design is to be a very simple application that just     *
* demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
* for issues such as checking system call return codes. etc.             *
*                                                                        *
* Requirements:                                                          *
*   -Supported Example Hardware Platforms                                *
*     Standard                                                           *
*     Full Featured                                                      *
*     Low Cost                                                           *
*   -Supported Development Boards                                        *
*     Nios II Development Board, Stratix II Edition                      *
*     Nios Development Board, Stratix Professional Edition               *
*     Nios Development Board, Stratix Edition                            *
*     Nios Development Board, Cyclone Edition                            *
*   -System Library Settings                                             *
*     RTOS Type - MicroC/OS-II                                           *
*     Periodic System Timer                                              *
*   -Know Issues                                                         *
*     If this design is run on the ISS, terminal output will take several*
*     minutes per iteration.                                             *
**************************************************************************/


#include <stdio.h>
#include <string.h>
#include "includes.h"

#define  TASK_STK_SIZE                 512       /* Size of each task's stacks (# of WORDs)            */
#define  N_TASKS                        3       /* Number of identical tasks        */
#define _PRINT_BUF  8
#define _PREEMPT 1
#define _COMPLETE 2
#define  _TIMEOUT 3


#define COMP 0
#define PERIOD 1
#define _TT 3
#define _TO 2
#define _FROM 1
#define _INFO 0

OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
OS_STK        TaskStartStk[TASK_STK_SIZE];/* Parameters to pass to each task               */
OS_EVENT     *RandomSem;


//RM task_data[N_TASKS];
INT8U err;
int PRINT = 0;
INT32U task_data[][2]={
		{ 1, 3 },
		{ 3, 6 },
		{ 4, 9 }
};


INT32U msg[4];
extern char pb[25][45];
extern void set_buff_size( INT32U init );
extern INT32U get_buff_size();
extern INT8U get_timeout_flag();
extern INT32U get_buff_idx();
INT8U idx;
INT8U tmp;
void print_buffer(void) {
	if( PRINT == 0 ){
		printf("Time\tInformation\t\tfrom\tto\n");
		printf("----------------------------------------------------------\n");
	}
    if( get_buff_idx()<= _PRINT_BUF ) tmp = get_buff_idx();
    else tmp = _PRINT_BUF;
    //printf("get_buff_idx():%d\n",get_buff_idx());
    for( idx =0; idx<tmp; idx ++){
        //char **pb = get_print_buff();
        //printf("a: %s, l: %d\n", pb[idx + PRINT*_PRINT_BUF],strlen(pb[idx + PRINT*_PRINT_BUF]));

        sscanf( pb[idx + PRINT*_PRINT_BUF], "%d %d %d %d ",&msg[_INFO], &msg[_FROM], &msg[_TO], &msg[_TT]);

        if( msg[_INFO] == _TIMEOUT ){
            printf( "time:%d\t",msg[_TT]);
            printf( "task %d exceed deadline\n", msg[_FROM]);
        }
        else{
            printf( "%d\t",msg[_TT]);
            if( msg[_INFO] == _COMPLETE )
                printf( "COMPLETE\t\t%d\t%d\n", msg[_FROM], msg[_TO]);
            else if( msg[_INFO] == _PREEMPT )
                printf( "PREEMPT\t\t%d\t%d\n", msg[_FROM], msg[_TO]);
        }
        //OSTimeDlyHMSM(0, 0, 1, 0);
    }
    if( get_timeout_flag()==1 ) PRINT = 3;
    PRINT += 1;
}

void Task(void *pdata)
{
    int start ; //the start time
    int end ; //the end time
    int toDelay;
    //INT32U C;
    INT32U *p = (INT32U *)pdata;
    OSTCBCur->compTime = p[COMP];
    //printf("\n&which: %d cc %d", OSPrioCur,((RM *)pdata)->c);
    OSTCBCur->period = p[PERIOD];
    //printf("\n&which: %d pp %d", OSPrioCur,((RM *)pdata)->p);
    //printf("OSTCBCur->compTime: %d, OSTCBCur->period: %d\n", OSTCBCur->compTime, OSTCBCur->period);
    start=OSTimeGet();
    while(1)
    {
        //OS_ENTER_CRITICAL();
    	//printf("OSTCBCur->OSTCBPrio: %d\n", OSTCBCur->OSTCBPrio);
    	//OSTimeDly(1);
    	//OSTimeDlyHMSM(0, 0, 3, 0);

        while(OSTCBCur->compTime>0) //C ticks
        {
            // do nothing
        	//printf("AA: %d\n",OSTCBCur->compTime);
        }
        end = OSTimeGet() ; // end time
        toDelay = p[PERIOD]-(end-start) ;//period == (end-start)
        start = start + p[PERIOD]; // next start time
        OSTCBCur->compTime = p[COMP];//C ;// reset the counter (c ticks for computation)
        OSTCBCur->period = OSTCBCur->period + p[PERIOD];
        //printf("AA: %d\n",get_buff_idx());
        //printf("get_buff_idx: %d, get_buff_idx: %d\n",get_buff_idx(),get_buff_size() );
    	//OSTimeDlyHMSM(0, 0, 3, 0);
        if( get_buff_idx() == get_buff_size() || get_timeout_flag( ) ){
			if( PRINT < 3 ){
				print_buffer();
			}
		}
        OSTimeDly (toDelay);// delay and wait (P-C) times

    }
}
static void TaskStartCreateTasks(void)
{

    //CHANGE
    OSTaskCreate(Task, (void *)task_data[0], &TaskStk[0][TASK_STK_SIZE - 1], 1);
    OSTaskCreate(Task, (void *)task_data[1], &TaskStk[1][TASK_STK_SIZE - 1], 2);
    OSTaskCreate(Task, (void *)task_data[2], &TaskStk[2][TASK_STK_SIZE - 1], 3);
}


int  main (void)
{
    //PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK);      /* Clear the screen                         */

    //OSInit();                                              /* Initialize uC/OS-II                      */
    //OSStatInit();                                          /* Initialize uC/OS-II's statistics         */

    TaskStartCreateTasks();
    //OSCtxSwCtr = 0;                                    /* Clear context switch counter             */
    OSTimeSet(0);
    set_buff_size( 0 );

    //OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);
    OSStart();                                             /* Start multitasking                       */
    return 0;
}


// �p��utilization if  sigma(c/p) <= U(n) = n(2^(1/n)-1), scheduable; otherwise not sure

// ��response time ?

// ostickisr = �t�Ϊ�
