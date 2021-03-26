// lox_fill.c
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This file contains methods for the LOX FILL
//  state.
//  LoxFill has two verification Methods
//  It first checks that both current and previous states are valid
//  It then checks if the expected state is the state passed
//  At this point the valves are opened and a log entry is created
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include <lox_fill.h>
#include <messages.h>
#include "ring_buffer.h"

uint32_t LoxFill(struct StateVars *ctrl){
	uint32_t success = FALSE;
	ctrl->valveConfiguration = StateConfiguration();
	ctrl->valveTarget  = ((uint16_t)SOV4 	\
			 |(uint16_t)SOV7 	\
			 |(uint16_t)SOV8);

    if(VerifyState(ctrl->currentState) && VerifyState(ctrl->lastState))
    {
    	if((ctrl->currentState & LOX_FILL) == LOX_FILL){
    		// SOV1   SOV2   SOV3   SOV4   SOV5   SOV6   SOV7   SOV8  SOV9   IG1
    		//|  0  |   0  |   0  |   1  |   0  |   0  |   1  |   1  |  0  |  0

    		StateStatus(ctrl);
    		// If this is the first time, initialize state
    		if(ctrl->currentState != ctrl->lastState)
    	    {
    			success = StateInitialize(ctrl);
    	    }


    		ProcessCommands(ctrl);

			success = (ctrl->valveConfiguration == ctrl->valveTarget ? TRUE : FALSE);

			//NominalValueCheck(ctrl, Rx_Buffer);
			ringBufferRead();

    		// Increment state counter
			ctrl->stateCounter++;
			if(ctrl->stateCounter >= 4294967295) ctrl->stateCounter = 0;

    	}else{
    		//Log Expected State != Passed State
    		Get_State_Disagree_Error_Msg(Tx_Buffer, LOX_FILL, ctrl->currentState);
    		UART_SendMessage(&huart2,Tx_Buffer);

    	}
    }else{
    	// Log Invalid State
    	Get_Invalid_State_Error_Msg(Tx_Buffer, ctrl->currentState, ctrl->lastState);
    	UART_SendMessage(&huart2,Tx_Buffer);
    }

	return success;
}
