// burn_termination_2.c
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
// TODO Update this
// Change Log
// 	-Creation of Document 5/17/2020 [APJ]
//
//	This file contains methods for the Burn Termination 2
//  state.
//  BurnTermination2 has two verification Methods
//  It first checks that both current and previous states are valid
//  It then checks if the expected state is the state passed
//  At this point the valves are opened and a log entry is created
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include "burn_termination_2.h"

uint32_t BurnTermination2(struct StateVars *ctrl)
{
	uint32_t success = FALSE;
	ctrl->valveConfiguration = StateConfiguration();
	ctrl->valveTarget  = ((uint16_t)PV1 	\
			 |(uint16_t)PV2 	\
			 |(uint16_t)PV3 	\
			 |(uint16_t)VV1 	\
			 |(uint16_t)VV2 	\
			 |(uint16_t)MV1);
	uint32_t now = HAL_GetTick();
	uint32_t TIMEOUT = 10000;

    if(VerifyState(ctrl->currentState) && VerifyState(ctrl->lastState))
    {
    	if((ctrl->currentState & BURN_TERMINATION_2) == BURN_TERMINATION_2){
    		// PV1 PV2 PV3 VV1 VV2 IV1 IV2 MV1 MV2
    		// | 1| 1|  1|  1|  1|  0|  0| 10| 0

    	    // If this is the first time, initialize state
    		if(ctrl->currentState != ctrl->lastState)
    	    {
    			success = StateInitialize(ctrl);
    	    }

    		// OnTick
    		success = SendStatusMessage(ctrl);

    		//TODO Specify time frame
    		if(now - ctrl->timeStarted > TIMEOUT && success)
    		{
        		ctrl->currentState= BURN_TERMINATION_3;
    		}
    	}else{
    		// Log Expected State != Passed State
    		Get_State_Disagree_Error_Msg(TxMessageBuffer1, BURN_TERMINATION_2, ctrl->currentState);
    		UART_SendMessage(&hlpuart1, TxMessageBuffer1);
    	}

		// Increment state counter
		ctrl->stateCounter++;
		if(ctrl->stateCounter >= 4294967295) ctrl->stateCounter = 0;
    }else{
    	// Log Invalid State
    	Get_Invalid_State_Error_Msg(TxMessageBuffer1, ctrl->currentState, ctrl->lastState);
    	UART_SendMessage(&hlpuart1, TxMessageBuffer1);
    }
	return success;
}
