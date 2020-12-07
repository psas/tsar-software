// messages.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 5/25/2020 [APJ]
//
//	This is the header file for messages.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#ifndef __MESSAGES_H
#define __MESSAGES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utilities.h"
#include "init.h"
#include "main.h"
#include "states_def.h"

uint32_t ProcessMessages(struct StateVars (*));
uint32_t SendStatusMessage(struct StateVars (*));
void Get_State_Disagree_Error_Msg(char (*),enum StateName , enum StateName );
void Get_Invalid_State_Error_Msg(char (*), enum StateName , enum StateName );
void Get_Valve_State_Status_Msg(char (*), struct StateVars (*) , uint32_t );

volatile char TxMessageBuffer1[];
volatile char RxMessageBuffer1[];
volatile char *RxMessageIdx;

#endif
