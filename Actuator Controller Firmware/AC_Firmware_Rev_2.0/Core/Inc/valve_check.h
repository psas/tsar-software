// valve_check.h
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This is the header file for valve_check.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef VALVE_CHECK_H
#define VALVE_CHECK_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t ValveCheck(struct StateVars (*));
uint32_t CycleValves(struct StateVars (*));

#endif // VALVE_CHECK_H
