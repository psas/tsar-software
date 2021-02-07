// valve_check.c
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This file contains the Valve check section.
//
//  ValveCheck has two verification Methods
//  It first checks that both current and previous states are valid
//  It then checks if the expected state is the state passed
//  At this point the valves are opened and a log entry is created
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include <valve_check.h>
#include <messages.h>

uint32_t ValveCheck(struct StateVars *ctrl)
{
	uint32_t success = TRUE;
	ctrl->valveConfiguration = StateConfiguration();

    if(VerifyState(ctrl->currentState) && VerifyState(ctrl->lastState))
    {
    	if((ctrl->currentState & VALVE_CHECK) == VALVE_CHECK){
    		// SOV1   SOV2   SOV3   SOV4   SOV5   SOV6   SOV7   SOV8
    		//|  0  |    0  |  0  |   0  |   0  |   0  |   0  |   0

    		StateStatus(ctrl);
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
    					|(uint16_t)SOV8)\
    					|(uint16_t)SOV9;
    			success = StateInitialize(ctrl);
    	    }

    		// OnTick
    		if(0 == ctrl->stateCounter % 500)
    		{
    			success &= CycleValves(ctrl);
    		}

    		ProcessCommands(ctrl);

    		success &= (ctrl->valveConfiguration == ctrl->valveTarget ? TRUE : FALSE);

    		// Increment state counter
			ctrl->stateCounter++;
			if(ctrl->stateCounter >= 4294967295) ctrl->stateCounter = 0;
    	}else{
    		//Log Expected State != Passed State
    		Get_State_Disagree_Error_Msg(Tx_Buffer, VALVE_CHECK, ctrl->currentState);
    		UART_SendMessage(&huart2,Tx_Buffer);
    	}
    }else{
    	    	// Log Invalid State
    	Get_Invalid_State_Error_Msg(Tx_Buffer, ctrl->currentState, ctrl->lastState);
    	UART_SendMessage(&huart2, Tx_Buffer);
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

	return success;
}
