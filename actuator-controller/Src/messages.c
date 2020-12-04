// messages.c
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
// Change Log
// 	-Creation of Document 5/18/2020 [APJ]
//
//	This file contains methods for getting common actuator controller
//  messages.
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#include "messages.h"
// TODO
// Encode these messages in hex strings
// Add Parity/Checksum

uint32_t ProcessMessages()
{
// TODO Finish
// Payload Description
// Start Byte 		AA
// Payload Size 	0D
// Payload			--
// Valve Out State  0x7FF
// Valve
	return FALSE;
}

void Get_State_Disagree_Error_Msg(char *errorMessage, enum StateName expectedState, \
			enum StateName passedState)
{
	char state_disagree[PRINT_BUFFER_SIZE] = "ERROR: Expected State, Passed State Disagree. ";
	char temp[32];
	strcat(state_disagree, "Expected: ");
	sprintf(temp, "%d", expectedState);
	strcat(state_disagree, temp);
	strcat(state_disagree,"Passed: ");
	sprintf(temp, "%d", passedState);
	strcat(state_disagree, temp);
	memset(errorMessage, '\0', PRINT_BUFFER_SIZE);
	strcpy(errorMessage, state_disagree);
}

void Get_Invalid_State_Error_Msg(char *errorMessage, enum StateName state, enum StateName lastState)
{
	char state_invalid[PRINT_BUFFER_SIZE] = "Invalid State: ";
	char temp[32];
	sprintf(temp, "%d",state);
	strcat(state_invalid, temp);
	strcat(state_invalid, ", lastState: ");
	sprintf(temp, "%d", lastState);
	strcat(state_invalid, temp);
	memset(errorMessage, '\0', PRINT_BUFFER_SIZE);
	strcpy(errorMessage, state_invalid);
}

void Get_Valve_State_Status_Msg(char *statusMessage, uint32_t valveConfiguration, uint32_t success)
{
	char valve_state[PRINT_BUFFER_SIZE] = "Valve Configuration: ";
	char temp[32];
	sprintf(temp, "%03x", (int)valveConfiguration);
	strcat(valve_state, temp);
	strcat(valve_state, ", Success: ");
	strcat(valve_state, (success > 0 ? "True." : "False."));
	memset(statusMessage, '\0', PRINT_BUFFER_SIZE);
	strcpy(statusMessage, valve_state);
}
