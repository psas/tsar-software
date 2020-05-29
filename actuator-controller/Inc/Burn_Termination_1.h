// burn_termination_1.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//
//	This is the header file for burn_termination_1.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef BURN_TERMINATION_1_H
#define BURN_TERMINATION_1_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"


uint32_t BurnTermination1(enum StateName (*), enum StateName (*));

#endif // BURN_TERMINATION_1_H