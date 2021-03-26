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
#include "ring_buffer.h"


uint32_t ValveCheck(struct StateVars *ctrl)
{
	uint32_t success = TRUE;
	// Configure the Valves depending in the State.
	ctrl->valveConfiguration = StateConfiguration();


    if(VerifyState(ctrl->currentState) && VerifyState(ctrl->lastState))
    {
    	if((ctrl->currentState & VALVE_CHECK) == VALVE_CHECK){
    		// SOV1   SOV2   SOV3   SOV4   SOV5   SOV6   SOV7   SOV8   SOV9  IG1
    		//|  01 |  01  |  01  |   01  |  01  |  01 |  01  |  01  |  01 |  01 |

    		StateStatus(ctrl);
    		// If this is the first time, initialize state
    		if(ctrl->currentState != ctrl->lastState)
    	    {
    			//Start by setting all the valves OFF
    			ctrl->valveTarget  = !((uint16_t)SOV1 \
    					|(uint16_t)SOV2 \
    					|(uint16_t)SOV3 \
    					|(uint16_t)SOV4 \
    					|(uint16_t)SOV5 \
    					|(uint16_t)SOV6 \
    					|(uint16_t)SOV7 \
    					|(uint16_t)SOV8 \
    					|(uint16_t)SOV9 \
						|(uint16_t)IG1);
    		    valves_selected = 0;
    			success = StateInitialize(ctrl);
    	    }
    		// Process a Marionette Data Packet
    		ringBufferRead();

    		// Check for command changes
    		ProcessCommands(ctrl);

    		// Function to handle individual valves.
    		ValveCheckCmd(ctrl);

    		//If the Valve configuration is the same as the ValveTarget set success as 1 for TRUE or 0 FALSE
    		success &= (ctrl->valveConfiguration == ctrl->valveTarget ? TRUE : FALSE);

    		// Increment state counter // Testing Purposes
			ctrl->stateCounter++;
			if(ctrl->stateCounter >= 4294967295) ctrl->stateCounter = 0;
    	}else{
    		// If current State dosn't match the expected current State
    		//Log Expected State != Passed State
    		Get_State_Disagree_Error_Msg(Tx_Buffer, VALVE_CHECK, ctrl->currentState);
    		UART_SendMessage(&huart2,Tx_Buffer);
    	}
    }else{

    	// If current State and Last State dosnt match the expected states.
    	// Log Invalid State and let the user know.
    	Get_Invalid_State_Error_Msg(Tx_Buffer, ctrl->currentState, ctrl->lastState);
    	UART_SendMessage(&huart2, Tx_Buffer);
    }
	return success;
}

