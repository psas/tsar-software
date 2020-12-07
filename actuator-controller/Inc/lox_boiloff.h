// post_fire.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//  - Renamed LOX_BOILOFF 12/7/2020 [APJ]
//
//	This is the header file for post_fire.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef LOX_BOILOFF_H
#define LOX_BOILOFF_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t LoxBoiloff(struct StateVars (*));

#endif // LOX_BOILOFF_H
