//////////////////////////////////////////////////////////////
// StateMachine.c - TSAR light indicator state machine
//
// Author:	<Katie Loseke> (kloseke@pdx.edu) 
// Date:	November 2020
//
// Description:
// ------------
// This state machine will evaluate the state of the valves on the
// TSAR and will determine if it is in the correct state or not.
// If it is in the wrong state, a signal will be sent to the light bar
// to display the correct error level.
//
////////////////////////////////////////////////////////////////
//I just realised that this is more of a logic block of if than a state machine
//For just setting error levels, we really don't need a true state machine

//Ask about if higher function will loop this in error state, or if we need to loop in error state

#include "StateMachine.h"

				//index - priority
// SOV_F01		//0 - 6
// PV_F01		//1 - 5
// PV_F02		//2 - 3
// PV_L01		//3 - 4
// PV_L02		//4 - 1
// PV_L03		//5 - 2
// PV_P01		//6 - 8
// PV_P02		//7 - 9
// PV_LF01		//8 - 7

int mainCheck(int valveCState[], int valveExState[], int batteryState)
{
	//For tracking error status, only says that we have an error
	int errorButton = 0;
	
	//Keeps trak of which state of error that we are in
	//0 = all good, 1 = minor error, 2 = bigger error, 3 = major error, 4 = power failure
	int errorState = 0;
	
	//array keeps track of which valves are in error
	int valvesInError[10]; //0 = good, 1 = error
	
	//initialising error array
	//10th index is for power
	int i = 0;
	for (i=0; i<9; ++i)
	{
		valvesInError[i] = 0;
	}
	valvesInError[9] = batteryState;
	
	errorButton = checkState(valveCState, valveExState, valvesInError);
		
	
	if(errorButton == 1)
	{
		errorState = errorFound(valvesInError);
		isError(valvesInError, errorState);
	}
	//clear any errors that may have been occuring
	if(errorButton == 0)
	{
		buzzOff();
		ledOff();
		errorState = 0;
	}
	
	return errorState;
}

//returns error level (0-4)
int checkState(int valveCState[], int valveExState[], int valvesInError[])
{
	//If 1, then an error has occured
	int errorButton = 0;
	
	//checking each valve for an error
	if(valveCState[0] != valveExState[0])
	{
		errorButton = 1;
		valvesInError[0] = 1;
	}
	if(valveCState[1] != valveExState[1])
	{
		errorButton = 1;
		valvesInError[1] = 1;
	}
	if(valveCState[2] != valveExState[2])
	{
		errorButton = 1;
		valvesInError[2] = 1;
	}
	if(valveCState[3] != valveExState[3])
	{
		errorButton = 1;
		valvesInError[3] = 1;
	}
	if(valveCState[4] != valveExState[4])
	{
		errorButton = 1;
		valvesInError[4] = 1;
	}
	if(valveCState[5] != valveExState[5])
	{
		errorButton = 1;
		valvesInError[5] = 1;
	}
	if(valveCState[6] != valveExState[6])
	{
		errorButton = 1;
		valvesInError[6] = 1;
	}
	if(valveCState[7] != valveExState[7])
	{
		errorButton = 1;
		valvesInError[7] = 1;
	}
	if(valveCState[8] != valveExState[8])
	{
		errorButton = 1;
		valvesInError[8] = 1;
	}
	if(valvesInError[9] == 1)
	{
		errorButton = 1;
	}
	
	return errorButton;
}

//when in doubt, error out
//returns a value depending on which level of error is found
int errorFound(int valvesInError[])
{
	int errorLevel = 0;
				//index - priority
// PV_L02		//4 - 1
// PV_L03		//5 - 2
// PV_F02		//2 - 3
// PV_L01		//3 - 4
// PV_F01		//1 - 5
// SOV_F01		//0 - 6
// PV_LF01		//8 - 7
// PV_P01		//6 - 8
// PV_P02		//7 - 9
//Error level is set to highest prio valve
	if(valvesInError[7])
		errorLevel = 1;
	if(valvesInError[6])
		errorLevel = 1;
	if(valvesInError[8])
		errorLevel = 1;
	if(valvesInError[0])
		errorLevel = 1;
	if(valvesInError[1])
		errorLevel = 2;
	if(valvesInError[3])
		errorLevel = 2;
	if(valvesInError[2])
		errorLevel = 2;
	if(valvesInError[5])
		errorLevel = 3;
	if(valvesInError[4])
		errorLevel = 3;
	if(valvesInError[9]) //power failure on battery, nothing else matters
		errorLevel = 4;
	
	return errorLevel;
}

