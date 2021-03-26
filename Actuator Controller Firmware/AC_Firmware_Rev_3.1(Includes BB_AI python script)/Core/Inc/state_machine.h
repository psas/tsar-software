// state_machine.h
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This is the header file for state_machine.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <automatic_shutdown.h>
#include <lox_fill.h>
#include <lox_chill.h>
#include <oxidizer_start.h>
#include <turnoff.h>
#include "safety.h"
#include "states_def.h"
#include "valve_check.h"
#include "pressurize.h"
#include "ignition.h"
#include "firing.h"
#include "purge.h"
#include "utilities.h"

void StateMachine(uint32_t, struct StateVars (*));

#endif

