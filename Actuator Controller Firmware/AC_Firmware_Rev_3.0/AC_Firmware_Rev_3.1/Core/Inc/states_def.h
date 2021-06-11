// states_def.h
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This file describes the allowable states of the Actuator-
//  Controller.  They are organized in roughly chronological order
//  Actual state diagrams include various identical states for
//  Set up and Post Fire that are identical to the software.  Those
//  states have been excluded.
//
//	References:
//
//
//  System Risk Factor = 0.25 (Catastrophic, Improbable)


#ifndef STATES_DEF_H
#define STATES_DEF_H

#include "stdint.h"


#define VALVE_STATE_BUFFER_SIZE 96


// Buffer Flags Definitions
#define RX_DATA_READY (1 << 0)//01

enum StateName{					//    SOV1   SOV2   SOV3   SOV4   SOV5   SOV6   SOV7   SOV8    SOV9   IG1
	START_HANDLER,              // 0 |  0   |  0  |   0  |   0  |   0  |   0  |   0  |   0  |   0   |  0
	VALVE_CHECK,				// 1 | 1 0  | 1 0 |  1 0 |  1 0 |  1 0 |  1 0 |  1 0 |  1 0 |  1 0  | 1 0
	LOX_PRE_FILL,			    // 2 |  0	|  1  |   0  |   1  |   0  |   1  |   0  |   0  |   0   |  0
	LOX_FILL,                   // 3 |  0   |  0  |   0  |   1  |   0  |   0  |   1  |   1  |   0   |  0
	LOX_CHILL,					// 4 |  0   |  0  |   0  |   1  |   0  |   1  |   0  |   1  |   0   |  0
	SAFETY,						// 5 |  0   |  0  |   0  |   1  |   0  |   0  |   0  |   1  |   0   |  0
	PRESSURIZE,					// 6 |  1   |  1  |   0  |   0  |   0  |   0  |   0  |   0  |   0   |  0
	IGNITION,					// 7 |  1   |  1  |   0  |   0  |   0  |   0  |   0  |   0  |   0   |  1
	OXIDIZER_START,				// 8 |  1   |  1  |   0  |   0  |   0  |   1  |   0  |   0  |   0   |  0
	FIRING,						// 9 |  1   |  1  |   0  |   0  |   1  |   1  |   0  |   0  |   0   |  0
	PURGE,						// A |  0   |  0  |   1  |   0  |   0  |   0  |   0  |   0  |   1   |  0
	TURN_OFF,					// B |  0   |  0  |   0  |   1  |   0  |   0  |   0  |   1  |   0   |  0
	AUTOMATIC_SHUTDOWN,		    // C | Purge -> Safety
};

struct StateVars{
uint32_t timeStarted;
uint32_t timeElapsed;
uint32_t stateCounter;
uint32_t safety;
uint32_t pressurizing;
uint32_t lox_fill;
uint32_t lox_chill;
uint32_t lox_pre_fill;
uint32_t valve_check;
uint32_t ignition;
uint32_t isStateMachineRunning;
uint32_t valveConfiguration;
uint32_t valveTarget;

enum StateName lastState;
enum StateName currentState;

struct StateVars *adr; // Struct pointer to use -> to access the struct
};

// Definitions of Solenoids
#define	SOV1  0x001 //1
#define SOV2  0x002 //2
#define SOV3  0x004 //4
#define SOV4  0x008 //8
#define SOV5  0x010 //16
#define SOV6  0x020 //32
#define SOV7  0x040 //64
#define SOV8  0x080 //128
#define SOV9  0x100 //256
#define IG1   0x200 //512


// Bool Definition
#define TRUE  1
#define FALSE 0

#endif // STATES_DEF_H
