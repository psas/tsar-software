// main.c
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
// TODO Update this
// Change Log
// 	-Creation of Document 6/12/2020 [APJ]
//
//	This is the main entry point for the TSAR firmware.
//
//	This initializes StateVars as follows:
//  Time Elapsed: 0
//  Time Started: 0
//  isArmed: FALSE
//  ignition: FALSE
//  Last State: FAILURE
//  Current State: SAFETY
//  Address: this
//  Initializing in this way:
//	- Ensures system is disarmed
// 	- guarantees a time-stamp for SAFETY
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#include "main.h"

#include "state_machine.h"
#include "states_def.h"
#include "init.h"


int main(void)
{
	struct StateVars controls = {0,//timeStarted
		0,			//timeElapsed
		0,			//stateCounter
		FALSE,		//isArmed
		0,			//ignition
		TRUE,		//isStateMachineRunning
		0,			//valveConfiguration
		0,			//valveTarget
		FAILURE,	//lastState
		SAFETY, 	//currentState
		&controls	//this address.
		};

	controls.isArmed = FALSE;
	HAL_Init();

	TransmitBuffer_Init();
	SystemClock_Config();
	//MX_WWDG_Init();
	MX_GPIO_Init();
	MX_ADC1_Init();
	MX_LPUART1_UART_Init();


	while(controls.isStateMachineRunning)
	{
	  StateMachine(controls.isStateMachineRunning,controls.adr);
	}
}
