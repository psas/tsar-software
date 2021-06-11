// utilities.h
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This is the header file for utilities.c
//
//	References:
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef UTILITIES_H
#define UTILITIES_H

#include <init.h>
#include "states_def.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx.h"
#include "stm32l4xx_hal_uart.h"
#include "stm32l4xx_hal_uart_ex.h"
#include "stm32l4xx_hal_def.h"
#include <stdio.h>
#include <string.h>

uint32_t VerifyState(int);
uint32_t StateConfiguration();
uint32_t ValveStateSetter(uint32_t);
uint32_t StateInitialize(struct StateVars (*));
void StateStatus(struct StateVars *ctrl);
void ValveCheckCmd(struct StateVars *ctrl);
uint32_t CycleValves(struct StateVars (*));
//uint16_t OffSpecificValves(uint16_t current_valves, uint16_t valve_off);
//char action[5];
uint v1,v2,v3,v4,v5,v6,v7,v8,v9,ignit;


#endif // UTILITIES_H
