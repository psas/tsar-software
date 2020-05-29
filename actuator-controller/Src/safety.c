// safety.c
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
// TODO UPDATE header
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//
//	This file contains methods for the SAFETY
//  state.
//  Safety has two verification Methods
//  It first checks that both current and previous states are valid
//  It then checks if the expected state is the state passed
//  At this point the valves are opened and a log entry is created
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include "safety.h"

uint32_t Safety(enum StateName *statePtr, enum StateName *lastStatePtr, uint32_t isSafetyGreen)
{
	uint32_t success = FALSE;
	uint32_t valve_configuration = 0;
	uint32_t valve_target = 0;
	char message[256];
	char *msgPtr = message;


    if(VerifyState(*statePtr) && VerifyState(*lastStatePtr))
    {
    	if((*statePtr & SAFETY) == SAFETY){
    		// PV1 PV2 PV3 VV1 VV2 IV1 IV2 MV1 MV2
    		// | 0|  0|  0| 1|  1|  0|  0|  0|   0
    		// Set Valve States
    		valve_target |= (uint16_t)VV1|(uint16_t)VV2;
    		ValveStateSetter(valve_target);
    		valve_configuration = StateConfiguration();
    		lastStatePtr = statePtr;
    		*statePtr = SAFETY;
    		success = (valve_configuration == valve_target ? TRUE : FALSE);
    		// Create Message and Transmit
    		Get_Valve_State_Status_Msg(msgPtr,valve_configuration,success);
    		UART_SendMessage(&hlpuart1, msgPtr);
    		if(isSafetyGreen && success)
    		{
    			// Check for Safety Green
    			*statePtr =SETUP_OPS;
    		}
    	}else{
    		// Log Expected State != Passed State
    		Get_State_Disagree_Error_Msg(msgPtr, SAFETY, *statePtr);
    		UART_SendMessage(&hlpuart1, msgPtr);
    	}
    }else{
    	// Log Invalid State
    	Get_Invalid_State_Error_Msg(msgPtr, *statePtr, *lastStatePtr);
    	UART_SendMessage(&hlpuart1, msgPtr);
    }
	return success;
}
