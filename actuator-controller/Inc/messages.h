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
uint32_t OnTickStatusMessage(struct StateVars (*));
uint32_t SendStatusMessage(struct StateVars (*));
void GetMessage(char (*), uint32_t);
uint32_t ProcessCommands(struct StateVars (*), char (*));
uint32_t FindMessageReturnSize(char (*));
void Get_State_Disagree_Error_Msg(char (*),enum StateName , enum StateName );
void Get_Invalid_State_Error_Msg(char (*), enum StateName , enum StateName );
void Get_Valve_State_Status_Msg(char (*), struct StateVars (*) , uint32_t );

char TxMessageBuffer1[256];
char RxMessageBuffer1[256];
char *RxEndofBuffer;
char *RxMessageReader;

volatile char *RxMessageIdx;
volatile uint32_t RxTxFlags;

static char *preamble = "A5A5";
static char *armCmd = "A5A503ARM";
static char *powerOffCmd = "A5A508POWEROFF";
static char *emergencyStopCmd = "A5A505ESTOP";
static char *failureAckCmd = "A5A507FAILACK";

#endif
