// tsar_state_machine.h
// Portland State Aerospace Society
// TSAR_Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//  -Added StateMachine(uint32_t, enum) 2/11/2020 [APJ]
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
	uint32_t success = FALSE;
	//enum StateName _nextState = Incoming;
	while(TRUE == control)
	{
		if(VerifyState(ctrl->currentState))
		{
			switch(ctrl->currentState)
			{
				case SETUP_OPS:
					control = SetupOps(ctrl);
					break;
				//case LOX_FILL:
					//control = LoxFill(ctrl);
					//break;
				case SITE_CLEAR_AND_START_UP:
					control = SiteClear(ctrl);
					break;
				case PRESSURIZATION:
					control = SiteClear(ctrl);
					break;
				case PRE_CHILL:
					control = PreChill(ctrl);
					break;
				case IGNITION:
					control = Ignition(ctrl);
					break;
				case LOX_INTRO:
					control = LoxIntro(ctrl);
					break;
				case BURN_INITIATED:
					control = BurnInitiated(ctrl);
					break;
				case BURN_FEEDBACK:
					control = BurnFeedback(ctrl);
					break;
				case BURN_TERMINATION_1:
					control = BurnTermination1(ctrl);
					break;
				case BURN_TERMINATION_2:
					control = BurnTermination2(ctrl);
					break;
				case BURN_TERMINATION_3:
					control = BurnTermination3(ctrl);
					break;
				case PURGE:
					control = Purge(ctrl);
					break;
				case POST_FIRE:
					control = PostFire(ctrl);
					break;
				//case SAFE_APPROACH:
					//control = SafeApproach(ctrl);
					//break;
				case FAILURE:
					control = Failure(ctrl);
					break;
				//case GROUNDSTATION:
					//control = Groundstation(ctrl);
					//break;
				case TEST:
					control = Test(ctrl);
					break;
				case SAFETY:
					control = Safety(ctrl);
					break;

				default:
					ctrl->currentState = FAILURE;
					break;
			}
		}else
		{
			char message[PRINT_BUFFER_SIZE];
			char *msgPtr = message;
			ctrl->currentState = FAILURE;
	    	// Log Invalid State
	    	Get_Invalid_State_Error_Msg(msgPtr, ctrl->currentState, ctrl->lastState);
	    	UART_SendMessage(&hlpuart1, msgPtr);
			//ERROR HANDLE
		}
		// WD check
	}
}

