// tsar_state_machine.h
// Portland State Aerospace Society
// TSAR_Capstone 2019-2020
//
// Change Log
// 	- Creation of Document 2/11/2020 [APJ]
//  - Added StateMachine(uint32_t, enum) 2/11/2020 [APJ]
//  - Fixed states to states described 8/2020 [APJ]
//
//	This file describes the state flow of the Actuator-
//  Controller.  This serves as the primary loop for
//  the controller.
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#include "state_machine.h"

void StateMachine(uint32_t control, struct StateVars *ctrl)
{
	while(TRUE == control)
	{
		if(VerifyState(ctrl->currentState))
		{
			switch(ctrl->currentState)
			{
				case SAFETY:
					control = Safety(ctrl);
					break;
				case VALVE_CHECK:
					control = ValveCheck(ctrl);
					break;
				case FUEL_FILL:
					control = FuelFill(ctrl);
					break;
				case LOX_PRE_FILL:
					control = LoxPreFill(ctrl);
					break;
				case LOX_CHILL:
					control = LoxChill(ctrl);
					break;
				case LOX_FILL:
					control = LoxFill(ctrl);
					break;
				case READY_STATE:
					control = ReadyState(ctrl);
					break;
				case PRESSURIZE:
					control = Pressurize(ctrl);
					break;
				case IGNITION:
					control = Ignition(ctrl);
					break;
				case LOX_START:
					control = LoxStart(ctrl);
					break;
				case FUEL_START:
					control = FuelStart(ctrl);
					break;
				case FIRING:
					control = Firing(ctrl);
					break;
				case PURGE:
					control = Purge(ctrl);
					break;
				case LOX_BOILOFF:
					control = LoxBoiloff(ctrl);
					break;
				case SHUTDOWN:
					control = Shutdown(ctrl);
					break;
				case FUEL_DRAIN:
					control = FuelDrain(ctrl);
					break;
				case FAILURE:
					control = Failure(ctrl);
					break;
				case TEST:
					control = Test(ctrl);
					break;

				default:
					ctrl->currentState = FAILURE;
					break;
			}
		}else
		{
			ctrl->currentState = FAILURE;
	    	// Log Invalid State
	    	Get_Invalid_State_Error_Msg(TxMessageBuffer1, ctrl->currentState, ctrl->lastState);
	    	UART_SendMessage(&hlpuart1, TxMessageBuffer1);
			//ERROR HANDLE
		}
		// WD check
	}
}

