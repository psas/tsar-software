// tsar_setup_ops.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//
//	This is the header file for setup_ops.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef TSAR_SETUP_OPS_H
#define TSAR_SETUP_OPS_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t SetupOps(enum StateName (*), enum StateName (*));

#endif // SETUP_OPS_H
