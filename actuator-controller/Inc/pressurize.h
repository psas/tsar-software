// fuel_intro.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//  - Renamed to PRESSURIZE 12/7/2020 [APJ]
//
//	This is the header file for fuel_intro.h
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef PRESSURIZE_H
#define PRESSURIZE_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t Pressurize(struct StateVars (*));

#endif // PRESSURIZE_H
