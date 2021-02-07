// safety.c
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
// TODO UPDATE header
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//
//	This file contains methods for the SAFETY
//  state.
//  Safety has two verification Methods
//  It first checks that both current and previous states are valid
//  It then checks if the expected state is the state passed
//  At this point the valves are opened and a log entry is created.
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include "safety.h"

uint32_t Safety(struct StateVars *ctrl) {
	uint32_t now = HAL_GetTick();
	uint32_t success = FALSE;
	ctrl->valveConfiguration = StateConfiguration();
	ctrl->valveTarget  = (uint16_t) SOV4 | (uint16_t) SOV8;

	if (VerifyState(ctrl->currentState) && VerifyState(ctrl->lastState)) {
		if ((ctrl->currentState & SAFETY) == SAFETY) {
			// SOV1   SOV2   SOV3   SOV4   SOV5   SOV6   SOV7   SOV8
			// | 0  |   0  |   0  |   1  |   0  |   0  |    0|    1  |

    	    // If this is the first time, initialize state
    		if(ctrl->currentState != ctrl->lastState)
    	    {
    			success = StateInitialize(ctrl);
    	    }

    		// OnTick
    		success = OnTickStatusMessage(ctrl);

    		// TODO: if(data in buffer) ProcessMessages();
			ProcessMessages(ctrl);
			success = (ctrl->valveConfiguration == ctrl->valveTarget ? TRUE : FALSE);

			if (ctrl->isArmed && success) {
				ctrl->currentState = VALVE_CHECK;
			}

			ctrl->stateCounter++;
			if(ctrl->stateCounter >= 4294967295) ctrl->stateCounter = 0;

		} else {
			// Log Expected State != Passed State
			Get_State_Disagree_Error_Msg(TxMessageBuffer1, SAFETY, ctrl->currentState);
			UART_SendMessage(&hlpuart1, TxMessageBuffer1);
		}
	} else {
		// Log Invalid State
		Get_Invalid_State_Error_Msg(TxMessageBuffer1, ctrl->currentState, ctrl->lastState);
		UART_SendMessage(&hlpuart1, TxMessageBuffer1);
	}
	return success;
}
