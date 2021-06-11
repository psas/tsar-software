// turnoff.h
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This is the header file for turnoff.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef TURNOFF_H
#define TURNOFF_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t TurnOff(struct StateVars (*));

#endif // TURNOFF_H
