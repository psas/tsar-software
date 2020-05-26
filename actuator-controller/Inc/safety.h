// safety.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 5/17/2020 [APJ]
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

uint32_t Safety(enum StateName, enum StateName, uint32_t isSafetyGreen);

#endif // SAFETY_H
