//////////////////////////////////////////////////////////////
// StateMachine.h - TSAR light indicator state machine
//
// Author:	<Katie Loseke> (kloseke@pdx.edu) 
// Date:	November 2020
//
// Description:
// ------------
// This is the header for a state machine which will be used in
// a light indicator bar for TSAR.
//
// 
////////////////////////////////////////////////////////////////

//Just relised that one of the valves is a 0 in binary, hard to light 0, should ask about this

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//this is the main for this part that will call all other functions
//Will return 0 if no errors were found
int checkState(int valveCState[], int valveExState[], int valvesInError[]);	//input is a placeholder as I don't know how we get valve info into function

//A valve state didn't match, so now we here
int errorFound(int valvesInError[]);

//Take read in valve states and mak them usuable
void fillStruct(int valveCState[], int valveExState[]);

//Main function for the checking
//It is expected that this is what a higher function calls to initiate check
int mainCheck(int valveCState[], int valveExState[], int batteryState);

//Just realised that I was just copying code in each error level
int isError(int valvesInError[], int errorLevel);

/*
//A big error occured
int bigError(int valvesInError[]);

//A moderate error occured
int someError(int valvesInError[]);

//A small error occured
int smallError(int valvesInError[]);
*/

//Just turn on Buzzer
void buzzOn();

//just turn  off Buzzer
void buzzOff();

//Turn on apecific binary (top is MSB) on LED lights
void ledOn(int whichOn);

//Turn off all LEDs
void ledOff();

//Power failure has occured
void powerFailureState();


