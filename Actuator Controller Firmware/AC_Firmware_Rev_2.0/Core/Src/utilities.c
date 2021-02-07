// utilities.c
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This file contains methods for verification and
//  manipulation of state changes
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include "utilities.h"
#include "messages.h"
//Function that sends the current state status to the user
void StateStatus(struct StateVars *ctrl){

     int state = ctrl->currentState;

	const char* naming[] = { [0] = "State : Start Handler\n\r", [1] = "State : Valve Check\n\r" ,	\
		[2] = "State : Lox Pre Fill\n\r", [3] = "State : Lox Fill\n\r", [4] = "State : Lox Chill\n\r",	\
		[5] = "State : Safety\n\r", [6] = "State : Pressurize\n\r", [7] = "State : Ignition\n\r",	[8] = "\nState : Oxidizer Start\n\r",\
		[9] = "State : Firing\n\r", [10] = "State : Purge\n\r",	[11] = "State : Turn Off\n\r", [12] = "State : Automatic Shutdown\n\r"};

		if (ctrl->lastState  != ctrl->currentState){
			UART_SendMessage(&huart2, naming[state]);
			// Flag for the transitioning to the same state
			retransition = FALSE;
	        }
		else if(status)
		{
			UART_SendMessage(&huart2, naming[state]);
			status = FALSE;

		}
}// end of function



/*
 * 	Function VerifyState()
 *
 *  Params:
 *  	state <int>: State to be verified
 *
 *  Returns:
 *  	success <uint32_t>: TRUE | FALSE | 1 | 0
 *
 *  Notes:
 */
uint32_t VerifyState(int state)
{

	uint32_t success = FALSE;
    if(state <= TURN_OFF && state >= START_HANDLER)
    {
    	success = TRUE;
		switch(state)
		{

			case START_HANDLER:
				break;
			case VALVE_CHECK:
				break;
			case LOX_PRE_FILL:
				break;
			case LOX_FILL:
				break;
			case LOX_CHILL:
				break;
			case SAFETY:
				break;
			case PRESSURIZE:
				break;
			case IGNITION:
				break;
			case OXIDIZER_START:
				break;
			case FIRING:
				break;
			case PURGE:
				break;
			case AUTOMATIC_SHUTDOWN:
				break;
			case TURN_OFF:
				break;

			default:
				success = FALSE;
				break;
		}
    }
	return success;
}

/*
 * 	Function StateConfiguration()
 *
 *  Params:
 *  	NONE
 *
 *  Returns:
 *  	vc <uint32_t>: Current reported valve states
 *
 *  Notes:
 */

uint32_t StateConfiguration()
{
	uint32_t vc = 0;
	//SOV1 - Fuel Pressure (PA_11)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOA, FUEL_PRESSURE_Pin) << 0);

	//SOV2 - Lox Pressure (PA_12)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOA, LOX_PRESSURE_Pin) << 1);

	//SOV3 - Fuel Purge N2 (PC_7)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOC, FUEL_PURGE_N2_Pin) << 2);

	//SOV4 - Fuel Vent (PC_6)
	vc |= (uint32_t)(HAL_GPIO_ReadPin (GPIOC, FUEL_VENT_Pin) << 3);

	//SOV5 - Fuel Main (PA_9)
	vc |= (uint32_t)(HAL_GPIO_ReadPin (GPIOA, FUEL_MAIN_Pin) << 4);

	//SOV6 - Lox Main (PA_8)
	vc |= (uint32_t)(HAL_GPIO_ReadPin (GPIOA, LOX_MAIN_Pin) << 5);

	//SOV7 - Lox Fill (PB_15)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOB, LOX_FILL_Pin) << 6);

	//SOV8 - Lox Vent (PB_10)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOB, LOX_VENT_Pin) << 7);

	//SOV9 - Lox Purge (PB_13)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin) << 8);

	//IG1 - Igniter (PA_10)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) << 9);

	return vc;
}

