// tsar_burn_termination_3.c
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
// TODO UPDATE THIS
// Change Log
// 	-Creation of Document 5/17/2020 [APJ]
// 	... Check GITHUB, bad at logging
//  - Utilized Initiate and logging commands 12/5/2020 [APJ]
//  - Renamed SHUTDOWN 12/7/2020 [APJ]
//
//	This file contains methods for the Shutdown
//  state.
//  Shutdown has two verification Methods
//  It first checks that both current and previous states are valid
//  It then checks if the expected state is the state passed
//  At this point the valves are opened and a log entry is created
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include <shutdown.h>

uint32_t Shutdown(struct StateVars *ctrl)
{
	uint32_t success = FALSE;
	ctrl->valveConfiguration = StateConfiguration();
	ctrl->valveTarget  = (uint16_t)SOV3;
	uint32_t now = HAL_GetTick();
	uint32_t TIMEOUT = 10000;

    if(VerifyState(ctrl->currentState) && VerifyState(ctrl->lastState))
    {
    	if((ctrl->currentState & SHUTDOWN) == SHUTDOWN){
    		// SOV1   SOV2   SOV3   SOV4   SOV5   SOV6   SOV7   SOV8
    		//|  0  |   0  |   1  |   0  |   0  |   0  |   0  |   0  |

    	    // If this is the first time, initialize state
    		if(ctrl->currentState != ctrl->lastState)
    	    {
    			success = StateInitialize(ctrl);
    	    }

    		// OnTick
    		success = SendStatusMessage(ctrl);

    		// TODO: if(data in buffer) ProcessMessages();
			ProcessMessages(ctrl);
			success = (ctrl->valveConfiguration == ctrl->valveTarget ? TRUE : FALSE);

    		//TODO Specify time frame
    		if(now - ctrl->timeStarted > TIMEOUT && success)
    		{
        		ctrl->currentState= FUEL_DRAIN;
    		}

    		// Increment state counter
			ctrl->stateCounter++;
			if(ctrl->stateCounter >= 4294967295) ctrl->stateCounter = 0;

    	}else{
    		// Log Expected State != Passed State
    		Get_State_Disagree_Error_Msg(TxMessageBuffer1, SHUTDOWN, ctrl->currentState);
    		UART_SendMessage(&hlpuart1, TxMessageBuffer1);
    	}
    }else{
    	// Log Invalid State
    	Get_Invalid_State_Error_Msg(TxMessageBuffer1, ctrl->currentState, ctrl->lastState);
    	UART_SendMessage(&hlpuart1, TxMessageBuffer1);
    }
	return success;
}
