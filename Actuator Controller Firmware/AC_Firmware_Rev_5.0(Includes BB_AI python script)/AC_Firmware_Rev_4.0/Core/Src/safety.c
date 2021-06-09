// safety.c
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This file contains methods for the SAFETY state
//
//  Safety has two verification Methods:
//  It first checks that both current and previous states are valid
//  It then checks if the expected state is the state passed
//  At this point the valves are opened and a log entry is created
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include "safety.h"
#include "messages.h"
#include "ring_buffer.h"


uint32_t Safety(struct StateVars *ctrl) {

	uint32_t success = FALSE;

	ctrl->valveConfiguration = StateConfiguration();
	ctrl->valveTarget  = !((uint16_t)pvf01 |(uint16_t)pvl01 |(uint16_t)pvp01 \
									|(uint16_t)sovf01 |(uint16_t)pvf02 |(uint16_t)pvl03 \
									|(uint16_t)pvlf01 |(uint16_t)pvl02 |(uint16_t)pvp02 \
									|(uint16_t)IG1);

	if (VerifyState(ctrl->currentState) && VerifyState(ctrl->lastState)) {
		if ((ctrl->currentState & SAFETY) == SAFETY) {
			// pvf01   pvl01   pvp01   sovf01   pvf02   pvl03   pvlf01   pvl02  pvp02   IG1
			// | 0  |   0  |   0  |   0  |   0  |   0  |   0 |    0  |  0  |  0

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

			ctrl->stateCounter++;
			if(ctrl->stateCounter >= 4294967295) ctrl->stateCounter = 0;

		} else {
			// Log Expected State != Passed State
			Get_State_Disagree_Error_Msg(Tx_Buffer, SAFETY, ctrl->currentState);
			UART_SendMessage(&huart2,Tx_Buffer);
		}
	} else {
		// Log Invalid State
		Get_Invalid_State_Error_Msg(Tx_Buffer, ctrl->currentState, ctrl->lastState);
		UART_SendMessage(&huart2,Tx_Buffer);
	}
	return success;
}
