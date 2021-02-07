// messages.c
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
// Change Log
// 	- Creation of Document 5/18/2020 [APJ]
//  - ... Check Github, bad at logging
//	- Created ProcessMessages() 12/6/2020 [APJ]
//  - Created OnTickStatusMessage() 12/6/2020 [APJ]
//  - Created FindMessageReturnSize() 12/8/2020 [APJ]
// 	- Optimized Processmessages(), Read around the ring 12/8/2020 [APJ]
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
 *		A5A5 + Size of Command (2bytes) + Command
 *		Ex: A5A503ARM Is the ARM command (Switches ARM state);
 *	TODO:
 *		Convert to Hex Strings
 */
uint32_t ProcessMessages(struct StateVars *ctrl)
{
	uint32_t success = TRUE;
	if((RxTxFlags & RX_DATA_READY) != 0)
	{
		uint32_t take = FindMessageReturnSize(preamble);

		if(take)
		{
			char message[take + 1];
			GetMessage(message, take + 1);

			success = ProcessCommands(ctrl, message);
		}
	}
	return success;
}

/* uint32_t ProcessCommands(struct StateVars * ctrl, char *message)
 *
 *  This method compares a c_string with commands defined in messages.h
 *  if a successful comparison is made, the appropriate command is executed.
 *  Whether successful or not, clears the message in the buffer.
 *
 *  Params:
 * 		ctrl <struct ptr>: State Machine Control Structure
 * 		message <c_string>: Command to parse
 * 		RxTxFlags GLOBAL<uint32_t>:  Rx Tx Flags
 *
 *  Returns:
 *  	success <uint32_t>: TRUE | FALSE | 1 | 0,
 *  	If a command was processed successfully return TRUE Else FALSE
 *
 *	Notes:
 */

uint32_t ProcessCommands(struct StateVars * ctrl, char *message)
{
	uint32_t success = FALSE;
	// TODO: Faster validation
	if(0 == strcmp(message, armCmd))
	{
		// Flip ARM switch, clear message
		ctrl->isArmed = (ctrl->isArmed == TRUE ? FALSE : TRUE);

		success = TRUE;

		// Clear data read flag
		RxTxFlags &= !RX_DATA_READY;
	}

	if(0 == strcmp(message, powerOffCmd))
	{
		// Flip ARM switch, clear message buffer, point index back to beginning of buffer
		ctrl->isStateMachineRunning = FALSE;
		success = TRUE;
		// Clear data read flag
		RxTxFlags &= !RX_DATA_READY;
	}

	if(0 == strcmp(message, emergencyStopCmd))
	{
		ctrl->currentState = FAILURE;
		success = TRUE;
		// Clear data read flag
		RxTxFlags &= !RX_DATA_READY;
	}

	if(0 == strcmp(message, failureAckCmd))
	{
		ctrl->currentState = SAFETY;
		success = TRUE;
		// Clear data read flag
		RxTxFlags &= !RX_DATA_READY;
	}

	// Clear the message, move the reader
	for(int i = 0; i < strlen(message) + 1;i++)
	{
		*RxMessageReader = 0;
		RxMessageReader++;
	}

	return success;
}

void GetMessage(char *message, uint32_t take)
{
	memset(message, '\0', take);
	for(int i = 0; i < take; i++)
	{
		if((RxMessageReader + i) > RxEndofBuffer)
		{
			message[i] = *(RxMessageBuffer1 + (RxMessageReader - RxEndofBuffer + i - 1));
		}else
		{
			message[i] = *(RxMessageReader + i);
		}
	}
}

