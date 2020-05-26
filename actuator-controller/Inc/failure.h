// failure.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//
//	This is the header file for failure.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef FAILURE_H
#define FAILURE_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t Failure(enum StateName, enum StateName);

#endif // FAILURE_H
