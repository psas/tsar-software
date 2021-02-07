// burn_feedback.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//  - Renamed LOX_START 12/7/2020 [APJ]
//
//	This is the header file for burn_feedback.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef LOX_START_H
#define LOX_START_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t LoxStart(struct StateVars (*));

#endif // LOX_START_H