/* uint32_t FindMessageReturnSize()
 *
 *  Starts reading *RxMessageBuffer1 at *RxMessageReader Searches
 *  the buffer until it finds a match to the preamble symbol defined in
 *  messages.h. It returns the size of the message detected or 0 if no
 *  message is detected.
 *
 *  Params:
 * 		*RxMessageReader GLOBAL<ptr>: Points to last read in RxMessageBuffer1
 * 		*RxMessageBuffer GLOBAL<ptr>: Points to rx ring buffer
 * 		*RxEndofBuffer GLOBAL<ptr>: Points to the end of the ring buffer
 *
 *  Returns:
 *  	take <uint32_t>: 0 | Number of chars <bytes> to take
 *
 *	Notes:
 *	TODO:
 *		Test reading around the ring buffer (edge cases)
 */

uint32_t FindMessageReturnSize(char * preamble)
{
	uint32_t valid = FALSE;
	uint32_t noMessage = FALSE;
	uint32_t take = 0;
	char *start = RxMessageReader;
	while(!valid && !noMessage)
	{
		if(preamble[0] == *RxMessageReader)
		{
			valid = TRUE;
			for(int i = 1; i < strlen(preamble); i++)
			{
				//
				if((RxMessageReader + i) > RxEndofBuffer)
				{
					valid &= (preamble[i] == *(RxMessageBuffer1 + (RxMessageReader - RxEndofBuffer + i - 1)));
				}else
				{
					valid &= (preamble[i] == *(RxMessageReader + i));
				}
			}

			if(valid)
			{
				char c_take[2] = {'\0'};
				// Case 1 splits the ring: 255 | 0
				if(RxMessageReader + 4 == RxEndofBuffer)
				{
					c_take[0] = *RxEndofBuffer;
					c_take[1] = *RxMessageBuffer1;
				// Case 2 Overshoots
				}else if((RxMessageReader + 4) > RxEndofBuffer)
				{
					c_take[0] = *(RxMessageBuffer1 + (RxMessageReader + 4 - RxEndofBuffer));
					c_take[1] = *(RxMessageBuffer1 + (RxMessageReader + 5 - RxEndofBuffer));
				// Generic case
				}else
				{
					c_take[0] = *(RxMessageReader+4);
					c_take[1] = *(RxMessageReader+5);
				}
				take = atoi(c_take) + strlen(preamble) + strlen(c_take);
			}
		}

		if(RxMessageReader >= RxEndofBuffer && !valid)
		{
			RxMessageReader = RxMessageBuffer1;
		}else if (!valid)
		{
			RxMessageReader++;
		}
		if(RxMessageReader == start)
		{
			noMessage = TRUE;
		}
	}

	return take;
}

/* uint32_t OnTickStatusMessage(struct StateVars *ctrl)
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
uint32_t OnTickStatusMessage(struct StateVars *ctrl)
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

/* uint32_t SendStatusMessage(struct StateVars *ctrl)
 *
 * - Generates and sends a status message with the current time, valve configuration,
 *  and whether the configuration matches the target
 *
 * Params:
 * 		StateVars <struct>: System controls struct
 * 		ctrl->valveConfiguration <uint32_t>: Valve Configuration
 * 		ctrl->valveTarget <uint32_t>: Valve Target Configuration
 *
 * Returns:
 *  	success <uint32_t>: TRUE | FALSE | 1 | 0
 *  		Return TRUE if this is a tick AND valveConfiguration == valveTarget
 *  		Else return FALSE
 *
 *	Notes:
 */
uint32_t SendStatusMessage(struct StateVars *ctrl)
{
	uint32_t success = FALSE;

	ctrl->valveConfiguration = StateConfiguration();
	success = (ctrl->valveConfiguration == ctrl->valveTarget ? TRUE : FALSE);

	// Create Message and Transmit
	Get_Valve_State_Status_Msg(TxMessageBuffer1, ctrl, success);
	UART_SendMessage(&hlpuart1, TxMessageBuffer1);

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
/*
		uint32_t match = FALSE;
		uint32_t iter = 0;
		uint32_t start = 0;
		uint32_t take = 0;

		char test;
		char comp = 'A';
		char *preamble_comp = "A5A5";

		success = FALSE;
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
*/
