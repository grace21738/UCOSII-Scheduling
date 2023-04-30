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
#define BUFFER_SIZE 15
#define _PRINT_BUF  8
#define _PRINT_TOL 2
#define _PREEMPT 1
#define _COMPLETE 2
#define  _TIMEOUT 3


#define COMP 0
#define PERIOD 1
#define _TT 0
#define _INFO 1
#define _TO 2
#define _FROM 3

OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
OS_STK        TaskStartStk[TASK_STK_SIZE];/* Parameters to pass to each task               */
OS_EVENT     *RandomSem;


//RM task_data[N_TASKS];
INT8U err;
int PRINT = 0;
INT8U task_data[][2]={
		//{ 1, 3 },
        //{ 3, 5 }
         { 1, 4 },
		 { 2, 5 },
		 { 2, 10 }
};


int pb[BUFFER_SIZE][4];
extern void set_buff_size( INT8U init );
extern INT8U get_buff_size();
extern INT8U get_timeout_flag();
extern INT8U get_buff_idx();
INT8U i_cnt;
INT8U tmp;
INT8U t_cnt;
void print_buffer(void) {

	if( PRINT == 0 ){
		printf("Time\tInformation\t\tfrom\tto\n");
		printf("----------------------------------------------------------\n");
	}
	if( get_buff_idx() - (PRINT*_PRINT_BUF) < _PRINT_BUF ) tmp = get_buff_idx()%_PRINT_BUF;
	else tmp = _PRINT_BUF;
    //printf("get_buff_idx():%d\n",get_buff_idx());
    for( i_cnt =0; i_cnt<tmp; i_cnt ++){
    	t_cnt = i_cnt + PRINT*_PRINT_BUF;
        //char **pb = get_print_buff();
        //printf("a: %s, l: %d\n", pb[i_cnt + PRINT*_PRINT_BUF],strlen(pb[i_cnt + PRINT*_PRINT_BUF]));

        //sscanf( pb[i_cnt + PRINT*_PRINT_BUF], "%u %u %u %u ",&msg[_INFO], &msg[_FROM], &msg[_TO], &msg[_TT]);
        //printf("MM: %u %u %u %u\n",msg[_INFO], msg[_FROM], msg[_TO], msg[_TT]);

        if( pb[t_cnt][_INFO] == _TIMEOUT ){
            printf( "time:%d\t",pb[t_cnt][_TT]);
            printf( "task %d exceed deadline\n", pb[t_cnt][_FROM]);
        }
        else{
            printf( "%u\t",pb[t_cnt][_TT]);
            if( pb[t_cnt][_INFO] == _COMPLETE )
                printf( "COMPLETE\t\t%u\t%u\n", pb[t_cnt][_FROM], pb[t_cnt][_TO]);
            else if( pb[t_cnt][_INFO] == _PREEMPT )
                printf( "PREEMPT\t\t%u\t%u\n", pb[t_cnt][_FROM], pb[t_cnt][_TO]);
        }
        //OSTimeDlyHMSM(0, 0, 1, 0);
    }
    if( get_timeout_flag()==1 ) PRINT = _PRINT_TOL;
    PRINT += 1;
}

void Task(void *pdata)
{
    int start ; //the start time
    int end ; //the end time
    int toDelay;
    //INT32U C;
    INT8U *p = (INT8U *)pdata;
    //OSTCBCur->compTime = p[COMP];
    //printf("\n&which: %d cc %d", OSPrioCur,((RM *)pdata)->c);
    //OSTCBCur->period = p[PERIOD];
    //printf("\n&which: %d pp %d", OSPrioCur,((RM *)pdata)->p);
    //printf("OSTCBCur->compTime: %d, OSTCBCur->period: %d\n", OSTCBCur->compTime, OSTCBCur->period);
    OSTCBCur->ddl = p[PERIOD];
    OSTCBCur->compTime = p[COMP];
    OSTCBCur->fullComp = p[COMP];
    OSTCBCur->period = p[PERIOD];
    start=OSTimeGet();
    while(1)
    {
        //OS_ENTER_CRITICAL();
    	//printf("OSTCBCur->OSTCBPrio: %d\n", OSTCBCur->OSTCBPrio);
    	//OSTimeDly(1);
    	//OSTimeDlyHMSM(0, 0, 3, 0);
		#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
			OS_CPU_SR  cpu_sr = 0;
		#endif

        while(OSTCBCur->compTime>0) //C ticks
        {
            // do nothing
        	//printf("AA: %d\n",OSTCBCur->compTime);
        }

        if(  get_buff_idx() < get_buff_size()){
		     //printf("OSTimeGet: %o\n", OSTimeGet());
		     //printf("Prio:%o , Deadline: %o \n",OSTCBCur->OSTCBPrio, OSTCBCur->ddl);
		 }

		OS_ENTER_CRITICAL();
		end = OSTimeGet() ; // end time
		toDelay = (OSTCBCur->ddl)-end;//period == (end-start)
		toDelay = toDelay < 0 ? 0 : toDelay;
		// if(  get_buff_idx() < get_buff_size() ){
		//     printf("start, end: %d, %d\n", start, end);
		//     }
		//start = start + p[PERIOD]; // next start time
		OSTCBCur->compTime = p[COMP];//C ;// reset the counter (c ticks for computation)
        if( get_buff_idx() == get_buff_size() || get_timeout_flag( ) ){
			if( PRINT < _PRINT_TOL ){
				print_buffer();
			}
		}
        OSTCBCur->ddl += OSTCBCur->period;
        OS_EXIT_CRITICAL();
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
    Init_Task_ddl();
    //OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);
    OSStart();                                             /* Start multitasking                       */
    return 0;
}

OS_TCB *current;
void Init_Task_ddl(void){
    for (i_cnt = 1; i_cnt < OS_LOWEST_PRIO-1 ; i_cnt++)
        {

            if (OSTCBPrioTbl[i_cnt] != (OS_TCB *)0 )
            {
                // printf("BBBBBB, %d\n",cnt);
                current = OSTCBPrioTbl[i_cnt];
                current->compTime = task_data[i_cnt-1][COMP];
                current->fullComp = task_data[i_cnt-1][COMP];
                current->period = task_data[i_cnt-1][PERIOD];
                current->ddl = task_data[i_cnt-1][PERIOD];
            }
        }
}


// 計算utilization if  sigma(c/p) <= U(n) = n(2^(1/n)-1), scheduable; otherwise not sure

// 算response time ?

// ostickisr = 系統的
