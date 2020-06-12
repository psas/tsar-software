// tsar_site_clear.c
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
// TODO Update header site clear
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//
//	This file contains methods for the SITE_CLEAR_AND_START_UP
//  state.
//  SiteClear has two verification Methods
//  It first checks that both current and previous states are valid
//  It then checks if the expected state is the state passed
//  At this point the valves are opened and a log entry is created
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include "site_clear.h"

uint32_t SiteClear(struct StateVars *ctrl)
{
	uint32_t success = FALSE;
	uint32_t valve_configuration = 0;
	uint32_t valve_target = 0;
	uint32_t now = HAL_GetTick();
	char message[256];
	char *msgPtr = message;

	//TODO Specify a real timeout
	uint32_t TIMEOUT = 2000;

    if(VerifyState(ctrl->currentState) && VerifyState(ctrl->lastState))
    {
    	if((ctrl->currentState & SITE_CLEAR_AND_START_UP) == SITE_CLEAR_AND_START_UP){
    		// PV1 PV2 PV3 VV1 VV2 IV1 IV2 MV1 MV2
    		// | 0| 0|  0|  1|  1|  0|  0|  0|  0
    	    // If this is the first time, mark time
    		if(ctrl->currentState != ctrl->lastState)
    	    {
    	    	ctrl->timeStarted = HAL_GetTick();
    	    }

    		// Set Valve States
    		valve_target |= ((uint16_t)VV1 	\
						 |(uint16_t)VV2);
    		// Timestamp and Log
    		valve_configuration = StateConfiguration();

    		// Change State conditions
    		ctrl->lastState = ctrl->currentState;
    		ctrl->currentState = SITE_CLEAR_AND_START_UP;
    		success = (valve_configuration == valve_target ? TRUE : FALSE);

    		//TODO Specify time frame
    		if(now - ctrl->timeStarted > TIMEOUT)
    		{
        		ctrl->currentState = PRE_CHILL;
    		}

    		// Create Message and Transmit
    		Get_Valve_State_Status_Msg(msgPtr,valve_configuration,success);
    		UART_SendMessage(&hlpuart1, msgPtr);
    	}else{
    		//Log Expected State != Passed State
    		Get_State_Disagree_Error_Msg(msgPtr, SITE_CLEAR_AND_START_UP, ctrl->currentState);
    		UART_SendMessage(&hlpuart1,msgPtr);

    	}
    }else{
    	// Log Invalid State
    	Get_Invalid_State_Error_Msg(msgPtr, ctrl->currentState, ctrl->lastState);
    	UART_SendMessage(&hlpuart1, msgPtr);
    }
	return success;
}
