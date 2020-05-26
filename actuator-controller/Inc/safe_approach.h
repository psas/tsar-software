// safe_appraoch.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//
//	This is the header file for safe_approach.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef SAFE_APPROACH_H
#define SAFE_APPROACH_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t SafeApproach(enum StateName (*), enum StateName (*));

#endif // SAFE_APPROACH_H
