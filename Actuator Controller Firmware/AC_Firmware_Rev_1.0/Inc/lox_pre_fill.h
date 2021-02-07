// safe_appraoch.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//  - Renamed LOX PRE FILL 12/7/2020 [APJ]
//
//	This is the header file for safe_approach.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef LOX_PRE_FILL_H
#define LOX_PRE_FILL_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t LoxPreFill(struct StateVars (*));

#endif // LOX_PRE_FILL_H
