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

#include "states_def.h"
#include "setup_ops.h"
#include "lox_fill.h"
#include "site_clear.h"
#include "pressurization.h"
#include "pre_chill.h"
#include "ignition.h"
#include "lox_intro.h"
#include "fuel_intro.h"
#include "burn_initiated.h"
#include "burn_feedback.h"
#include "burn_termination_1.h"
#include "burn_termination_2.h"
#include "burn_termination_3.h"
#include "purge.h"
#include "post_fire.h"
#include "safe_approach.h"
#include "failure.h"
#include "groundstation.h"
#include "test.h"
#include "utilities.h"

void StateMachine(bool, enum StateName);

#endif

