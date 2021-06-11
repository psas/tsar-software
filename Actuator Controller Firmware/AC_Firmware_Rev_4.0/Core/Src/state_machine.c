// state_machine.h
// Portland State Aerospace Society
// TSAR_Capstone 2020-2021
//
//	This file describes the state flow of the Actuator-
//  Controller.  This serves as the primary loop for
//  the controller.
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
			    case START_HANDLER:
			    	control = StartHandler(ctrl);
			    	break;
				case VALVE_CHECK:
					control = ValveCheck(ctrl);
					break;
				case LOX_PRE_FILL:
					control = LoxPreFill(ctrl);
					break;
				case LOX_FILL:
					control = LoxFill(ctrl);
					break;
				case LOX_CHILL:
					control = LoxChill(ctrl);
				    break;
			    case SAFETY:
					control = Safety(ctrl);
					break;
				case PRESSURIZE:
					control = Pressurize(ctrl);
					break;
				case IGNITION:
					control = Ignition(ctrl);
					break;
				case OXIDIZER_START:
					control = OxidizerStart(ctrl);
					break;
				case FIRING:
					control = Firing(ctrl);
					break;
				case PURGE:
					control = Purge(ctrl);
					break;
				case TURN_OFF:
					control = TurnOff(ctrl);
					break;
				case AUTOMATIC_SHUTDOWN:
					control = AutomaticShutdown(ctrl);
					break;


				default:
					ctrl->currentState = AUTOMATIC_SHUTDOWN;
					break;
			}
		}else
		{
			ctrl->currentState = AUTOMATIC_SHUTDOWN;
	    	// Log Invalid State
	    	Get_Invalid_State_Error_Msg(Tx_Buffer, ctrl->currentState, ctrl->lastState);
	    	UART_SendMessage(&huart2,Tx_Buffer);
			//ERROR HANDLE
		}
		// WD check
	}
}

