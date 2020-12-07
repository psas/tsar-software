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
#define RX_BUFFER_SIZE 256
#define TX_BUFFER_SIZE 128
#define VALVE_STATE_BUFFER_SIZE 96

#define TICK_LENGTH 2000

enum StateName{					//    SOV1   SOV2   SOV3   SOV4   SOV5   SOV6   SOV7   SOV8
	SAFETY,						// 0 |  0   |  0  |   0  |   1  |   0  |   0  |   0  |   1  |
	VALVE_CHECK,				// 1 | 1 0  | 1 0 |  1 0 |  1 0 |  1 0 |  1 0 |  1 0 |  1 0 |
	FUEL_FILL,					// 2 |  0   |  0  |   0  |   1  |   0  |   0  |   0  |   1  |
	LOX_PRE_FILL,				// 3 |  0   |  1  |   0  |   0  |   0  |   1  |   0  |   0  |
	LOX_CHILL,					// 4 |  0   |  0  |   0  |   0  |   0  |   1  |   1  |   1  |
	LOX_FILL,					// 5 |  0   |  0  |   0  |   0  |   0  |   1  |   1  |   1  |
	READY_STATE,				// 6 |  0   |  0  |   0  |   1  |   0  |   0  |   0  |   1  |
	PRESSURIZE,					// 7 |  1   |  1  |   0  |   0  |   0  |   0  |   0  |   0  |
	IGNITION,					// 8 |  1   |  1  |   0  |   0  |   0  |   0  |   0  |   0  |
	LOX_START,					// 9 |  1   |  1  |   0  |   0  |   0  |   1  |   0  |   0  |
	FUEL_START,					// A |  1   |  1  |   0  |   0  |   1  |   1  |   0  |   0  |
	FIRING,						// B |  1   |  1  |   0  |   0  |   1  |   1  |   0  |   0  |
	PURGE,						// C |  0   |  0  |   1  |   0  |   0  |   0  |   0  |   0  |
	LOX_BOILOFF,				// D |  0   |  1  |   0  |   0  |   0  |   1  |   0  |   0  |
	SHUTDOWN,					// E |  0   |  0  |   1  |   0  |   0  |   0  |   0  |   0  |
	FUEL_DRAIN,					// F |  0   |  0  |   0  |   1  |   0  |   0  |   0  |   1  |
	FAILURE,					// 10|  0   |  0  |   1  |   0  |   0  |   0  |   0  |   0  |
	TEST						// 20| 1 0  | 1 0 |  1 0 |  1 0 |  1 0 |  1 0 |  1 0 |  1 0 |
};

struct StateVars{
uint32_t timeStarted;
uint32_t timeElapsed;
uint32_t stateCounter;
uint32_t isArmed;
uint32_t ignition;
uint32_t isStateMachineRunning;
uint32_t valveConfiguration;
uint32_t valveTarget;
enum StateName lastState;
enum StateName currentState;
struct StateVars *adr;
};

// Definitions of Solenoids
#define	SOV1  0x001				// GPIOA GPIO10_FAST_Pin (uint16_t)
#define SOV2  0x002				// GPIOD GPIO8D8_Pin (uint16_t)
#define SOV3  0x004				// GPIOD GPIO9_Pin (uint16_t)
#define SOV4  0x008				// GPIOC GPIO7_Pin (uint16_t)
#define SOV5  0x010				// GPIOC GPIO6_Pin (uint16_t)
#define SOV6  0x020				// GPIOA GPIO9_FAST_Pin (uint16_t)
#define SOV7  0x040				// GPIOA GPIO8_Pin (uint16_t)
#define SOV8  0x080				// GPIOB GPIO14_Pin (uint16_t)
// extra
#define XXX1  0x100				// GPIOB GPIO13_Pin (uint16_t)
#define XXX2  0x200				// GPIOB GPIO15_Pin (uint16_t)
#define XXX3  0x400				// GPIOD GPIO2_Pin (uint16_t)

// Bool Defs
#define TRUE  1
#define FALSE 0

#endif // STATES_DEF_H
