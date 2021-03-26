// start_handler.c
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//  STart Handler has two verification Methods
//  It first checks that both current and previous states are valid
//  It then checks if the expected state is the state passed
//  At this point the valves are opened and a log entry is created
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include <start_handler.h>
#include <messages.h>
#include "ring_buffer.h"

uint32_t StartHandler(struct StateVars *ctrl){

	uint32_t success = FALSE;
	ctrl->valveConfiguration = StateConfiguration();

    if(VerifyState(ctrl->currentState) && VerifyState(ctrl->lastState)){
    	if((ctrl->currentState & START_HANDLER) == START_HANDLER){
    		// SOV1   SOV2   SOV3   SOV4   SOV5   SOV6   SOV7   SOV8   SOV9
    		//|  0  |    0  |  0  |   0  |   0  |   0  |   0  |   0  |   0




    		// If this is the first time, initialize state
     		if(ctrl->currentState != ctrl->lastState)
    	    {
     			ctrl->valveTarget  = !((uint16_t)SOV1 \
     			    					|(uint16_t)SOV2 \
     			    					|(uint16_t)SOV3 \
     			    					|(uint16_t)SOV4 \
     			    					|(uint16_t)SOV5 \
     			    					|(uint16_t)SOV6 \
     			    					|(uint16_t)SOV7 \
     			    					|(uint16_t)SOV8 \
     			    					|(uint16_t)SOV9 \
     									|(uint16_t)IG1 );
    	    	success = StateInitialize(ctrl);
    	    }

    		ProcessCommands(ctrl);

			success &= (ctrl->valveConfiguration == ctrl->valveTarget ? TRUE : FALSE);

			//nomVal = NominalValueCheck(ctrl);
			//if (nomVal)
			// Read from ring Buffer and check nominal value
			ringBufferRead();

    		// Increment state counter
			ctrl->stateCounter++;
			if(ctrl->stateCounter >= 4294967295) ctrl->stateCounter = 0;
    	}else{
    		//Log Expected State != Passed State
    		Get_State_Disagree_Error_Msg(Tx_Buffer, START_HANDLER, ctrl->currentState);
    		UART_SendMessage(&huart2,Tx_Buffer);
    	}
    }else{
    	    	// Log Invalid State
    	Get_Invalid_State_Error_Msg(Tx_Buffer, ctrl->currentState, ctrl->lastState);
    	UART_SendMessage(&huart2, Tx_Buffer);
    }
	return success;
}
