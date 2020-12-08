// messages.c
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
// Change Log
// 	- Creation of Document 5/18/2020 [APJ]
//  - ... Check Github, bad at logging
//	- Created ProcessMessages() 12/6/2020 [APJ]
//  - Created SendStatusMessage() 12/6/2020 [APJ]
//
//	This file contains methods for getting common actuator controller
//  messages.
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#include "messages.h"

/* ProcessMessages()
 *  This method checks the RxBuffer if it finds a whole message it processes it by
 * removing the message from the buffer, shifting the data and the index to the left.
 *
 *  Params:
 * 		ctrl <struct ptr> State Machine Control Structure
 *
 *  Returns:
 *  	success <uint32_t>: TRUE | FALSE | 1 | 0,
 *  	If a message was processed return TRUE Else FALSE
 *
 *	Notes:
 *		Current message format:
 *		A5A5 + Size of Command + Command
 *		Ex: A5A503ARM Is the ARM command (Switches ARM state);
 *	TODO:
 *		Convert to Hex Strings
 *		Implement Buffer Parsing (currently just clears it after reading successful message)
 *		Implement reading around the ring buffer (edge cases)
 */
uint32_t ProcessMessages(struct StateVars *ctrl)
{
	uint32_t match = FALSE;
	uint32_t success = FALSE;
	uint32_t iter = 0;
	uint32_t start = 0;
	uint32_t take = 0;

	char test;
	char comp = 'A';
	char *preamble_comp = "A5A5";

	while(iter < RX_BUFFER_SIZE && !match)
	{
		test = RxMessageBuffer1[iter];
		if(test == comp)
		{
			char preamble[4] = {'\0'};
			match = TRUE;
			for(int i = iter; i < iter + 4;i++)
			{
				preamble[i - iter] = RxMessageBuffer1[i];
				match &= (preamble[i -iter] == preamble_comp[i-iter]);
			}

			if(match)
			{
				start = iter;
				char c_take[2] = {'\0'};
				for(int i = start + 4; i < start + 6; i++)
				{
					c_take[i - start - 4] = RxMessageBuffer1[i];
				}
				take = atoi(c_take) + 6;
			}
		}
		iter++;
	}

	if(match)
	{
		char message[take + 1];
		char *armCmd = "A5A503ARM";
		char *powerOffCmd = "A5A508POWEROFF";
		char *emergencyStopCmd = "A5A505ESTOP";
		char *failureAckCmd = "A5A507FAILACK";
		memset(message, '\0', take + 1);
		for(int i = start; i <= start + take; i++)
		{
			message[i-start] = RxMessageBuffer1[i];
		}

		if(0 == strcmp(message, armCmd))
		{
			// Flip ARM switch, clear message buffer, point index back to beginning of buffer
			ctrl->isArmed = (ctrl->isArmed == TRUE ? FALSE : TRUE);
			memset(RxMessageBuffer1, '\0', RX_BUFFER_SIZE);
			RxMessageIdx = RxMessageBuffer1;
			success = TRUE;
		}

		if(0 == strcmp(message, powerOffCmd))
		{
			// Flip ARM switch, clear message buffer, point index back to beginning of buffer
			ctrl->isStateMachineRunning = FALSE;
			memset(RxMessageBuffer1, '\0', RX_BUFFER_SIZE);
			RxMessageIdx = RxMessageBuffer1;
			success = TRUE;
		}

		if(0 == strcmp(message, emergencyStopCmd))
		{
			ctrl->currentState = FAILURE;
			memset(RxMessageBuffer1, '\0', RX_BUFFER_SIZE);
			RxMessageIdx = RxMessageBuffer1;
			success = TRUE;
		}

		if(0 == strcmp(message, failureAckCmd))
		{
			ctrl->currentState = SAFETY;
			memset(RxMessageBuffer1, '\0', RX_BUFFER_SIZE);
			RxMessageIdx = RxMessageBuffer1;
			success = TRUE;
		}

		// Default Case
		if(!success)
		{
			for(int i = start; i <= start + take; i++)
			{
				RxMessageBuffer1[i] = 0;
			}
		}
	}

	return success;
}

/* uint32_t SendStatusMessage(struct StateVars *ctrl)
 *
 * - Checks if stateCounter is a multiple of  TICK_LENGTH. If the condition is true, generates
 * and sends a status message with the current time, valve configuration, and whether the configuration
 * matches the target
 *
 * Params:
 * 		StateVars <struct>: System controls struct
 * 		ctrl->vc <uint32_t>: Valve Configuration
 * 		ctrl->vt <uint32_t>: Valve Target Configuration
 *
 * Returns:
 *  	success <uint32_t>: TRUE | FALSE | 1 | 0
 *  		Return TRUE if this is a tick AND vc == vt
 *  		Else return FALSE
 *
 *	Notes:
 */
uint32_t SendStatusMessage(struct StateVars *ctrl)
{
	uint32_t success = FALSE;
	if(ctrl->stateCounter % TICK_LENGTH == 0 || ctrl->currentState == VALVE_CHECK)
	{
		success = (ctrl->valveConfiguration == ctrl->valveTarget ? TRUE : FALSE);

		// Create Message and Transmit
		Get_Valve_State_Status_Msg(TxMessageBuffer1, ctrl, success);
		UART_SendMessage(&hlpuart1, TxMessageBuffer1);
	}

	return success;
}

void Get_State_Disagree_Error_Msg(char *errorMessage, enum StateName expectedState, \
			enum StateName passedState)
{
	char state_disagree[VALVE_STATE_BUFFER_SIZE] = "ERROR: Expected State, Passed State Disagree. ";
	char temp[32];
	strcat(state_disagree, "Expected: ");
	sprintf(temp, "%d", expectedState);
	strcat(state_disagree, temp);
	strcat(state_disagree,"Passed: ");
	sprintf(temp, "%d", passedState);
	strcat(state_disagree, temp);
	memset(errorMessage, '\0', VALVE_STATE_BUFFER_SIZE);
	strcpy(errorMessage, state_disagree);
}

void Get_Invalid_State_Error_Msg(char *errorMessage, enum StateName state, enum StateName lastState)
{
	char state_invalid[VALVE_STATE_BUFFER_SIZE] = "Invalid State: ";
	char temp[32];
	sprintf(temp, "%d",state);
	strcat(state_invalid, temp);
	strcat(state_invalid, ", lastState: ");
	sprintf(temp, "%d", lastState);
	strcat(state_invalid, temp);
	memset(errorMessage, '\0', VALVE_STATE_BUFFER_SIZE);
	strcpy(errorMessage, state_invalid);
}


void Get_Valve_State_Status_Msg(char *statusMessage, struct StateVars *ctrl, uint32_t success)
{
	char valve_state[VALVE_STATE_BUFFER_SIZE];
	char temp[4];

	memset(valve_state, '\0', VALVE_STATE_BUFFER_SIZE);
	memset(temp, '\0', 4);

	strcat(valve_state,"VC: ");
	sprintf(temp, "%03x", (int)ctrl->valveConfiguration);
	strcat(valve_state, temp);
	strcat(valve_state, ", VT: ");
	strcat(valve_state, (success > 0 ? "True, " : "False, "));
	strcat(valve_state, "St: ");
	sprintf(temp, "%02x", (int)ctrl->currentState);
	strcat(valve_state, temp);
	memset(statusMessage, '\0', VALVE_STATE_BUFFER_SIZE);
	strcpy(statusMessage, valve_state);
}