/*
 * 	Function ValveSetter()
 *
 *  Params:
 *  vs <uint32_t>: Valve settings to be set
 *
 *  Returns:
 *  success <uint32_t>: TRUE | FALSE | 1 | 0
 *
 *  Notes:
 */

uint32_t ValveStateSetter(uint32_t vs)
{
	uint32_t success = FALSE;
	//SOV1
	(vs & (uint32_t)SOV1) == (uint32_t)SOV1 \
			? HAL_GPIO_WritePin(GPIOA, FUEL_PRESSURE_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, FUEL_PRESSURE_Pin,GPIO_PIN_RESET);

	//SOV2
	(vs & (uint32_t)SOV2) == (uint32_t)SOV2 \
			? HAL_GPIO_WritePin(GPIOA, LOX_PRESSURE_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, LOX_PRESSURE_Pin,GPIO_PIN_RESET);

	//SOV3
	(vs & (uint32_t)SOV3) == (uint32_t)SOV3 \
			? HAL_GPIO_WritePin(GPIOC, FUEL_PURGE_N2_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOC, FUEL_PURGE_N2_Pin,GPIO_PIN_RESET);

	//SOV4
	(vs & (uint32_t)SOV4) == (uint32_t)SOV4 \
			? HAL_GPIO_WritePin(GPIOC, FUEL_VENT_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOC, FUEL_VENT_Pin,GPIO_PIN_RESET);

	//SOV5
	(vs & (uint32_t)SOV5) == (uint32_t)SOV5 \
			? HAL_GPIO_WritePin(GPIOA, FUEL_MAIN_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, FUEL_MAIN_Pin,GPIO_PIN_RESET);

	//SOV6
	(vs & (uint32_t)SOV6) == (uint32_t)SOV6 \
			? HAL_GPIO_WritePin(GPIOA, LOX_MAIN_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, LOX_MAIN_Pin,GPIO_PIN_RESET);

	//SOV7
	(vs & (uint32_t)SOV7) == (uint32_t)SOV7 \
			? HAL_GPIO_WritePin(GPIOB, LOX_FILL_Pin ,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOB, LOX_FILL_Pin,GPIO_PIN_RESET);

	//SOV8
	(vs & (uint32_t)SOV8) == (uint32_t)SOV8 \
			? HAL_GPIO_WritePin(GPIOB, LOX_VENT_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOB, LOX_VENT_Pin,GPIO_PIN_RESET);

	//SOV9
	(vs & (uint32_t)SOV9) == (uint32_t)SOV9 \
			? HAL_GPIO_WritePin(GPIOB, LOX_PURGE_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOB, LOX_PURGE_Pin,GPIO_PIN_RESET);

	//IG1
	(vs & (uint32_t)IG1) == (uint32_t)IG1 \
			? HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,GPIO_PIN_RESET);


	success = (vs == StateConfiguration() ? TRUE : FALSE);

	return success;
}


/* Function: uint32_t StateInitialize(enum StateName new, struct StateVars *ctrl)
 *
 * - Initialize state: Resets stateCounter, Logs entry time, set's new state and last state
 *   attempts to set valves
 *
 *  Params:
 *  hlpuart <pointer>: Pointer the the lpuart typedef created at initialization.
 *
 *  Returns:
 *  success <uint32_t>: TRUE | FALSE | 1 | 0
 *
 *  Notes:
 */
uint32_t StateInitialize(struct StateVars *ctrl)
{
	uint32_t success = FALSE;
	// Reset state counter, log start
	ctrl->stateCounter = 0;
	ctrl->timeStarted = HAL_GetTick();;

	// Change State conditions
	ctrl->lastState = ctrl->currentState;

	// Set Valve States
	ValveStateSetter(ctrl->valveTarget);
	ctrl->valveConfiguration = StateConfiguration();

	///success = SendStatusMessage(ctrl);
	return success;
}

