// states_machine.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//
//	This is the header file for state_machine.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "safety.h"
#include "states_def.h"
#include "valve_check.h"
#include "fuel_fill.h"
#include "lox_pre_fill.h"
#include "lox_chill.h"
#include "lox_fill.h"
#include "ready.h"
#include "pressurize.h"
#include "ignition.h"
#include "lox_start.h"
#include "fuel_start.h"
#include "firing.h"
#include "purge.h"
#include "lox_boiloff.h"
#include "shutdown.h"
#include "fuel_drain.h"
#include "failure.h"
#include "test.h"
#include "utilities.h"

void StateMachine(uint32_t, struct StateVars (*));

#endif

