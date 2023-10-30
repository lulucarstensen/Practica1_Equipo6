/*
 * MyNewTask.c
 *
 *  Created on: Oct 19, 2023
 *      Author: carstensen
 */


#include "MyNewTask.h"

/* Variables and Prototypes */
osaEventId_t mMyEvents;
/* Global Variable to store our TimerID */ tmrTimerID_t myTimerID = gTmrInvalidTimerID_c;
tmrTimerID_t ledTimerID = gTmrInvalidTimerID_c;
   /* Handler ID for task */
osaTaskId_t gMyTaskHandler_ID;
/* Local variable to store the current state of the LEDs */
static uint8_t ledsState = 0;



/* OSA Task Definition */
OSA_TASK_DEFINE(My_Task, gMyTaskPriority_c, 1, gMyTaskStackSize_c, FALSE );


/* Function to init the task */
void MyTask_Init(void)
{
	mMyEvents = OSA_EventCreate(TRUE);
    /* allocate a timer for use in LEDs */
	ledTimerID = TMR_AllocateTimer();

	/* The instance of the MAC is passed at task creaton */
	gMyTaskHandler_ID = OSA_TaskCreate(OSA_TASK(My_Task), NULL);
}

/* Main custom task */
void My_Task(osaTaskParam_t argument)
{
   osaEventFlags_t customEvent;
   myTimerID = TMR_AllocateTimer();
   MyTask_Init(); /* INIT MY NEW TASK */

	while(1)
	{
	OSA_EventWait(mMyEvents, osaEventFlagsAll_c, FALSE, osaWaitForever_c, &customEvent);

		if( !gUseRtos_c && !customEvent)
		{
			break;
		}
          /* Depending on the received event */
		switch(customEvent){

		case gMyNewTaskEvent1_c:
			TMR_StartIntervalTimer(myTimerID, /*myTimerID*/
									1000, /* Timer's Timeout */
									myTaskTimerCallback, /* pointer to myTaskTimerCallback function */
									NULL
			);
		TurnOffLeds(); /* Ensure all LEDs are turned off */

		break;

		case gMyNewTaskEvent2_c: /* Event called from myTaskTimerCallback */

			if(!ledsState) {
                   TurnOnLeds();
                   ledsState = 1;
			}

			else {
					TurnOffLeds();
					ledsState = 0;
			}
			break;

		case gMyNewTaskEvent3_c: /* Event to stop the timer */
			ledsState = 0;
			TurnOffLeds();
			TMR_StopTimer(myTimerID);
			break;

		default:
			break;
			}
	}
}



/* This is the function called by the Timer each time it expires */
static void myTaskTimerCallback(void *param)
{
	OSA_EventSet(mMyEvents, gMyNewTaskEvent2_c);
}

/* Public function to send an event to stop the timer */
void MyTaskTimer_Stop(void)
{
   OSA_EventSet(mMyEvents, gMyNewTaskEvent3_c);
}

void MyTaskTimer_Start(void)
{
	void MyTask_Init(void);
}

//Start Timer for LED//
void LED_StartTimer()
{
    /* start the timer */
    if(!TMR_IsTimerActive(ledTimerID))
    {
        //TMR_StartIntervalTimer(ledTimerID, 2000, (pfTmrCallBack_t)RGB_Timeout, (void*)((uint32_t)ledTimerID));
    }
}

static void newMessage(uint8_t message)
{
	uint8_t count;
    //Check new message is counter
	switch(count)
	{
		case 0:
			Led_RGB(LED_RGB,LED_MAX_RGB_VALUE_c,0,0);
			break;
		case 1:
			Led_RGB(LED_RGB,0,LED_MAX_RGB_VALUE_c,0);
			break;
		case 2:
			Led_RGB(LED_RGB,0,0,LED_MAX_RGB_VALUE_c);
			break;
		case 3:
			Led_RGB(LED_RGB,0,LED_MAX_RGB_VALUE_c,LED_MAX_RGB_VALUE_c);
			break;
		case 4:
			Led_RGB(LED_RGB,LED_MAX_RGB_VALUE_c,0,LED_MAX_RGB_VALUE_c);
			break;
		case 5:
			Led_RGB(LED_RGB,LED_MAX_RGB_VALUE_c,LED_MAX_RGB_VALUE_c,LED_MAX_RGB_VALUE_c);
			break;
		default:
			break;
	}
//Missing printing source address, LQI, payload size

}
