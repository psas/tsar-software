// safety.h
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This is the header file for safety.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef SAFETY_H
#define SAFETY_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t Safety(struct StateVars (*));

#endif // SAFETY_H
