// automatic_shutdown.h
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This is the header file for automatic_shutdown.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef AUTOMATIC_SHUTDOWN_H
#define AUTOMATIC_SHUTDOWN_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t AutomaticShutdown(struct StateVars (*));

#endif // AUTOMATIC_SHUTDOWN_H
