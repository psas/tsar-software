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

void Get_State_Disagree_Error_Msg(char (*),enum StateName , enum StateName );
void Get_Invalid_State_Error_Msg(char (*), enum StateName , enum StateName );
void Get_Valve_State_Status_Msg(char (*), uint32_t , uint32_t );

#endif
