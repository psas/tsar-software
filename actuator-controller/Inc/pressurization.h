// pressurization.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//
//	This is the header file for pressurization.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef PRESSURIZATION_H
#define PRESSURIZATION_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t Pressurization(enum StateName (*), enum StateName (*));

#endif // PRESSURIZATION_H
