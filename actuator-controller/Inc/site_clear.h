// site_clear.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//
//	This is the header file for site_clear.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef SITE_CLEAR_H
#define SITE_CLEAR_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t SiteClear(enum StateName (*), enum StateName (*));

#endif // SITE_CLEAR_H