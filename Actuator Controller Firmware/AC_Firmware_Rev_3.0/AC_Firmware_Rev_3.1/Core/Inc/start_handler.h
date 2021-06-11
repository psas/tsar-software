// start_handler.h
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This is the header file for start_handler.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef STARTHANDLER_H
#define STARTHANDLER_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"


uint32_t StartHandler(struct StateVars (*));

#endif // STARTHANDLER_H
