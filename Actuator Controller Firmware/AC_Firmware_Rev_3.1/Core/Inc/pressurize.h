// pressurize.h
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This is the header file for pressurize.h
//
//	References:
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef PRESSURIZE_H
#define PRESSURIZE_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t Pressurize(struct StateVars (*));

#endif // PRESSURIZE_H
