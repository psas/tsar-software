// tsar_setup_ops.c
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//
//	This file contains methods for the Setup Operations
//  state.
//  SetupOps has two verification Methods
//  It first checks that both current and previous states are valid
//  It then checks if the expected state is the state passed
//  At this point the valves are opened and a log entry is created
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include "setup_ops.h"

bool SetupOps(enum StateName state, enum StateName lastState)
{
	bool success = false;
	uint16_t valve_configuration = 0;
	uint16_t valve_target = 0;
    if(VerifyState(state) && VerifyState(lastState))
    {
    	if(state & SETUP_OPS == 0){
    		// PV1 PV2 PV3 VV1 VV2 IV1 IV2 MV1 MV2
    		// 11 |11 |11 |11 |11 |11 |11 |11 |11

    		// Set Valve States
    		valve_target |= (uint16_t)VV1|(uint16_t)VV2;
    		// Timestamp and Log
    		valve_configuration = StateConfiguration();
    		// Get current time, Log  Pin Configuration
    		// Change State conditions
    		lastState=state;
    		state = SETUP_OPS;
    		success = (valve_configuration == valv_target ? true : false);
    	}else{
    		// Log Expected State != Passed State

    	}
    }else{
    	// Log Invalid State
    }
	return success;
}
