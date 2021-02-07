// burn_termination_3.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//  - Renamed SHUTDOWN 12/7/2020 [APJ]
//
//	This is the header file for burn_termination_3.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef SHUTDOWN_H
#define SHUTDOWN_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t Shutdown(struct StateVars (*));

#endif // SHUTDOWN_H
