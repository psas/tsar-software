// purge.h
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This is the header file for purge.c
//
//	References:
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef PURGE_H
#define PURGE_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t Purge(struct StateVars (*));

#endif // PURGE_H
