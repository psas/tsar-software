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

void StateMachine(uint32_t control, enum StateName Incoming, uint32_t isSafetyGreen)
{
	enum StateName _state = Incoming;
	enum StateName _lastState = Incoming;
	enum StateName *statePtr;
	enum StateName *lastStatePtr;

	statePtr = &_state;
	lastStatePtr = &_lastState;

	uint32_t success = FALSE;
	//enum StateName _nextState = Incoming;
	while(TRUE == control)
	{
		if(VerifyState(_state))
		{
			_lastState = _state;
			switch(_state)
			{
				case SETUP_OPS:
					control = SetupOps(statePtr, lastStatePtr);
					break;
				case LOX_FILL:
					control = LoxFill(statePtr,lastStatePtr);
					break;
				case SITE_CLEAR_AND_START_UP:
					control = SiteClear(statePtr,lastStatePtr);
					break;
				case PRESSURIZATION:
					control = SiteClear(statePtr,lastStatePtr);
					break;
				case PRE_CHILL:
					control = PreChill(statePtr,lastStatePtr);
					break;
				case IGNITION:
					control = Ignition(statePtr,lastStatePtr);
					break;
				case LOX_INTRO:
					control = LoxIntro(statePtr,lastStatePtr);
					break;
				case BURN_INITIATED:
					control = BurnInitiated(statePtr, lastStatePtr);
					break;
				case BURN_FEEDBACK:
					control = BurnFeedback(statePtr,lastStatePtr);
					break;
				case BURN_TERMINATION_1:
					control = BurnTermination1(statePtr,lastStatePtr);
					break;
				case BURN_TERMINATION_2:
					control = BurnTermination2(statePtr,lastStatePtr);
					break;
				case BURN_TERMINATION_3:
					control = BurnTermination3(statePtr,lastStatePtr);
					break;
				case PURGE:
					control = Purge(statePtr,lastStatePtr);
					break;
				case POST_FIRE:
					control = PostFire(statePtr,lastStatePtr);
					break;
				case SAFE_APPROACH:
					control = SafeApproach(statePtr,lastStatePtr);
					break;
				case FAILURE:
					control = Failure(statePtr,lastStatePtr);
					break;
				case GROUNDSTATION:
					control = Groundstation(statePtr,lastStatePtr);
					break;
				case TEST:
					control = Test(statePtr,lastStatePtr);
					break;
				case SAFETY:
					control = Safety(statePtr,lastStatePtr,isSafetyGreen);
					break;

				default:
					_state = FAILURE;
					break;
			}
		}else
		{
			char message[PRINT_BUFFER_SIZE];
			char *msgPtr = message;
			_state = FAILURE;
	    	// Log Invalid State
	    	Get_Invalid_State_Error_Msg(msgPtr, _state, _lastState);
	    	UART_SendMessage(&hlpuart1, msgPtr);
			//ERROR HANDLE
		}
		// WD check
	}
}