int isError(int valvesInError[], int errorLevel)
{
	//The buzzer doesn't get turned off until error is cleared
	buzzOn();
	
	if (errorLevel == 4)
	{
		powerFailureState();  //It is currently not expected that we can return from this state without a restart
		return 1;
	}
	
  int i = 0;
	for(i=0; i<6; ++i)
	{
		ledOn(errorLevel);
		sleep(1);
		ledOff();
		sleep(1);
	}
	
	for(i=0; i<9; ++i)
	{
		if(valvesInError[i] == 1)
		{
			ledOn(i);
			sleep(1);
			ledOff();
		}
	}
	LedOn(errorLevel);
	return 0;
}

/*
int bigError(int valvesInError[])
{
	int turnOn;
	//turn on buzzer
	//turn on LEDs for error level
	//Flash for 5 seconds, .75s on, .25s off
	
	//Go through which valves are in error, hold LED on for 1s
	for(i=0; i<9; ++i)
	{
		if(valvesInError[i] = 1;)
		{
			
}

int someError(int valvesInError[])
{
}

int smallError(int valvesInError[])
{
	
}
*/

void buzzOn()
{
	//printf for debugging outside of board
	//will comment out in board testing
	printf("Buzzer ON\n");
}

void buzzOff()
{
	//printf for debugging outside of board
	//will comment out in board testing
	printf("Buzzer OFF\n");
	
}

void ledOn(int whichOn)
{
	//6,7,8 all have same light error
	if((whichOn == 7) || (whichOn == 8)) 
		whichOn = 6;
	++whichOn; //change from index to actual number displayed in binary on LEDs
	
	//add a check for bottom 3 prio as they all get same code
	//printf for debugging outside of board
	//will comment out in board testing
	printf("LED %d ON\n", whichOn);
	if(whichOn==7)
	{
		//turn on LED3
		//turn on LED2
		//turn on LED1
	}
	if(whichOn==6)
	{
		//turn on LED3
		//turn on LED2
	}
	if(whichOn==5)
	{
		//turn on LED3
		//turn on LED1
	}
	if(whichOn==4)
	{
		//turn on LED3
	}
	if(whichOn==3)
	{
		//turn on LED2
		//turn on LED1
	}
	if(whichOn==2)
	{
		//turn on LED2
	}
	if(whichOn==1)
	{
		//turn on LED1
	}
}

void ledOff()
{
	//printf for debugging outside of board
	//will comment out in board testing
	printf("LEDs OFF\n");
	
}

//goes down 3-2-1 continually
void powerFailureState()
{
	while(1)
	{
		ledOn(2);
		sleep(1);
		ledOff();
		
		ledOn(1);
		sleep(1);
		ledOff();
		
		ledOn(0);
		sleep(1);
		ledOff();
	}
}

//SM part of algorithm
//Got rid of state hex and replaced it with array for simplification
/*
Compare actual state hex to expected state hex
        If state matches:
Set current state to be everything good
Assert LEDs for state
Go back to waiting
        If state does not match: 
(optional check: check previous state to see if machine did not change states)
Check valves one by one (high priority to low priority) to see which valve(s) is in the wrong state
Depending on which valve(s) did not change, set current state to error level
Assert LEDs for error level (flashing, 3-5 seconds)
Switch LEDs to show which values are in error (higher priority first, 1 second each)
After this LED cycle, check each faulty valves to see if anything has been fixed (or just took too long to change)
Check if any valves are still in error
If so...
Reset error state
Loop back through error protocol for state and LEDs
        Else…
Set state back to all good
Assert LEDs
Go back to waiting for signal
*/

//Valve Priority levels (from LFETS meeting notes, is inacurate)
/*
1. PV-L02
2. PV-L03
3. PV-F02
4. PV-L01
5. PV-F01
6. SOV-F01
7. PV-LF01
8. PV-P01
9. PV-P02

*/
