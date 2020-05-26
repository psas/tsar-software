// purge.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//
//	This is the header file for purge.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef PURGE_H
#define PURGE_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t Purge(enum StateName (*), enum StateName (*));

#endif // PURGE_H
