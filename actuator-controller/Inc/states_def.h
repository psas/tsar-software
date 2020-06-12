// states_def.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/9/2020 [APJ]
//	-Created StateName Enumeration, Solenoid Macros 2/9/2020 [APJ]
//
//	This file describes the allowable states of the Actuator-
//  Controller.  They are organized in roughly chronological order
//  Actual state diagrams include various identical states for
//  Set up and Post Fire that are identical to the software.  Those
//  states have been excluded.
//
//	References:
//	Valve_State_Reference.xls (Currently Located in LFETS)
//
//  System Risk Factor = 0.25 (Catastrophic, Improbable)


#ifndef STATES_DEF_H
#define STATES_DEF_H

#include "stdint.h"

#define PRINT_BUFFER_SIZE 256

enum StateName{					// PV1	PV2	PV3	VV1	VV2	IV1	IV2	MV1	MV2
	SETUP_OPS,					// |11	|11	|11	|11 |11	|11	|11	|11	|11
	//N2_AND_FUEL_FILL,			// | 0	| 0	| 0	| 1 | 1	| 0	| 0	| 0	| 0
	//FUEL_LINE_CLEAR,			// | 0	| 0	| 0	| 1 | 1	| 0	| 0	| 0	| 0
	//MID_FUELING_HOLD,			// | 0	| 0	| 0	| 1 | 1	| 0	| 0	| 0	| 0
	//LOX_FILL,					// | 0	| 0	| 0	| 1 | 1	| 0	| 0	| 0	| 0
	SITE_CLEAR_AND_START_UP,	// | 0	| 0	| 0	| 1 | 1	| 0	| 0	| 0	| 0
	PRESSURIZATION,				// | 0	| 1	| 1	| 0 | 0	| 0	| 0	| 0	| 0
	PRE_CHILL,					// | 0	| 1	| 1	| 0 | 0	| 0	| 0	|10	| 0
	IGNITION,					// | 0	| 1	| 1	| 0 | 0	| 1	| 1	|10	| 0
	LOX_INTRO,					// | 0	| 1	| 1	| 0 | 0	| 1	| 1	| 1	|10
	FUEL_INTRO,					// | 0	| 1	| 1	| 0 | 0	| 1	| 1	| 1	| 1
	BURN_INITIATED,				// | 0	| 1	| 1	| 0 | 0	| 1	| 1	| 1	| 1
	BURN_FEEDBACK,				// | 0	| 1	| 1	| 0 | 0	| 0	| 0	| 1	| 1
	BURN_TERMINATION_1,			// | 0	| 1	| 1	| 0 | 0	| 0	| 0	| 1	|10
	BURN_TERMINATION_2,			// | 1	| 1	| 1	| 1 | 1	| 0	| 0	|10	| 0
	BURN_TERMINATION_3,			// | 1	| 0	| 0	| 1 | 1	| 0	| 0	| 0	| 0
	PURGE,						// | 1	| 0	| 0	| 1 | 1	| 0	| 0	| 0	| 0
	POST_FIRE,					// | 0	| 0	| 0	| 1 | 1	| 0	| 0	| 0	| 0
	//SAFE_APPROACH,			// | 0	| 0	| 0	| 1 | 1	| 0	| 0	| 0	| 0
	FAILURE,					// | 0	| 0	| 0	| 1 | 1	| 0	| 0	| 0	| 0
	//GROUNDSTATION,			// | 0	| 0	| 0	| 1 | 1	| 0	| 0	| 0	| 0
	SAFETY,						// | 0	| 0	| 0	| 1 | 1	| 0	| 0	| 0	| 0
	TEST						// | *	| *	| *	| * | *	| *	| *	| *	| *
};

struct StateVars{
uint32_t timeStarted;
uint32_t timeElapsed;
uint32_t isArmed;
uint32_t ignition;
enum StateName lastState;
enum StateName currentState;
uint32_t *adr;
};

// Definitions of Solenoids
#define	PV1  0x001				// GPIOA GPIO8_Pin (uint_16t)0x0100
#define PV2  0x002				// GPIOA GPIO9_FAST_Pin (uint16_t)0x0200
#define PV3  0x004				// GPIOA GPIO10_FAST_Pin (uint16_t)0x0400
#define VV1  0x008				// GPIOB GPIO13_Pin (uint16_t)0x2000
#define VV2  0x010				// GPIOB GPIO14_Pin (uint16_t)0x4000
#define IV1  0x020				// GPIOB GPIO15_Pin (uint16_t)0x8000
#define IV2  0x040				// GPIOD GPIO8D8_Pin (uint_16t)0x0100
#define MV1  0x080				// GPIOD GPIO9_Pin (uint_16t)0x0200
#define MV2  0x100				// GPIOD GPIO2_Pin (uint_16t)0x0004

// Bool Defs
#define TRUE  1
#define FALSE 0

#endif // STATES_DEF_H
