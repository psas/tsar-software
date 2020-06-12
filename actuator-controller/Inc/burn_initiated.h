// burn_initiated.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//
//	This is the header file for burn_initiated.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef BURN_INITIATED_H
#define BURN_INITIATED_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"


uint32_t BurnInitiated(struct StateVars (*));

#endif // BURN_INITIATED_H
