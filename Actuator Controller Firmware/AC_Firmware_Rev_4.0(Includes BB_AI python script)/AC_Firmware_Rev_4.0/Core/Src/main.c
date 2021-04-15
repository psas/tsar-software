// main.c
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This is the main entry point for the TSAR firmware.
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include <init.h>
#include "state_machine.h"
#include "states_def.h"
#include "init.h"
#include "ring_buffer.h"
int main(void)

{
struct StateVars controls = {
		0,			//timeStarted
		0,			//timeElapsed
		0,			//stateCounter
		0,          //safety
		0,			//pressurizing
		0, 			//lox_fill
		0,			//lox_chill
		0,			//lox_pre_fill_purge
		0,			// valve_check
		0,			//ignition
		TRUE,		//isStateMachineRunning
		0,			//valveConfiguration
		0,			//valveTarget
		TURN_OFF,	//lastState
		START_HANDLER, 	//currentState
		&controls	//this address.
		};

	HAL_Init();

	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
    circular = 0; // Flag for circular buffer init
    bufferLength = 0, readIndex = 0, writeIndex = 0;
    initOnceFlag = TRUE;

	 HAL_UART_Receive_IT(&huart2,(uint8_t *)Rx_data, 1); // Asserts UART Interrupt every time 1 byte is received.


	while(controls.isStateMachineRunning)
	{

	  StateMachine(controls.isStateMachineRunning,controls.adr);
	}

}
