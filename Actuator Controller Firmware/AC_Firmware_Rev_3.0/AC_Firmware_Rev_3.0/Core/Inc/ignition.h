// ignition.h
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This is the header file for ignition.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef IGNITION_H
#define IGNITION_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"


uint32_t Ignition(struct StateVars (*));

#endif // IGNITION_H
