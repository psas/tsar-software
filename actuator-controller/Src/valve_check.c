// tsar_setup_ops.c
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//  - Renamed valve check 12/6/2020 [APJ]
//
//	This file contains methods for the Setup Operations
//  state.
//  ValveCheck has two verification Methods
//  It first checks that both current and previous states are valid
//  It then checks if the expected state is the state passed
//  At this point the valves are opened and a log entry is created
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include <valve_check.h>

uint32_t ValveCheck(struct StateVars *ctrl)
{
	uint32_t success = TRUE;
	ctrl->valveConfiguration = StateConfiguration();

	uint32_t now = HAL_GetTick();
	uint32_t TIMEOUT = 10000;

    if(ctrl->currentState != ctrl->lastState)
    {
    	ctrl->timeStarted = now;
    }

    if(VerifyState(ctrl->currentState) && VerifyState(ctrl->lastState))
    {
    	if((ctrl->currentState & VALVE_CHECK) == VALVE_CHECK){
    		// SOV1   SOV2   SOV3   SOV4   SOV5   SOV6   SOV7   SOV8
    		//|  0  |    0  |  0  |   1  |   0  |   0  |   0  |   1

    	    // If this is the first time, initialize state
    		if(ctrl->currentState != ctrl->lastState)
    	    {
    			ctrl->valveTarget  = ((uint16_t)SOV1 \
    					|(uint16_t)SOV2 \
    					|(uint16_t)SOV3 \
    					|(uint16_t)SOV4 \
    					|(uint16_t)SOV5 \
    					|(uint16_t)SOV6 \
    					|(uint16_t)SOV7 \
    					|(uint16_t)SOV8);
    			success = StateInitialize(ctrl);
    	    }

    		// OnTick
    		if(0 == ctrl->stateCounter % 500)
    		{
    			success &= CycleValves(ctrl);
    		}

    		// TODO: if(data in buffer) ProcessMessages();
			ProcessMessages(ctrl);
			success &= (ctrl->valveConfiguration == ctrl->valveTarget ? TRUE : FALSE);

    		//TODO Specify time frame
    		if(now - ctrl->timeStarted > TIMEOUT && success )
    		{
        		ctrl->currentState = FUEL_FILL;
    		}

    		// Increment state counter
			ctrl->stateCounter++;
			if(ctrl->stateCounter >= 4294967295) ctrl->stateCounter = 0;
    	}else{
    		//Log Expected State != Passed State
    		Get_State_Disagree_Error_Msg(TxMessageBuffer1, VALVE_CHECK, ctrl->currentState);
    		UART_SendMessage(&hlpuart1,TxMessageBuffer1);
    	}
    }else{
    	    	// Log Invalid State
    	Get_Invalid_State_Error_Msg(TxMessageBuffer1, ctrl->currentState, ctrl->lastState);
    	UART_SendMessage(&hlpuart1, TxMessageBuffer1);
    }
	return success;
}

uint32_t CycleValves(struct StateVars *ctrl)
{
	uint32_t success = TRUE;

	ctrl->valveTarget *= 2;

	if(ctrl->valveTarget > 0xFF)
	{
		ctrl->valveTarget = 0x1;
	}

	success &= ValveStateSetter(ctrl->valveTarget);
	ctrl->valveConfiguration = StateConfiguration();
	success &= SendStatusMessage(ctrl);

	return success;
}

