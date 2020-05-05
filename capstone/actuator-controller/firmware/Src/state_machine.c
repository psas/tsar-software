// tsar_state_machine.h
// Portland State Aerospace Society
// TSAR_Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//  -Added StateMachine(bool, enum) 2/11/2020 [APJ]
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

void StateMachine(bool control, enum StateName Incoming)
{
	enum StateName _state = Incoming;
	enum StateName _lastState = Incoming;
	uint32_t success = FALSE;
	//enum StateName _nextState = Incoming;
	while(control)
	{
		if(VerifyState(_state))
		{
			_lastState = _state;
			switch(_state)
			{
				case SETUP_OPS:
					control = SetupOps(_state, _lastState);
					break;
				case LOX_FILL:
					control = LoxFill(_state,_lastState);
					break;
				case SITE_CLEAR_AND_START_UP:
					control = SiteClear(_state,_lastState);
					break;
				case PRESSURIZATION:
					control = SiteClear(_state,_lastState);
					break;
				case PRE_CHILL:
					control = PreChill(_state,_lastState);
					break;
				case IGNITION:
					control = Ignition(_state,_lastState);
					break;
				case LOX_INTRO:
					control = LoxIntro(_state,_lastState);
					break;
				case BURN_INITIATED:
					control = BurnInitiated(_state, _lastState);
					break;
				case BURN_FEEDBACK:
					control = BurnFeedback(_state,_lastState);
					break;
				case BURN_TERMINATION_1:
					control = BurnTermination1(_state,_lastState);
					break;
				case BURN_TERMINATION_2:
					control = BurnTermination2(_state,_lastState);
					break;
				case BURN_TERMINATION_3:
					control = BurnTermination3(_state,_lastState);
					break;
				case PURGE:
					control = Purge(_state,_lastState);
					break;
				case POST_FIRE:
					control = PostFire(_state,_lastState);
					break;
				case SAFE_APPROACH:
					control = SafeApproach(_state,_lastState);
					break;
				case FAILURE:
					control = Failure(_state,_lastState);
					break;
				case GROUNDSTATION:
					control = Groundstation(_state,_lastState);
					break;
				case TEST:
					control = Test(_state,_lastState);
					break;
				case SAFETY:
					control = Safety(_state,_lastState);
					break;

				default:
					_state = FAILURE;
					break;
			}
		}else
		{
			_state = FAILURE;
			//Log Invalid State
			//ERROR HANDLE
		}
		// WD check
	}
}

