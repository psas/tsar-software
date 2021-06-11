// automatic_shutdown.c
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This file handles the Automatic Shutdown sequence: Purge + Safety states
//  Automatic Shutdown has two verification Methods
//  It first checks that both current and previous states are valid
//  It then checks if the expected state is the state passed
//  At this point the valves are opened and a log entry is created
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include <automatic_shutdown.h>
#include "utilities.h"
#include "ring_buffer.h"


//Automatic shutdown : Purge and go back to SAFETY

uint32_t AutomaticShutdown(struct StateVars *ctrl)
{
	uint32_t success = FALSE;
	ctrl->valveConfiguration = StateConfiguration();

	/*FUEL PURGE N2  - LOX PURGE N2*/
	ctrl->valveTarget  = ((uint16_t)SOV3 	\
			 |(uint16_t)SOV9);


	uint32_t TIMEOUT = 3000; // 3 second time out
	
    if(VerifyState(ctrl->currentState) && VerifyState(ctrl->lastState))
    {
    	if((ctrl->currentState & AUTOMATIC_SHUTDOWN) == AUTOMATIC_SHUTDOWN){


    		StateStatus(ctrl);

    		uint32_t now = HAL_GetTick();
    		// If this is the first time, initialize state
    		if(ctrl->currentState != ctrl->lastState)
    	    {
    			success = StateInitialize(ctrl);
    	    }


    		success = (ctrl->valveConfiguration == ctrl->valveTarget ? TRUE : FALSE);

    		 if(now - ctrl->timeStarted > TIMEOUT && success)
    		   {
    		       ctrl->currentState= SAFETY;
    		   }

    		 // Increment state counter
			ctrl->stateCounter++;
			if(ctrl->stateCounter >= 4294967295) ctrl->stateCounter = 0;
    	}else{
    		// Log Expected State != Passed State
    		Get_State_Disagree_Error_Msg(Tx_Buffer, AUTOMATIC_SHUTDOWN, ctrl->currentState);
    		UART_SendMessage(&huart2, Tx_Buffer);
    	}
    }else{
    	// Log Invalid State
    	Get_Invalid_State_Error_Msg(Tx_Buffer, ctrl->currentState, ctrl->lastState);
    	UART_SendMessage(&huart2, Tx_Buffer);
    }
    circular_buffer_readFlag = TRUE;
	return success;
}
