// burn_termination_2.c
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
// TODO Update this
// Change Log
// 	-Creation of Document 5/17/2020 [APJ]
//  - Renamed to FIRING 12/7/2020 [APJ]
//
//	This file contains methods for the Burn Termination 2
//  state.
//  Firing has two verification Methods
//  It first checks that both current and previous states are valid
//  It then checks if the expected state is the state passed
//  At this point the valves are opened and a log entry is created
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include <firing.h>

uint32_t Firing(struct StateVars *ctrl)
{
	uint32_t success = FALSE;
	ctrl->valveConfiguration = StateConfiguration();
	ctrl->valveTarget  = ((uint16_t)SOV1 	\
			 |(uint16_t)SOV2 	\
			 |(uint16_t)SOV5 	\
			 |(uint16_t)SOV6);
	uint32_t now = HAL_GetTick();
	uint32_t TIMEOUT = 10000;

    if(VerifyState(ctrl->currentState) && VerifyState(ctrl->lastState))
    {
    	if((ctrl->currentState & FIRING) == FIRING){
    		// SOV1   SOV2   SOV3   SOV4   SOV5   SOV6   SOV7   SOV8
    		//|  1  |   1  |   0  |   0  |   1  |   1  |   0  |   0  |

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

    		//TODO Specify time frame
    		if(now - ctrl->timeStarted > TIMEOUT && success)
    		{
        		ctrl->currentState= PURGE;
    		}

    		// Increment state counter
    		ctrl->stateCounter++;
    		if(ctrl->stateCounter >= 4294967295) ctrl->stateCounter = 0;
    	}else{
    		// Log Expected State != Passed State
    		Get_State_Disagree_Error_Msg(TxMessageBuffer1, FIRING, ctrl->currentState);
    		UART_SendMessage(&hlpuart1, TxMessageBuffer1);
    	}
    }else{
    	// Log Invalid State
    	Get_Invalid_State_Error_Msg(TxMessageBuffer1, ctrl->currentState, ctrl->lastState);
    	UART_SendMessage(&hlpuart1, TxMessageBuffer1);
    }
	return success;
}
