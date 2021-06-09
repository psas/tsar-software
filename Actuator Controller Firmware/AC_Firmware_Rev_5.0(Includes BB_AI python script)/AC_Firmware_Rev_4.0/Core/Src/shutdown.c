// SHUTDOWN.c
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This file handles the Shutdown sequence: Purge + Safety states
//  Shutdown has two verification Methods
//  It first checks that both current and previous states are valid
//  It then checks if the expected state is the state passed
//  At this point the valves are opened and a log entry is created
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include "SHUTDOWN.h"
#include "utilities.h"
#include "ring_buffer.h"
#include "messages.h"


//Shutdown : Purge and go back to SAFETY

uint32_t AutomaticShutdown(struct StateVars *ctrl)
{
	uint32_t TIMEOUT = 3300; // 3 second time out
	uint32_t delay_shut = 300; //.3 seconds so that the valves move
	uint32_t success = FALSE;

	ctrl->valveConfiguration = StateConfiguration();

	if(one_time_flag == TRUE){
		//Turn off all the valves like the safety state firts
		ctrl->valveTarget  = !((uint16_t)pvf01 |(uint16_t)pvl01 |(uint16_t)pvp01 \
								|(uint16_t)sovf01 |(uint16_t)pvf02 |(uint16_t)pvl03 \
								|(uint16_t)pvlf01 |(uint16_t)pvl02 |(uint16_t)pvp02 \
								|(uint16_t)IG1);
		one_time_flag = FALSE;
	}

    if(VerifyState(ctrl->currentState) && VerifyState(ctrl->lastState))
    {
    	if((ctrl->currentState & SHUTDOWN) == SHUTDOWN){

    		 StateStatus(ctrl);

    		uint32_t now = HAL_GetTick();
    		// If this is the first time, initialize state
    		if(ctrl->currentState != ctrl->lastState)
    	    {
    			success = StateInitialize(ctrl);
    	    }
    		success = (ctrl->valveConfiguration == ctrl->valveTarget ? TRUE : FALSE);

    		// Wait between .3 seconds to allow the  valves move
    		if((now - ctrl->timeStarted) > delay_shut && time_1 == TRUE)
    		    {
    			   /*FUEL PURGE N2  - LOX PURGE N2*/
    			   ctrl->valveTarget  = ((uint16_t)pvp01 | (uint16_t)sovf01 | (uint16_t)pvl02 | (uint16_t)pvp02);
    			   // Set Valve States
    			   	ValveStateSetter(ctrl->valveTarget);
    			   	ctrl->valveConfiguration = StateConfiguration();
    			   	time_1 = FALSE;
    		    }



             // Wait 3 second until transitioning to the safety state
    		 if((now - ctrl->timeStarted) > TIMEOUT && success)
    		   {
    		       ctrl->currentState= SAFETY;
                   // need to change command so that we stay in safety
    		       command = safety;
    		   }


    		 // Increment state counter
			ctrl->stateCounter++;
			if(ctrl->stateCounter >= 4294967295) ctrl->stateCounter = 0;
    	}else{
    		// Log Expected State != Passed State
    		Get_State_Disagree_Error_Msg(Tx_Buffer, SHUTDOWN, ctrl->currentState);
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
