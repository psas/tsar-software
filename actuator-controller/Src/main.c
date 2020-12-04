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
	struct StateVars controls = {0,0,0,0, FAILURE, SAFETY, &controls};
	// TODO: Move these to init
	char RxMessageBuffer1[RX_BUFFER_SIZE];
	char *RxMessagePtr = &RxMessageBuffer1[0];
	char *RxMemEnd = &RxMessageBuffer1[RX_BUFFER_SIZE - 1];
	char *RxMemBase = &RxMessageBuffer1;
	//TODO HACK initialize isArm false
	controls.isArmed = TRUE;
	uint32_t isProgramRunning = TRUE;
	HAL_Init();

	SystemClock_Config();
	//MX_WWDG_Init();
	MX_GPIO_Init();
	MX_ADC1_Init();
	MX_LPUART1_UART_Init();


	while(1)
	{
	  StateMachine(isProgramRunning,controls.adr);
	}
}
